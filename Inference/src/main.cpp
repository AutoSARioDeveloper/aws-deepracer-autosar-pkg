#include "ara/core/initialization.h"
#include "deepracer/inferencedata/inferece_skeleton.h"
#include "deepracer/sensorfusiondata/sensorfusion_proxy.h"
#include "ara/exec/execution_client.h"
#include <thread>
 
int main(int argc, char *argv[])
{
    ara::core::Initialize();    
    
    // TODO: your code here
    uint8_t send_value = 3;

    ara::core::InstanceSpecifier inferenceSpecifier("Inference/InferenceAA/PPortInference");
    deepracer::inferencedata::skeleton::InfereceSkeleton infernecePPort(inferenceSpecifier);
    infernecePPort.OfferService();

    ara::exec::ExecutionClient executionClient;
    auto exec = executionClient.ReportExecutionState(ara::exec::ExecutionState::kRunning);

    std::unique_ptr<deepracer::sensorfusiondata::proxy::SensorFusionProxy> sensorfusionRPort{nullptr};
    ara::core::InstanceSpecifier sensorfusionSpecifier("Inference/InferenceAA/RPortSensorFusion");

    auto callback = [&](auto services, auto handler) {
        for (auto service : services)
        {   
            if ((service.GetServiceHandle().serviceId == 82) && (service.GetServiceHandle().version.major == 1))
            {
                sensorfusionRPort = std::make_unique<deepracer::sensorfusiondata::proxy::SensorFusionProxy>(service);
                sensorfusionRPort->TimingEvent_100ms.Subscribe(10);
                auto receiveSensorFusionEvent = [&] () {
                    sensorfusionRPort->TimingEvent_100ms.GetNewSamples([&](auto noti){
                        printf("[R]SensorFusion: %d\n", *noti);
                     });
                };
                sensorfusionRPort->TimingEvent_100ms.SetReceiveHandler(receiveSensorFusionEvent);
            } 
        }
    };

    auto retSensorFusion = deepracer::sensorfusiondata::proxy::SensorFusionProxy::StartFindService(callback, sensorfusionSpecifier);
 
    do
    {
        infernecePPort.TimingEvent_100ms.Send(send_value);
        printf("[P]Inference: %d\n", send_value);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    } while (true);
    

    ara::core::Deinitialize();
    
    return 0;
}
