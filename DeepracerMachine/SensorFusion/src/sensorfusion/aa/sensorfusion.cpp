///////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                        
/// Copyright, 2021 PopcornSAR Co., Ltd. All rights reserved.                                              
/// This software is copyright protected and proprietary to PopcornSAR Co., Ltd.                           
/// PopcornSAR Co., Ltd. grants to you only those rights as set out in the license conditions.             
///                                                                                                        
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// AUTOSAR VERSION                   : R20-11
/// GENERATED BY                      : PARA, Adaptive Application Generator
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// GENERATED FILE NAME               : sensorfusion.cpp
/// SOFTWARE COMPONENT NAME           : SensorFusion
/// GENERATED DATE                    : 2024-12-03 10:27:58
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// INCLUSION HEADER FILES
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "sensorfusion/aa/sensorfusion.h"
 
namespace sensorfusion
{
namespace aa
{
 
SensorFusion::SensorFusion()
    : m_logger(ara::log::CreateLogger("SEFU", "SWC", ara::log::LogLevel::kVerbose))
    , m_workers(3)
    , m_running(false)
{
}
 
SensorFusion::~SensorFusion()
{
}
 
bool SensorFusion::Initialize()
{
    m_logger.LogVerbose() << "SensorFusion::Initialize";
    
    bool init{true};
    
    m_PPortSensorFusion = std::make_shared<sensorfusion::aa::port::PPortSensorFusion>();
    m_RPortCamera = std::make_shared<sensorfusion::aa::port::RPortCamera>();
    m_RPortLidar = std::make_shared<sensorfusion::aa::port::RPortLidar>();
    
    return init;
}
 
void SensorFusion::Start()
{
    m_logger.LogVerbose() << "SensorFusion::Start";
    
    m_RPortCamera->Start();
    m_RPortLidar->Start();
    m_PPortSensorFusion->Start();
    
    // run software component
    Run();
}
 
void SensorFusion::Terminate()
{
    m_logger.LogVerbose() << "SensorFusion::Terminate";

    m_running = false;

    m_PPortSensorFusion->Terminate();
    m_RPortCamera->Terminate();
    m_RPortLidar->Terminate();
}
 
void SensorFusion::Run()
{
    m_logger.LogVerbose() << "SensorFusion::Run";

    m_running = true;

    std::atomic<bool> camStarted{false}, lidarStarted{false};

    m_workers.Async([this, &camStarted] {
        camStarted = true;
        TaskReceiveCamEventCyclic();
    });

    m_workers.Async([this, &lidarStarted] {
        lidarStarted = true;
        TaskReceiveLidarEventCyclic();
    });

    while (!camStarted || !lidarStarted) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    m_workers.Async([this] {
        HandleSensorData();
    });

    m_workers.Wait();
}

void SensorFusion::TaskReceiveCamEventCyclic()
{
    m_logger.LogInfo() << "TaskReceiveCamEventCyclic: Setting ReceiveEventCamEventHandler";
    m_RPortCamera->SetReceiveEventCamEventHandler([this](const auto& sample)
    {
        OnReceiveCamEvent(sample);
    });
    m_RPortCamera->ReceiveEventCamEventCyclic();
}

void SensorFusion::OnReceiveCamEvent(const deepracer::service::camera::proxy::events::CamEvent::SampleType& sample)
{
    m_latestCameraData = std::make_shared<deepracer::serviceinterfacecam::StereoCamera>(sample);
    m_logger.LogInfo() << "SensorFusion::OnReceiveCamEvent: Camera data updated.";
}

void SensorFusion::TaskReceiveLidarEventCyclic()
{
    m_logger.LogInfo() << "TaskReceiveLidarEventCyclic: Setting ReceiveEventLidarEventHandler";
    m_RPortLidar->SetReceiveEventLidarEventHandler([this](const auto& sample)
    {
        OnReceiveLidarEvent(sample);
    });
    m_RPortLidar->ReceiveEventLidEventCyclic();
}

void SensorFusion::OnReceiveLidarEvent(const deepracer::service::lidar::proxy::events::LidEvent::SampleType& sample)
{
    auto lidarData = std::make_shared<deepracer::serviceinterfacelidar::SectorLidar>();
    lidarData->data = sample.data;
    lidarData->timestamp = sample.timestamp;

    {
        std::lock_guard<std::mutex> lock(lidarMutex);
        if (lidarBuffer.size() > 20) {
            lidarBuffer.pop_front();
        }
        lidarBuffer.push_back(lidarData);
    }

    m_logger.LogInfo() << "SensorFusion::OnReceiveLidarEvent: Lidar data buffered.";
}

void SensorFusion::HandleSensorData() {
    while (m_running) {
        std::shared_ptr<deepracer::serviceinterfacecam::StereoCamera> cameraData;

        while (!m_latestCameraData) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }

        cameraData = m_latestCameraData;
        m_latestCameraData.reset();

        auto lidarData = FindClosestLidarData(cameraData->timestamp);

        if (!lidarData) {
            m_logger.LogWarn() << "HandleSensorData: No suitable lidar data found.";
            continue;
        }

        deepracer::serviceinterfacefusion::SensorFusion fusionData;
        fusionData.data.StereoCamera = *cameraData;
        fusionData.data.SectorLidar = lidarData->data;
        fusionData.timestamp = cameraData->timestamp;

        m_PPortSensorFusion->WriteDataSFEvent(fusionData);
        m_PPortSensorFusion->SendEventSFEventTriggered();
        m_logger.LogInfo() << "HandleSensorData: Sensor fusion data sent.";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

std::shared_ptr<deepracer::serviceinterfacelidar::SectorLidar> SensorFusion::FindClosestLidarData(uint64_t cameraTimestamp)
{
    std::lock_guard<std::mutex> lock(lidarMutex);
    std::shared_ptr<deepracer::serviceinterfacelidar::SectorLidar> closestLidarData = nullptr;

    for (const auto& lidarData : lidarBuffer) {
        if (lidarData->timestamp <= cameraTimestamp) {
            closestLidarData = lidarData;
        } else {
            break;
        }
    }
    return closestLidarData;
}

// ara::core::Result<deepracer::serviceinterfacelidar::SectorLidar> SensorFusion::FindClosestLidarData(uint64_t cameraTimestamp)
// {
//     std::lock_guard<std::mutex> lock(lidarMutex);
//     std::shared_ptr<deepracer::serviceinterfacelidar::SectorLidar> closestLidarData = nullptr;

//     for (const auto& lidarData : lidarBuffer) {
//         if (lidarData->timestamp <= cameraTimestamp) {
//             closestLidarData = lidarData;
//         } else {
//             break;
//         }
//     }
//     if (closestLidarData) {
//         return *closestLidarData;
//     } else {
//         return ara::core::Result<deepracer::serviceinterfacelidar::SectorLidar>::FromError();
//     }
// }

} /// namespace aa
} /// namespace sensorfusion
