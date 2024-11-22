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
/// GENERATED FILE NAME               : rportinference.cpp
/// SOFTWARE COMPONENT NAME           : RPortInference
/// GENERATED DATE                    : 2024-11-22 14:30:35
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "navigation/aa/port/rportinference.h"
 
namespace navigation
{
namespace aa
{
namespace port
{
 
RPortInference::RPortInference()
    : m_logger(ara::log::CreateLogger("NAVI", "PORT", ara::log::LogLevel::kVerbose))
    , m_running{false}
    , m_found{false}
    , m_receiverInfEvent{nullptr}
{
}
 
RPortInference::~RPortInference()
{
}
 
void RPortInference::Start()
{
    m_logger.LogVerbose() << "RPortInference::Start";
    
    // regist callback
    ara::core::InstanceSpecifier specifier{"Navigation/AA/RPortInference"};
    auto handler = [this](ara::com::ServiceHandleContainer<deepracer::service::inference::proxy::SvInfereceProxy::HandleType> handles,
                          ara::com::FindServiceHandle findHandle) {
        this->Find(handles, findHandle);
    };
    
    // find service
    auto find = deepracer::service::inference::proxy::SvInfereceProxy::StartFindService(handler, specifier);
    if (find.HasValue())
    {
        m_logger.LogVerbose() << "RPortInference::Start::StartFindService";
    }
    else
    {
        m_logger.LogError() << "RPortInference::Start::StartFindService::" << find.Error().Message();
    }
    
    // run port
    m_running = true;
}
 
void RPortInference::Terminate()
{
    m_logger.LogVerbose() << "RPortInference::Terminate";
    
    // stop port
    m_running = false;
    
    // clear service proxy
    if (m_interface)
    {
        // stop subscribe
        StopSubscribeInfEvent();
        
        // stop find service
        m_interface->StopFindService(*m_findHandle);
        m_found = false;
        
        m_logger.LogVerbose() << "RPortInference::Terminate::StopFindService";
    }
}
 
void RPortInference::Find(ara::com::ServiceHandleContainer<deepracer::service::inference::proxy::SvInfereceProxy::HandleType> handles, ara::com::FindServiceHandle findHandle)
{
    // check finding handles
    if (handles.empty())
    {
        m_logger.LogVerbose() << "RPortInference::Find::Service Instances not found";
        return;
    }
    else
    {
        for (auto& handle : handles)
        {
            m_logger.LogVerbose() << "RPortInference::Find::Searched Instance::ServiceId =" << 
                                     handle.GetServiceHandle().serviceId << 
                                     ", InstanceId =" << 
                                     handle.GetServiceHandle().instanceId;
        }
    }
    
    // create proxy
    if (m_interface)
    {
        m_logger.LogVerbose() << "RPortInference::Find::Proxy is already running";
    }
    else
    {
        m_logger.LogVerbose() << "RPortInference::Find::Using Instance::ServiceId =" << 
                                 handles[0].GetServiceHandle().serviceId << 
                                 ", InstanceId =" << 
                                 handles[0].GetServiceHandle().instanceId;
        m_interface = std::make_unique<deepracer::service::inference::proxy::SvInfereceProxy>(handles[0]);
        m_findHandle = std::make_unique<ara::com::FindServiceHandle>(findHandle);
        m_found = true;
        
        // subscribe events
        SubscribeInfEvent();
    }
}
 
void RPortInference::SubscribeInfEvent()
{
    if (m_found)
    {
        // regist receiver handler
        // if you want to enable it, please uncomment below code
        // RegistReceiverInfEvent();
        
        // request subscribe
        auto subscribe = m_interface->InfEvent.Subscribe(1);
        if (subscribe.HasValue())
        {
            m_logger.LogVerbose() << "RPortInference::SubscribeInfEvent::Subscribed";
        }
        else
        {
            m_logger.LogError() << "RPortInference::SubscribeInfEvent::" << subscribe.Error().Message();
        }
    }
}
 
void RPortInference::StopSubscribeInfEvent()
{
    if (m_found)
    {
        // request stop subscribe
        m_interface->InfEvent.Unsubscribe();
        m_logger.LogVerbose() << "RPortInference::StopSubscribeInfEvent::Unsubscribed";
    }
}
 
void RPortInference::RegistReceiverInfEvent()
{
    if (m_found)
    {
        // set callback
        auto receiver = [this]() -> void {
            return ReceiveEventInfEventTriggered();
        };
        
        // regist callback
        auto callback = m_interface->InfEvent.SetReceiveHandler(receiver);
        if (callback.HasValue())
        {
            m_logger.LogVerbose() << "RPortInference::RegistReceiverInfEvent::SetReceiveHandler";
        }
        else
        {
            m_logger.LogError() << "RPortInference::RegistReceiverInfEvent::SetReceiveHandler::" << callback.Error().Message();
        }
    }
}
 
void RPortInference::ReceiveEventInfEventTriggered()
{
    if (m_found)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        
        if (m_interface->InfEvent.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
        {
            auto recv = std::make_unique<ara::core::Result<size_t>>(m_interface->InfEvent.GetNewSamples([&](auto samplePtr) {
                RPortInference::ReadDataInfEvent(std::move(samplePtr));
            }));
            if (recv->HasValue())
            {
                m_logger.LogVerbose() << "RPortInference::ReceiveEventInfEvent::GetNewSamples::" << recv->Value();
            }
            else
            {
                m_logger.LogError() << "RPortInference::ReceiveEventInfEvent::GetNewSamples::" << recv->Error().Message();
            }
        }
    }
}
 
void RPortInference::ReceiveEventInfEventCyclic()
{
    while (m_running)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        
        if (m_found)
        {
            if (m_interface->InfEvent.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
            {
                auto recv = std::make_unique<ara::core::Result<size_t>>(m_interface->InfEvent.GetNewSamples([&](auto samplePtr) {
                    RPortInference::ReadDataInfEvent(std::move(samplePtr));
                }));
                if (recv->HasValue())
                {
                    m_logger.LogVerbose() << "RPortInference::ReceiveEventInfEvent::GetNewSamples::" << recv->Value();
                }
                else
                {
                    m_logger.LogError() << "RPortInference::ReceiveEventInfEvent::GetNewSamples::" << recv->Error().Message();
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
 
void RPortInference::ReadDataInfEvent(ara::com::SamplePtr<deepracer::service::inference::proxy::events::InfEvent::SampleType const> samplePtr)
{
    auto data = *samplePtr.Get();
    if (m_receiverInfEvent)
    {
        m_receiverInfEvent(data);
    }
}
 
void RPortInference::RegistEventHandlerInfEvent(std::function<void(const deepracer::service::inference::proxy::events::InfEvent::SampleType&)> handler)
{
    m_receiverInfEvent = handler;
}
 
} /// namespace port
} /// namespace aa
} /// namespace navigation
 
/// EOF