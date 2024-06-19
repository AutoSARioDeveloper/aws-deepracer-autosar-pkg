#include "ara/core/initialization.h"
#include "deepracer/navigationdata/navigation_proxy.h"
#include "ara/exec/execution_client.h"
#include <thread>
 
int main(int argc, char *argv[])
{
    ara::core::Initialize();
    
    // TODO: your code here
    ara::exec::ExecutionClient executionClient;
    auto exec = executionClient.ReportExecutionState(ara::exec::ExecutionState::kRunning);

    std::unique_ptr<deepracer::navigationdata::proxy::NavigationProxy> navigationRPort{nullptr};
    ara::core::InstanceSpecifier navigationSpecifier("Servo/ServoAA/RPortNavigation");

    auto callback = [&](auto services, auto handler) {
        for (auto service : services)
        {   
            if ((service.GetServiceHandle().serviceId == 84) && (service.GetServiceHandle().version.major == 1))
            {
                navigationRPort = std::make_unique<deepracer::navigationdata::proxy::NavigationProxy>(service);
                navigationRPort->TimingEvent_100ms.Subscribe(10);
                auto receiveNavigationEvent = [&] () {
                    navigationRPort->TimingEvent_100ms.GetNewSamples([&](auto noti){
                        printf("[R-Servo]Navigation: %d\n", *noti);
                     });
                };
                navigationRPort->TimingEvent_100ms.SetReceiveHandler(receiveNavigationEvent);
            } 
        }
    };

    auto retNavigation = deepracer::navigationdata::proxy::NavigationProxy::StartFindService(callback, navigationSpecifier);

    while(true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    
    ara::core::Deinitialize();
    
    return 0;
}
