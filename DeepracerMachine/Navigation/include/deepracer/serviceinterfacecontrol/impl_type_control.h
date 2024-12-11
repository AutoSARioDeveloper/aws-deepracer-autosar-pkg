///////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                        
/// Copyright, 2021 PopcornSAR Co., Ltd. All rights reserved.                                              
/// This software is copyright protected and proprietary to PopcornSAR Co., Ltd.                           
/// PopcornSAR Co., Ltd. grants to you only those rights as set out in the license conditions.             
///                                                                                                        
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// AUTOSAR VERSION                   : R20-11
/// GENERATED BY                      : PARA, ARA::COM Generator
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// GENERATED FILE NAME               : impl_type_control.h
/// IMPLEMENTATION DATA TYPE NAME     : Control
/// GENERATED DATE                    : 2024-12-03 10:27:53
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                        
/// CAUTION!! AUTOMATICALLY GENERATED FILE - DO NOT EDIT                                                   
///                                                                                                        
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef PARA_ARA_COM_GEN_CPP_IMPL_TYPE_CONTROL_H
#define PARA_ARA_COM_GEN_CPP_IMPL_TYPE_CONTROL_H
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// INCLUSION HEADER FILES
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @uptrace{SWS_CM_00402}
#include <ara/core/structure.h>
#include "deepracer/controldata/impl_type_controldata.h"
#include "deepracer/timestamp/impl_type_timestamp.h"
/// @uptrace{SWS_CM_10375}
namespace deepracer
{
namespace serviceinterfacecontrol
{
struct Control
{
    deepracer::controldata::ControlData data;
    deepracer::timestamp::Timestamp timestamp;
    PARA_STRUCTURE(Control, data, timestamp);
};
} /// namespace serviceinterfacecontrol
} /// namespace deepracer
#endif /// PARA_ARA_COM_GEN_CPP_IMPL_TYPE_CONTROL_H