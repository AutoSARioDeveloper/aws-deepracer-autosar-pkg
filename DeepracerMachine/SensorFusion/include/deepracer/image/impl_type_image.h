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
/// GENERATED FILE NAME               : impl_type_image.h
/// IMPLEMENTATION DATA TYPE NAME     : Image
/// GENERATED DATE                    : 2024-12-03 10:27:54
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                        
/// CAUTION!! AUTOMATICALLY GENERATED FILE - DO NOT EDIT                                                   
///                                                                                                        
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef PARA_ARA_COM_GEN_CPP_IMPL_TYPE_IMAGE_H
#define PARA_ARA_COM_GEN_CPP_IMPL_TYPE_IMAGE_H
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// INCLUSION HEADER FILES
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @uptrace{SWS_CM_00402}
#include <cstdint>
#include <ara/core/structure.h>
#include "deepracer/imagedatalist/impl_type_imagedata.h"
/// @uptrace{SWS_CM_10375}
namespace deepracer
{
namespace image
{
struct Image
{
    std::uint32_t width;
    std::int32_t height;
    deepracer::imagedatalist::ImageData data;
    PARA_STRUCTURE(Image, width, height, data);
};
} /// namespace image
} /// namespace deepracer
#endif /// PARA_ARA_COM_GEN_CPP_IMPL_TYPE_IMAGE_H