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
/// GENERATED FILE NAME               : impl_type_stereocamera.h
/// IMPLEMENTATION DATA TYPE NAME     : StereoCamera
/// GENERATED DATE                    : 2024-12-03 10:27:51
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                        
/// CAUTION!! AUTOMATICALLY GENERATED FILE - DO NOT EDIT                                                   
///                                                                                                        
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef PARA_ARA_COM_GEN_CPP_IMPL_TYPE_STEREOCAMERA_H
#define PARA_ARA_COM_GEN_CPP_IMPL_TYPE_STEREOCAMERA_H
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// INCLUSION HEADER FILES
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @uptrace{SWS_CM_00402}
#include <ara/core/structure.h>
#include "deepracer/image/impl_type_image.h"
#include "deepracer/timestamp/impl_type_timestamp.h"
/// @uptrace{SWS_CM_10375}
namespace deepracer
{
namespace serviceinterfacecam
{
struct StereoCamera
{
    deepracer::image::Image left;
    deepracer::image::Image right;
    deepracer::timestamp::Timestamp timestamp;
    PARA_STRUCTURE(StereoCamera, left, right, timestamp);
};
} /// namespace serviceinterfacecam
} /// namespace deepracer
#endif /// PARA_ARA_COM_GEN_CPP_IMPL_TYPE_STEREOCAMERA_H