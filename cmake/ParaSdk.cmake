# ============================================================================
# GENERATED BY PARA, PopcornSAR
# Copyright(c) 2024 by PopcornSAR Co.,Ltd. All rights reserved.
# ============================================================================
 
macro(ParaSdk_Init)
    if (NOT DEFINED PARA_SDK OR PARA_SDK STREQUAL "")
        message(FATAL_ERROR "[PARA] ERROR: The variable 'PARA_SDK' is not set")
    elseif (NOT EXISTS ${PARA_SDK})
        message(FATAL_ERROR "[PARA] ERROR: The variable 'PARA_SDK' has invalid path: ${PARA_SDK}")
    endif()
    
    list(APPEND CMAKE_MODULE_PATH ${PARA_SDK}/share/cmake/Modules)
    include(PARA/Para)
endmacro()
 
ParaSdk_Init()