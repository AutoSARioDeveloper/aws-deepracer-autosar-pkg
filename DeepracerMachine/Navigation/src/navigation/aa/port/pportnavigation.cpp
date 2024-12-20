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
/// GENERATED FILE NAME               : pportnavigation.cpp
/// SOFTWARE COMPONENT NAME           : PPortNavigation
/// GENERATED DATE                    : 2024-12-03 10:27:57
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "navigation/aa/port/pportnavigation.h"
 
namespace deepracer
{
namespace service
{
namespace navigation
{
namespace skeleton
{
 
SvNavigationSkeletonImpl::SvNavigationSkeletonImpl(ara::core::InstanceSpecifier instanceSpec, ara::com::MethodCallProcessingMode mode)
    : SvNavigationSkeleton(instanceSpec, mode)
    , m_logger(ara::log::CreateLogger("NAVI", "PORT", ara::log::LogLevel::kVerbose))
{
}
 
} /// namespace skeleton
} /// namespace navigation
} /// namespace service
} /// namespace deepracer

namespace navigation
{
namespace aa
{
namespace port
{
 
PPortNavigation::PPortNavigation()
    : m_logger(ara::log::CreateLogger("NAVI", "PORT", ara::log::LogLevel::kVerbose))
    , m_running{false}
    , m_NavEventData{{0.0f, 0.0f}, 0ULL}
{
}
 
PPortNavigation::~PPortNavigation()
{
}
 
void PPortNavigation::Start()
{
    m_logger.LogVerbose() << "PPortNavigation::Start";
    
    // construct skeleton
    ara::core::InstanceSpecifier specifier{"Navigation/AA/PPortNavigation"};
    m_interface = std::make_shared<deepracer::service::navigation::skeleton::SvNavigationSkeletonImpl>(specifier);
    
    // offer service
    auto offer = m_interface->OfferService();
    if (offer.HasValue())
    {
        m_running = true;
        m_logger.LogVerbose() << "PPortNavigation::Start::OfferService";
    }
    else
    {
        m_running = false;
        m_logger.LogError() << "PPortNavigation::Start::OfferService::" << offer.Error().Message();
    }
}
 
void PPortNavigation::Terminate()
{
    m_logger.LogVerbose() << "PPortNavigation::Terminate";
    
    // stop port
    m_running = false;
    
    // stop offer service
    m_interface->StopOfferService();
    m_logger.LogVerbose() << "PPortNavigation::Terminate::StopOfferService";
}
 
void PPortNavigation::WriteDataNavEvent(const deepracer::service::navigation::skeleton::events::NavEvent::SampleType& data)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_NavEventData = data;
}
 
void PPortNavigation::SendEventNavEventCyclic()
{
    while (m_running)
    {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            auto send = m_interface->NavEvent.Send(m_NavEventData);
            if (send.HasValue())
            {
                m_logger.LogVerbose() << "PPortNavigation::SendEventNavEventCyclic::Send";
            }
            else
            {
                m_logger.LogError() << "PPortNavigation::SendEventNavEventCyclic::Send::" << send.Error().Message();
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
 
void PPortNavigation::SendEventNavEventTriggered()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto send = m_interface->NavEvent.Send(m_NavEventData);
    if (send.HasValue())
    {
        m_logger.LogVerbose() << "PPortNavigation::SendEventNavEventTriggered::Send";
    }
    else
    {
        m_logger.LogError() << "PPortNavigation::SendEventNavEventTriggered::Send::" << send.Error().Message();
    }
}
 
void PPortNavigation::SendEventNavEventTriggered(const deepracer::service::navigation::skeleton::events::NavEvent::SampleType& data)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_NavEventData = data;
    auto send = m_interface->NavEvent.Send(m_NavEventData);
    if (send.HasValue())
    {
        m_logger.LogVerbose() << "PPortNavigation::SendEventNavEventTriggered::Send";
    }
    else
    {
        m_logger.LogError() << "PPortNavigation::SendEventNavEventTriggered::Send::" << send.Error().Message();
    }
}
 
} /// namespace port
} /// namespace aa
} /// namespace navigation
 
/// EOF