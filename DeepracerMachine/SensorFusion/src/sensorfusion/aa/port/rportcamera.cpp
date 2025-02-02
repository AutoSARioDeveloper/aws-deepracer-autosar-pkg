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
/// GENERATED FILE NAME               : rportcamera.cpp
/// SOFTWARE COMPONENT NAME           : RPortCamera
/// GENERATED DATE                    : 2024-12-03 10:27:58
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "sensorfusion/aa/port/rportcamera.h"
 
namespace sensorfusion
{
namespace aa
{
namespace port
{
 
RPortCamera::RPortCamera()
    : m_logger(ara::log::CreateLogger("SEFU", "PORT", ara::log::LogLevel::kVerbose))
    , m_running{false}
    , m_found{false}
{
}
 
RPortCamera::~RPortCamera()
{
}
 
void RPortCamera::Start()
{
    m_logger.LogVerbose() << "RPortCamera::Start";
    
    // regist callback
    ara::core::InstanceSpecifier specifier{"SensorFusion/AA/RPortCamera"};
    auto handler = [this](ara::com::ServiceHandleContainer<deepracer::service::camera::proxy::SvCameraProxy::HandleType> handles,
                          ara::com::FindServiceHandle findHandle) {
        this->Find(handles, findHandle);
    };
    
    // find service
    auto find = deepracer::service::camera::proxy::SvCameraProxy::StartFindService(handler, specifier);
    if (find.HasValue())
    {
        m_logger.LogVerbose() << "RPortCamera::Start::StartFindService";
    }
    else
    {
        m_logger.LogError() << "RPortCamera::Start::StartFindService::" << find.Error().Message();
    }
    
    // run port
    m_running = true;
}
 
void RPortCamera::Terminate()
{
    m_logger.LogVerbose() << "RPortCamera::Terminate";
    
    // stop port
    m_running = false;
    
    // clear service proxy
    if (m_interface)
    {
        // stop subscribe
        StopSubscribeCamEvent();
        
        // stop find service
        m_interface->StopFindService(*m_findHandle);
        m_found = false;
        
        m_logger.LogVerbose() << "RPortCamera::Terminate::StopFindService";
    }
}
 
void RPortCamera::Find(ara::com::ServiceHandleContainer<deepracer::service::camera::proxy::SvCameraProxy::HandleType> handles, ara::com::FindServiceHandle findHandle)
{
    // check finding handles
    if (handles.empty())
    {
        m_logger.LogVerbose() << "RPortCamera::Find::Service Instances not found";
        return;
    }
    else
    {
        for (auto& handle : handles)
        {
            m_logger.LogVerbose() << "RPortCamera::Find::Searched Instance::ServiceId =" << 
                                     handle.GetServiceHandle().serviceId << 
                                     ", InstanceId =" << 
                                     handle.GetServiceHandle().instanceId;
        }
    }
    
    // create proxy
    if (m_interface)
    {
        m_logger.LogVerbose() << "RPortCamera::Find::Proxy is already running";
    }
    else
    {
        m_logger.LogVerbose() << "RPortCamera::Find::Using Instance::ServiceId =" << 
                                 handles[0].GetServiceHandle().serviceId << 
                                 ", InstanceId =" << 
                                 handles[0].GetServiceHandle().instanceId;
        m_interface = std::make_shared<deepracer::service::camera::proxy::SvCameraProxy>(handles[0]);
        m_findHandle = std::make_shared<ara::com::FindServiceHandle>(findHandle);
        m_found = true;
        
        // subscribe events
        SubscribeCamEvent();
    }
}
 
void RPortCamera::SubscribeCamEvent()
{
    if (m_found)
    {
        // regist receiver handler
        // if you want to enable it, please uncomment below code
        // 
        // RegistReceiverCamEvent();
        
        // request subscribe
        auto subscribe = m_interface->CamEvent.Subscribe(1);
        if (subscribe.HasValue())
        {
            m_logger.LogVerbose() << "RPortCamera::SubscribeCamEvent::Subscribed";
        }
        else
        {
            m_logger.LogError() << "RPortCamera::SubscribeCamEvent::" << subscribe.Error().Message();
        }
    }
}
 
void RPortCamera::StopSubscribeCamEvent()
{
    if (m_found)
    {
        // request stop subscribe
        m_interface->CamEvent.Unsubscribe();
        m_logger.LogVerbose() << "RPortCamera::StopSubscribeCamEvent::Unsubscribed";
    }
}
 
void RPortCamera::RegistReceiverCamEvent()
{
    if (m_found)
    {
        // set callback
        auto receiver = [this]() -> void {
            return ReceiveEventCamEventTriggered();
        };
        
        // regist callback
        auto callback = m_interface->CamEvent.SetReceiveHandler(receiver);
        if (callback.HasValue())
        {
            m_logger.LogVerbose() << "RPortCamera::RegistReceiverCamEvent::SetReceiveHandler";
        }
        else
        {
            m_logger.LogError() << "RPortCamera::RegistReceiverCamEvent::SetReceiveHandler::" << callback.Error().Message();
        }
    }
}
 
void RPortCamera::ReceiveEventCamEventTriggered()
{
    if (m_found)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_interface->CamEvent.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
        {
            auto recv = std::make_unique<ara::core::Result<size_t>>(m_interface->CamEvent.GetNewSamples([&](auto samplePtr) {
                RPortCamera::ReadDataCamEvent(std::move(samplePtr));
            }));
            if (recv->HasValue())
            {
                m_logger.LogVerbose() << "RPortCamera::ReceiveEventCamEvent::GetNewSamples::" << recv->Value();
            }
            else
            {
                m_logger.LogError() << "RPortCamera::ReceiveEventCamEvent::GetNewSamples::" << recv->Error().Message();
            }
        }
    }
}
 
void RPortCamera::ReceiveEventCamEventCyclic()
{
    while (m_running)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_found)
        {
            if (m_interface->CamEvent.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
            {
                auto recv = std::make_unique<ara::core::Result<size_t>>(m_interface->CamEvent.GetNewSamples([&](auto samplePtr) {
                    RPortCamera::ReadDataCamEvent(std::move(samplePtr));
                }));
                if (recv->HasValue())
                {
                    m_logger.LogVerbose() << "RPortCamera::ReceiveEventCamEvent::GetNewSamples::" << recv->Value();
                }
                else
                {
                    m_logger.LogError() << "RPortCamera::ReceiveEventCamEvent::GetNewSamples::" << recv->Error().Message();
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}
 
void RPortCamera::ReadDataCamEvent(ara::com::SamplePtr<deepracer::service::camera::proxy::events::CamEvent::SampleType const> samplePtr) 
{
    auto data = *samplePtr.Get();

    if (m_receiveEventCamEventHandler != nullptr)
    {
        m_receiveEventCamEventHandler(data);
    }
}

void RPortCamera::SetReceiveEventCamEventHandler(
    std::function<void(const deepracer::service::camera::proxy::events::CamEvent::SampleType&)> handler)
{
    m_receiveEventCamEventHandler = handler;
}


} /// namespace port
} /// namespace aa
} /// namespace sensorfusion
 
/// EOF