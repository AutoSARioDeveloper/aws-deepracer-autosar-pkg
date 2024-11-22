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
/// GENERATED FILE NAME               : pportcamera.cpp
/// SOFTWARE COMPONENT NAME           : PPortCamera
/// GENERATED DATE                    : 2024-11-22 14:30:35
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "camera/aa/port/pportcamera.h"
 
namespace deepracer
{
namespace service
{
namespace camera
{
namespace skeleton
{
 
SvCameraSkeletonImpl::SvCameraSkeletonImpl(ara::core::InstanceSpecifier instanceSpec, ara::com::MethodCallProcessingMode mode)
    : SvCameraSkeleton(instanceSpec, mode)
    , m_logger(ara::log::CreateLogger("CAM", "PORT", ara::log::LogLevel::kVerbose))
{
}
 
} /// namespace skeleton
} /// namespace camera
} /// namespace service
} /// namespace deepracer

namespace camera
{
namespace aa
{
namespace port
{
 
PPortCamera::PPortCamera()
    : m_logger(ara::log::CreateLogger("CAM", "PORT", ara::log::LogLevel::kVerbose))
    , m_running{false}
    , m_CamEventData{0U}
{
}
 
PPortCamera::~PPortCamera()
{
}
 
void PPortCamera::Start()
{
    m_logger.LogVerbose() << "PPortCamera::Start";
    
    // construct skeleton
    ara::core::InstanceSpecifier specifier{"Camera/AA/PPortCamera"};
    m_interface = std::make_unique<deepracer::service::camera::skeleton::SvCameraSkeletonImpl>(specifier);
    
    // offer service
    auto offer = m_interface->OfferService();
    if (offer.HasValue())
    {
        m_running = true;
        m_logger.LogVerbose() << "PPortCamera::Start::OfferService";
    }
    else
    {
        m_running = false;
        m_logger.LogError() << "PPortCamera::Start::OfferService::" << offer.Error().Message();
    }
}
 
void PPortCamera::Terminate()
{
    m_logger.LogVerbose() << "PPortCamera::Terminate";
    
    // stop port
    m_running = false;
    
    // stop offer service
    m_interface->StopOfferService();
    m_logger.LogVerbose() << "PPortCamera::Terminate::StopOfferService";
}
 
void PPortCamera::WriteDataCamEvent(const deepracer::service::camera::skeleton::events::CamEvent::SampleType& data)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    
    m_CamEventData = data;
}
 
void PPortCamera::SendEventCamEventCyclic()
{
    while (m_running)
    {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            
            auto send = m_interface->CamEvent.Send(m_CamEventData);
            if (send.HasValue())
            {
                m_logger.LogVerbose() << "PPortCamera::SendEventCamEventCyclic::Send";
            }
            else
            {
                m_logger.LogError() << "PPortCamera::SendEventCamEventCyclic::Send::" << send.Error().Message();
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
 
void PPortCamera::SendEventCamEventTriggered()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    
    auto send = m_interface->CamEvent.Send(m_CamEventData);
    if (send.HasValue())
    {
        m_logger.LogVerbose() << "PPortCamera::SendEventCamEventTriggered::Send";
    }
    else
    {
        m_logger.LogError() << "PPortCamera::SendEventCamEventTriggered::Send::" << send.Error().Message();
    }
}
 
void PPortCamera::SendEventCamEventTriggered(const deepracer::service::camera::skeleton::events::CamEvent::SampleType& data)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    
    m_CamEventData = data;
    auto send = m_interface->CamEvent.Send(m_CamEventData);
    if (send.HasValue())
    {
        m_logger.LogVerbose() << "PPortCamera::SendEventCamEventTriggered::Send";
    }
    else
    {
        m_logger.LogError() << "PPortCamera::SendEventCamEventTriggered::Send::" << send.Error().Message();
    }
}
 
} /// namespace port
} /// namespace aa
} /// namespace camera
 
/// EOF