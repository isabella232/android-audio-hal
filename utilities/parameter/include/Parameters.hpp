/*
 * INTEL CONFIDENTIAL
 * Copyright (c) 2015 Intel
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
 * disclosed in any way without Intel's prior express written permission.
 *
 * No license under any patent, copyright, trade secret or other intellectual
 * property right is granted to or conferred upon you by disclosure or delivery
 * of the Materials, either expressly, by implication, inducement, estoppel or
 * otherwise. Any license under such intellectual property rights must be
 * express and approved by Intel in writing.
 */

#pragma once

#include <Direction.hpp>
#include <string>

namespace intel_audio
{

/**
 * Helper class to provide definition of parameter key recognized by Audio HAL.
 */
class Parameters
{
private:
    template <typename T>
    struct Key
    {
        typedef const T const_bidirectionalKey[Direction::gNbDirections];
    };

public:
    /**
     * Compress Offload routing key (e.g. for Compress Offload routing use case)
     * The values that the parameter with this keys can use are the output devices
     * enumeration found in @see audio_output_flags_t enum (system/audio.h)
     * If none, it follows the same routing than the policy: the use case shall be unrouted.
     */
    static const std::string &gKeyCompressOffloadRouting;

    /** Android Mode Parameter Key. */
    static const std::string &gKeyAndroidMode;

    /** Mic Mute Parameter Key. */
    static const std::string &gKeyMicMute;

    /** Output and Input Devices Parameter Key. */
    static Key<std::string>::const_bidirectionalKey gKeyDevices;

    /** Output and input flags key. */
    static Key<std::string>::const_bidirectionalKey gKeyFlags;

    /** Use case key, i.e. input source for input stream, unused for output. */
    static Key<std::string>::const_bidirectionalKey gKeyUseCases;

    /** VoIP Band Parameter Key. */
    static const std::string &gKeyVoipBandType;

    /** PreProc Parameter Key. */
    static const std::string &gKeyPreProcRequested;
};

}   // namespace intel_audio
