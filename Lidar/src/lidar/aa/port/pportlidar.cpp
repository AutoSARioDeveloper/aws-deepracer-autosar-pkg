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
/// GENERATED FILE NAME               : pportlidar.cpp
/// SOFTWARE COMPONENT NAME           : PPortLidar
/// GENERATED DATE                    : 2024-11-22 14:30:35
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "lidar/aa/port/pportlidar.h"
 
namespace deepracer
{
namespace service
{
namespace lidar
{
namespace skeleton
{
 
SvLidarSkeletonImpl::SvLidarSkeletonImpl(ara::core::InstanceSpecifier instanceSpec, ara::com::MethodCallProcessingMode mode)
    : SvLidarSkeleton(instanceSpec, mode)
    , m_logger(ara::log::CreateLogger("LID", "PORT", ara::log::LogLevel::kVerbose))
{
}
 
} /// namespace skeleton
} /// namespace lidar
} /// namespace service
} /// namespace deepracer

namespace lidar
{
namespace aa
{
namespace port
{
 
PPortLidar::PPortLidar()
    : m_logger(ara::log::CreateLogger("LID", "PORT", ara::log::LogLevel::kVerbose))
    , m_running{false}
    , m_LidEventData{0U}
{
}
 
PPortLidar::~PPortLidar()
{
}
 
void PPortLidar::Start()
{
    m_logger.LogVerbose() << "PPortLidar::Start";
    
    // construct skeleton
    ara::core::InstanceSpecifier specifier{"Lidar/AA/PPortLidar"};
    m_interface = std::make_unique<deepracer::service::lidar::skeleton::SvLidarSkeletonImpl>(specifier);
    
    // offer service
    auto offer = m_interface->OfferService();
    if (offer.HasValue())
    {
        m_running = true;
        m_logger.LogVerbose() << "PPortLidar::Start::OfferService";
    }
    else
    {
        m_running = false;
        m_logger.LogError() << "PPortLidar::Start::OfferService::" << offer.Error().Message();
    }
}
 
void PPortLidar::Terminate()
{
    m_logger.LogVerbose() << "PPortLidar::Terminate";
    
    // stop port
    m_running = false;
    
    // stop offer service
    m_interface->StopOfferService();
    m_logger.LogVerbose() << "PPortLidar::Terminate::StopOfferService";
}
 
void PPortLidar::WriteDataLidEvent(const deepracer::service::lidar::skeleton::events::LidEvent::SampleType& data)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    
    m_LidEventData = data;
}
 
void PPortLidar::SendEventLidEventCyclic()
{
    while (m_running)
    {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            
            auto send = m_interface->LidEvent.Send(m_LidEventData);
            if (send.HasValue())
            {
                m_logger.LogVerbose() << "PPortLidar::SendEventLidEventCyclic::Send";
            }
            else
            {
                m_logger.LogError() << "PPortLidar::SendEventLidEventCyclic::Send::" << send.Error().Message();
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
 
void PPortLidar::SendEventLidEventTriggered()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    
    auto send = m_interface->LidEvent.Send(m_LidEventData);
    if (send.HasValue())
    {
        m_logger.LogVerbose() << "PPortLidar::SendEventLidEventTriggered::Send";
    }
    else
    {
        m_logger.LogError() << "PPortLidar::SendEventLidEventTriggered::Send::" << send.Error().Message();
    }
}
 
void PPortLidar::SendEventLidEventTriggered(const deepracer::service::lidar::skeleton::events::LidEvent::SampleType& data)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    
    m_LidEventData = data;
    auto send = m_interface->LidEvent.Send(m_LidEventData);
    if (send.HasValue())
    {
        m_logger.LogVerbose() << "PPortLidar::SendEventLidEventTriggered::Send";
    }
    else
    {
        m_logger.LogError() << "PPortLidar::SendEventLidEventTriggered::Send::" << send.Error().Message();
    }
}
 
} /// namespace port
} /// namespace aa
} /// namespace lidar
 
/// EOF