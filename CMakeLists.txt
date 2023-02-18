cmake_minimum_required(VERSION 3.15.3)

project(cyphal_example)

FILE(GLOB libcyphalSources                  ${CMAKE_CURRENT_LIST_DIR}/libcyphal/*.cpp)
FILE(GLOB libcanardSources                  ${CMAKE_CURRENT_LIST_DIR}/Libs/libcanard/libcanard/*.c)
FILE(GLOB libparamsSources                  ${CMAKE_CURRENT_LIST_DIR}/Libs/libparams/libparams/*.c)
FILE(GLOB o1heapSources                     ${CMAKE_CURRENT_LIST_DIR}/Libs/o1heap/o1heap/*.c)

if(PLATFORM STREQUAL "bxcan")
    FILE(GLOB libcyphalPlatformSpecificSources  ${CMAKE_CURRENT_LIST_DIR}/platform_specific/bxcan/*.cpp)
    FILE(GLOB libparamsPlatformSpecificSources  ${CMAKE_CURRENT_LIST_DIR}/Libs/libparams/platform_specific/stm32f103/*.c)
    FILE(GLOB transportPlatformSpecificSources  ${CMAKE_CURRENT_LIST_DIR}/Libs/platform_specific_components/stm32/libcanard/bxcan/src/*.c)
    include_directories(${CMAKE_CURRENT_LIST_DIR}/platform_specific/bxcan)
    include_directories(${CMAKE_CURRENT_LIST_DIR}/Libs/libparams/platform_specific/stm32f103)
    include_directories(${CMAKE_CURRENT_LIST_DIR}/Libs/platform_specific_components/stm32/libcanard/bxcan/src)
elseif(PLATFORM STREQUAL "fdcan")
    FILE(GLOB libparamsPlatformSpecificSources  ${CMAKE_CURRENT_LIST_DIR}/Libs/libparams/platform_specific/stm32g0b1/*.c)
    FILE(GLOB transportPlatformSpecificSources  ${CMAKE_CURRENT_LIST_DIR}/Libs/platform_specific_components/stm32/libcanard/bxcan/src/*.c)
    include_directories(${CMAKE_CURRENT_LIST_DIR}/Libs/libparams/platform_specific/stm32g0b1)
    include_directories(${CMAKE_CURRENT_LIST_DIR}/Libs/platform_specific_components/stm32/libcanard/bxcan/src)
elseif(PLATFORM STREQUAL "socketcan")
    FILE(GLOB libcyphalPlatformSpecificSources  ${CMAKE_CURRENT_LIST_DIR}/platform_specific/socketcan/*.cpp)
    FILE(GLOB libparamsPlatformSpecificSources  ${CMAKE_CURRENT_LIST_DIR}/Libs/libparams/platform_specific/ubuntu/*.cpp)
    FILE(GLOB transportPlatformSpecificSources  ${CMAKE_CURRENT_LIST_DIR}/Libs/platform_specific_components/socketcan/libcanard/src/*.c)
    include_directories(${CMAKE_CURRENT_LIST_DIR}/platform_specific/socketcan)
    include_directories(${CMAKE_CURRENT_LIST_DIR}/Libs/libparams/platform_specific/ubuntu)
    include_directories(${CMAKE_CURRENT_LIST_DIR}/Libs/platform_specific_components/socketcan/libcanard/src)
else()
	message(SEND_ERROR "Platform is not specified!")
endif()

set(SRC
    ${libcyphalSources}
    ${libcanardSources}
    ${libparamsSources}
    ${o1heapSources}
    ${libcyphalPlatformSpecificSources}
    ${libparamsPlatformSpecificSources}
    ${transportPlatformSpecificSources}
)

include_directories(${CMAKE_CURRENT_LIST_DIR}/libcyphal)
include_directories(${CMAKE_CURRENT_LIST_DIR}/Libs/libcanard/libcanard)
include_directories(${CMAKE_CURRENT_LIST_DIR}/Libs/libparams/libparams)
include_directories(${CMAKE_CURRENT_LIST_DIR}/Libs/o1heap/o1heap)
include_directories(${CMAKE_CURRENT_LIST_DIR}/build/nunavut_out)