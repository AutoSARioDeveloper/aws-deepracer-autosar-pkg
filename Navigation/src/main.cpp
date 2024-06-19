#include "ara/core/initialization.h"
#include "deepracer/inferencedata/inferece_proxy.h"
#include "deepracer/navigationdata/navigation_skeleton.h"
#include "ara/exec/execution_client.h"
#include <thread>
 
int main(int argc, char *argv[])
{
   ara::core::Initialize();
    
    // TODO: your code here
    uint8_t send_value = 4;

    ara::core::InstanceSpecifier navigationSpecifier("Navigation/NavigationAA/PPortNavigation");
    deepracer::navigationdata::skeleton::NavigationSkeleton navigationPPort(navigationSpecifier);
    navigationPPort.OfferService();
    
    ara::exec::ExecutionClient executionClient;
    auto exec = executionClient.ReportExecutionState(ara::exec::ExecutionState::kRunning);

    std::unique_ptr<deepracer::inferencedata::proxy::InfereceProxy> inferenceRPort{nullptr};
    ara::core::InstanceSpecifier inferenceSpecifier("Navigation/NavigationAA/RPortInference");

    auto callback = [&](auto services, auto handler) {
        for (auto service : services)
        {   
            if ((service.GetServiceHandle().serviceId == 83) && (service.GetServiceHandle().version.major == 1))
            {
                inferenceRPort = std::make_unique<deepracer::inferencedata::proxy::InfereceProxy>(service);
                inferenceRPort->TimingEvent_100ms.Subscribe(10);
                auto receiveInferenceEvent = [&] () {
                    inferenceRPort->TimingEvent_100ms.GetNewSamples([&](auto noti){
                        printf("[R]Inference: %d\n", *noti);
                    });
                };
                inferenceRPort->TimingEvent_100ms.SetReceiveHandler(receiveInferenceEvent);
            } 
        }
    };

    auto retInference = deepracer::inferencedata::proxy::InfereceProxy::StartFindService(callback, inferenceSpecifier);

    do
    {
        navigationPPort.TimingEvent_100ms.Send(send_value);
        printf("[P]Navigation: %d\n", send_value);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    } while (true);
    

    ara::core::Deinitialize();
    
    return 0;
}
