/*
 * INTEL CONFIDENTIAL
 * Copyright © 2013 Intel
 * Corporation All Rights Reserved.
 *
 * The source code contained or described herein and all documents related to
 * the source code ("Material") are owned by Intel Corporation or its suppliers
 * or licensors. Title to the Material remains with Intel Corporation or its
 * suppliers and licensors. The Material contains trade secrets and proprietary
 * and confidential information of Intel or its suppliers and licensors. The
 * Material is protected by worldwide copyright and trade secret laws and
 * treaty provisions. No part of the Material may be used, copied, reproduced,
 * modified, published, uploaded, posted, transmitted, distributed, or
 * disclosed in any way without Intel’s prior express written permission.
 *
 * No license under any patent, copyright, trade secret or other intellectual
 * property right is granted to or conferred upon you by disclosure or delivery
 * of the Materials, either expressly, by implication, inducement, estoppel or
 * otherwise. Any license under such intellectual property rights must be
 * express and approved by Intel in writing.
 *
 */
#pragma once

#include "AudioDevice.hpp"
#include <tinyalsa/asoundlib.h>

class StreamRouteConfig;

class TinyAlsaAudioDevice : public IAudioDevice
{
public:
    TinyAlsaAudioDevice()
        : _pcmDevice(NULL) {}

    /**
     * Get the pcm device handle.
     * Must only be called if isRouteAvailable returns true.
     * and any access to the device must be called with Lock held.
     *
     * @return pcm handle on alsa tiny device.
     */
    pcm *getPcmDevice();

    virtual android::status_t open(const char *cardName, uint32_t deviceId,
                                   const StreamRouteConfig &config, bool isOut);

    virtual bool isOpened();

    virtual android::status_t close();

private:
    /**
     * Take the power lock associated to the tiny alsa device.
     */
    void acquirePowerLock();

    /**
     * Release the power lock associated to the tiny alsa device.
     */
    void releasePowerLock();

    pcm *_pcmDevice; /**< Handle on tiny alsa PCM device. */

    static const char *const _powerLockTag; /**< Name of the power lock held. */
};
