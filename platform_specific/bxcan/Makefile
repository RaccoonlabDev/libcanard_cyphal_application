# Copyright (c) 2022 Dmitry Ponomarev
# Distributed under the MIT License, available in the file LICENSE.
# Author: Dmitry Ponomarev <ponomarevda96@gmail.com>
BXCAN_IMPLEMENTATION_DIR := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))
PLATFORM_SPECIFIC_DIR := $(patsubst %/,%,$(dir $(BXCAN_IMPLEMENTATION_DIR)))
CYPHAL_DIR := $(patsubst %/,%,$(dir $(PLATFORM_SPECIFIC_DIR)))
FDCAN_LIB_DIR := $(CYPHAL_DIR)/Libs/platform_specific_components/stm32

CPP_SOURCES += $(wildcard ${BXCAN_IMPLEMENTATION_DIR}/*.cpp)
C_SOURCES += ${FDCAN_LIB_DIR}/libcanard/bxcan/src/bxcan.c
INC += -I${FDCAN_LIB_DIR}/libcanard/bxcan/src
