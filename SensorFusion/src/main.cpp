#include "ara/core/initialization.h"
#include "deepracer/cameradata/camera_skeleton.h"
#include "deepracer/cameradata/camera_proxy.h"
#include "deepracer/lidardata/lidar_proxy.h"
#include "deepracer/simulatordata/simulator_proxy.h"
#include "deepracer/sensorfusiondata/sensorfusion_skeleton.h"
#include "ara/exec/execution_client.h"
#include <thread>
 
int main(int argc, char *argv[])
{
    ara::core::Initialize();
    
    // TODO: your code here
    uint8_t send_value = 2;

    ara::core::InstanceSpecifier sensorfusionSpecifier("SensorFusion/SensorFusionAA/PPortSensorFusion");
    deepracer::sensorfusiondata::skeleton::SensorFusionSkeleton sensorfusionPPort(sensorfusionSpecifier);
    sensorfusionPPort.OfferService();

    ara::exec::ExecutionClient executionClient;
    auto exec = executionClient.ReportExecutionState(ara::exec::ExecutionState::kRunning);

    std::unique_ptr<deepracer::cameradata::proxy::CameraProxy> cameraRPort{nullptr};
    ara::core::InstanceSpecifier cameraSpecifier("SensorFusion/SensorFusionAA/RPortCamera");

    auto cameraCallback = [&](auto services, auto handler) {
        for (auto service : services)
        {
            if((service.GetServiceHandle().serviceId == 80) && (service.GetServiceHandle().version.major == 1))
            { 
                
                cameraRPort = std::make_unique<deepracer::cameradata::proxy::CameraProxy>(service);
                cameraRPort->TimingEvent_100ms.Subscribe(10);
                auto receiveCameraEvent = [&] () {
                    cameraRPort->TimingEvent_100ms.GetNewSamples([&](auto noti){
                        printf("[R]Camera: %d\n", *noti);
                    });
                };
                cameraRPort->TimingEvent_100ms.SetReceiveHandler(receiveCameraEvent);
            }
        } 
    };
    auto retCamera = deepracer::cameradata::proxy::CameraProxy::StartFindService(cameraCallback, cameraSpecifier);

    std::unique_ptr<deepracer::lidardata::proxy::LidarProxy> lidarRPort{nullptr};
    ara::core::InstanceSpecifier lidarSpecifier("SensorFusion/SensorFusionAA/RPortLidar");

    auto lidarCallback = [&](auto services, auto handler) {
        for (auto service : services)
        {   
            if((service.GetServiceHandle().serviceId == 81) && (service.GetServiceHandle().version.major == 1))
            {
                lidarRPort = std::make_unique<deepracer::lidardata::proxy::LidarProxy>(service);
                lidarRPort->TimingEvent_100ms.Subscribe(10);
                auto receiveLidarEvent = [&] () {
                    lidarRPort->TimingEvent_100ms.GetNewSamples([&](auto noti){
                        printf("[R]Lidar: %d\n", *noti);
                    });
                };
                lidarRPort->TimingEvent_100ms.SetReceiveHandler(receiveLidarEvent);
            }
        }
    };

    auto retLidar = deepracer::lidardata::proxy::LidarProxy::StartFindService(lidarCallback, lidarSpecifier);
    


    std::unique_ptr<deepracer::simulatordata::proxy::SimulatorProxy> simulatorRPort{nullptr};
    ara::core::InstanceSpecifier simulatorSpecifier("SensorFusion/SensorFusionAA/RPortSimulator");

    auto simulatorCallback = [&](auto services, auto handler) {
        for (auto service : services)
        {   
            if((service.GetServiceHandle().serviceId == 86) && (service.GetServiceHandle().version.major == 1))
            {
                simulatorRPort = std::make_unique<deepracer::simulatordata::proxy::SimulatorProxy>(service);
                simulatorRPort->TimingEvent_100ms.Subscribe(10);
                auto receiveSimulatorEvent = [&] () {
                    simulatorRPort->TimingEvent_100ms.GetNewSamples([&](auto noti){
                        printf("[R]Simulator: %d\n", *noti);
                    });
                };
                simulatorRPort->TimingEvent_100ms.SetReceiveHandler(receiveSimulatorEvent);
            }
        }
    };

    auto retSimulator = deepracer::simulatordata::proxy::SimulatorProxy::StartFindService(simulatorCallback, simulatorSpecifier);
  
    do
    {
        sensorfusionPPort.TimingEvent_100ms.Send(send_value);
        printf("[P]SensorFusion: %d\n", send_value);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    } while (true);

    ara::core::Deinitialize();
    
    return 0;
}
