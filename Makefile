# SPDX-License-Identifier: GPL-2.0
# Copyright (c) Bao Project and Contributors. All rights reserved.

# Prepare enviroment for baremetal-runtime
NAME:=baremetal
ROOT_DIR:=$(realpath .)
BUILD_DIR:=$(ROOT_DIR)/build/$(PLATFORM)

# Setup baremetal-runtime build
include $(ROOT_DIR)/setup.mk

APP_SRC_DIR?=$(ROOT_DIR)/src
include $(APP_SRC_DIR)/sources.mk
C_SRC+=$(addprefix $(APP_SRC_DIR)/, $(src_c_srcs))

# Include the final baremetal-runtime build
include $(ROOT_DIR)/build.mk
