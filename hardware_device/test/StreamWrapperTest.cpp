
/*
 * INTEL CONFIDENTIAL
 * Copyright (c) 2014-2015 Intel
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

#include <StreamWrapper.hpp>
#include <StreamMock.hpp>
#include <gtest/gtest.h>

namespace intel_audio
{

class StreamWrapperTest : public ::testing::Test
{
private:
    virtual void SetUp()
    {
        mCInStream = InputStreamWrapper::bind(&mInMock);
        EXPECT_TRUE(NULL != mCInStream);

        mCOutStream = OutputStreamWrapper::bind(&mOutMock);
        EXPECT_TRUE(NULL != mCOutStream);
    }
    virtual void TearDown()
    {
        // Check I get back the stream I gave at creation
        EXPECT_EQ(&mInMock, InputStreamWrapper::release(mCInStream));
        EXPECT_EQ(&mOutMock, OutputStreamWrapper::release(mCOutStream));
    }

protected:
    StreamInMock mInMock;
    audio_stream_in_t *mCInStream;

    StreamOutMock mOutMock;
    audio_stream_out_t *mCOutStream;
};

TEST_F(StreamWrapperTest, CreateDelete)
{
}

TEST_F(StreamWrapperTest, OutputWrapper)
{
    audio_stream_t *stream = &mCOutStream->common;
    EXPECT_EQ(stream->get_sample_rate(stream), static_cast<uint32_t>(1234));

    EXPECT_EQ(stream->set_sample_rate(stream, 48000), 0);

    EXPECT_EQ(stream->get_buffer_size(stream), 54321u);

    EXPECT_EQ(stream->get_channels(stream), static_cast<audio_channel_mask_t>(7));

    EXPECT_EQ(stream->get_format(stream), AUDIO_FORMAT_PCM_32_BIT);

    EXPECT_EQ(stream->set_format(stream, AUDIO_FORMAT_AAC), 0);

    EXPECT_EQ(stream->standby(stream), 0);

    EXPECT_EQ(stream->dump(stream, 453), 0);

    EXPECT_EQ(stream->get_device(stream), AUDIO_DEVICE_OUT_HDMI);

    EXPECT_EQ(stream->set_device(stream, AUDIO_DEVICE_IN_AMBIENT), 0);

    std::string kvpairs("woannnagain bistoufly");
    EXPECT_EQ(stream->set_parameters(stream, kvpairs.c_str()), 0);
}

TEST_F(StreamWrapperTest, InputWrapper)
{
    audio_stream_t *stream = &mCInStream->common;
    EXPECT_EQ(stream->get_sample_rate(stream), static_cast<uint32_t>(1234));

    EXPECT_EQ(stream->set_sample_rate(stream, 48000), 0);

    EXPECT_EQ(stream->get_buffer_size(stream), 11155u);

    EXPECT_EQ(stream->get_channels(stream), static_cast<audio_channel_mask_t>(5));

    EXPECT_EQ(stream->get_format(stream), AUDIO_FORMAT_PCM_16_BIT);

    EXPECT_EQ(stream->set_format(stream, AUDIO_FORMAT_AAC), 0);

    EXPECT_EQ(stream->standby(stream), 0);

    EXPECT_EQ(stream->dump(stream, 453), 0);

    EXPECT_EQ(stream->get_device(stream), AUDIO_DEVICE_OUT_AUX_DIGITAL);

    EXPECT_EQ(stream->set_device(stream, AUDIO_DEVICE_IN_AMBIENT), 0);

    std::string kvpairs("woannnagain bistoufly");
    EXPECT_EQ(stream->set_parameters(stream, kvpairs.c_str()), 0);

    std::string keys("woannnagain");
    std::string values("Input");
    char *read_values = stream->get_parameters(stream, keys.c_str());
    EXPECT_STREQ(values.c_str(), read_values);
    free(read_values);

    effect_handle_t effect = reinterpret_cast<effect_handle_t>(alloca(sizeof(effect)));
    EXPECT_EQ(stream->add_audio_effect(stream, effect), 0);

    EXPECT_EQ(stream->remove_audio_effect(stream, effect), 0);

    // Specific API check
    EXPECT_EQ(mCInStream->set_gain(mCInStream, -3.1), 0);

    char buffer[1024];
    // Read successes
    EXPECT_EQ(1024, mCInStream->read(mCInStream, buffer, 1024));

    EXPECT_EQ(mCInStream->get_input_frames_lost(mCInStream), static_cast<uint32_t>(15));
}

}
