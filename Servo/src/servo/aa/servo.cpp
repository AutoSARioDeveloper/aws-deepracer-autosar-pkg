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
/// GENERATED FILE NAME               : servo.cpp
/// SOFTWARE COMPONENT NAME           : Servo
/// GENERATED DATE                    : 2024-11-22 14:30:35
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// INCLUSION HEADER FILES
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "servo/aa/servo.h"
 
namespace servo
{
namespace aa
{
 
Servo::Servo()
    : m_logger(ara::log::CreateLogger("SERV", "SWC", ara::log::LogLevel::kVerbose))
    , m_running{false}
    , m_workers(1)
{
}
 
Servo::~Servo()
{
}
 
bool Servo::Initialize()
{
    m_logger.LogVerbose() << "Servo::Initialize";
    
    bool init = true;
    
    m_RPortNavigation = std::make_unique<servo::aa::port::RPortNavigation>();
    
    // put your code for initialization
    
    return init;
}
 
void Servo::Start()
{
    m_logger.LogVerbose() << "Servo::Start";
    
    m_RPortNavigation->Start();
}
 
void Servo::Terminate()
{
    m_logger.LogVerbose() << "Servo::Terminate";
    
    // stop running
    m_running = false;
    
    m_RPortNavigation->Terminate();
}
 
void Servo::Run()
{
    m_logger.LogVerbose() << "Servo::Run";
    
    // start running
    m_running = true;
    
    m_workers.Async([this] { m_RPortNavigation->ReceiveEventNavEventCyclic(); });
    
    m_workers.Wait();
}
 
} /// namespace aa
} /// namespace servo