#include "ara/core/initialization.h"
#include "deepracer/lidardata/lidar_skeleton.h"
#include "ara/exec/execution_client.h"
#include <thread>

int main(int argc, char *argv[])
{
    ara::core::Initialize();
    
    // TODO: your code here
    uint8_t send_value = 1;

    ara::core::InstanceSpecifier specifier("Lidar/LidarAA/PPortLidar");
    deepracer::lidardata::skeleton::LidarSkeleton lidarPPort(specifier);
    lidarPPort.OfferService();

    ara::exec::ExecutionClient executionClient;
    auto exec = executionClient.ReportExecutionState(ara::exec::ExecutionState::kRunning);

    do
    {
        lidarPPort.TimingEvent_100ms.Send(send_value);
        printf("[P]Lidar: %d\n", send_value);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    } while (true);
    
    ara::core::Deinitialize();
    
    return 0;
}