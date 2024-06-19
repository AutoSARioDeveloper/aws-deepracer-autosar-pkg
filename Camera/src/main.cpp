#include "ara/core/initialization.h"
#include "deepracer/cameradata/camera_skeleton.h"
#include "ara/exec/execution_client.h"
#include <thread>

int main(int argc, char *argv[])
{
    ara::core::Initialize();
    
    // TODO: your code here
    uint8_t send_value = 0;
    
    ara::core::InstanceSpecifier specifier("Camera/CameraAA/PPortCamera");
    deepracer::cameradata::skeleton::CameraSkeleton cameraPPort(specifier);
    cameraPPort.OfferService();

    ara::exec::ExecutionClient executionClient;
    auto exec = executionClient.ReportExecutionState(ara::exec::ExecutionState::kRunning);
 
    do  
    {
        cameraPPort.TimingEvent_100ms.Send(send_value);
        printf("[P]Camera: %d\n", send_value);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    } while (true);
    
    ara::core::Deinitialize();
    
    return 0;
}
