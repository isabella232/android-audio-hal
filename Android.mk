#
#
# Copyright (C) Intel 2013-2017
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

ifeq ($(INTEL_AUDIO_HAL),new_hal)

LOCAL_PATH := $(call my-dir)
include $(OPTIONAL_QUALITY_ENV_SETUP)

define named-subdir-makefiles
$(wildcard $(addsuffix /Android.mk, $(addprefix $(LOCAL_PATH)/,$(1))))
endef

HAL_COMMON_CFLAGS :=

USE_ALSA_LIB := 1
ifeq ($(USE_ALSA_LIB), 1)
HAL_COMMON_CFLAGS := $(HAL_COMMON_CFLAGS) -DUSE_ALSA_LIB
endif

HAVE_BOOST := 0
ifeq ($(HAVE_BOOST), 1)
HAL_COMMON_CFLAGS := $(HAL_COMMON_CFLAGS) -DHAVE_BOOST
endif

HAL_COMMON_CFLAGS := $(HAL_COMMON_CFLAGS) \
    -Wall -Werror -Wextra -Wno-unused-parameter -Wno-unused-function

SUBDIRS := audio_conversion \
           audio_dump_lib \
           audio_policy \
           utilities/typeconverter \
           audio_route_manager \
           audio_stream_manager \
           effects \
           parameter_mgr_helper \
           sample_specifications \
           stream_lib \
           audio_platform_state \
           hardware_device \
           utilities/active_value_set \
           utilities/parameter \
           utilities \
           uevent_emulation

# Call sub-folders' Android.mk
include $(call named-subdir-makefiles, $(SUBDIRS))

include $(OPTIONAL_QUALITY_ENV_TEARDOWN)

endif # INTEL_AUDIO_HAL
