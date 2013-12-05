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

#include "AudioEffectStub.hpp"
#include <hardware/audio_effect.h>
#include <NonCopyable.hpp>
#include <list>
#include <utils/Errors.h>

class AudioEffectStub;

class AudioEffectSessionStub : private audio_comms::utilities::NonCopyable
{
private:
    typedef std::list<AudioEffectStub *>::iterator EffectListIterator;
    typedef std::list<AudioEffectStub *>::const_iterator EffectListConstIterator;

public:
    AudioEffectSessionStub(uint32_t sessionId);

    /**
     * Add an effect. It does not attach the effect to the session. It helps the session
     * knowing what effects are available, and being able to serve request to create an effect
     * referred by its uuid and handle.
     *
     * @param[in] effect Audio Effect to add.
     *
     * @return OK is success, error code otherwise.
     */
    android::status_t addEffect(AudioEffectStub *effect);

    /**
     * Create an effect. It attaches the effect to the session
     *
     * @param[in] uuid unique identifier of the audio effect.
     * @param[in] interface handle of the audio effect.
     *
     * @return OK is success, error code otherwise.
     */
    android::status_t createEffect(const effect_uuid_t *uuid, effect_handle_t *interface);

    /**
     * Set the IO handle attached to the session.
     *
     * @param[in] effect Audio Effect to add.
     *
     * @return OK is success, error code otherwise.
     */
    android::status_t removeEffect(AudioEffectStub *effect);

    /**
     * Set the IO handle attached to the session.
     *
     * @param[in] ioHandle IO handle to attach to the session.
     */
    void setIoHandle(int ioHandle);

    /**
     * Get the IO handle attached to the session.
     *
     * @return IO handle.
     */
    int getIoHandle() const { return _ioHandle; }

    /**
     * Get the effect session Id.
     *
     * @return session id.
     */
    uint32_t getId() const { return _id; }

    static const int _sessionNone = -1; /**< No session tag. */

private:
    /**
     * Function to be used as the predicate in find_if call.
     */
    struct MatchUuid : public std::binary_function<AudioEffectStub *, const effect_uuid_t *, bool>
    {

        bool operator()(const AudioEffectStub *effect,
                        const effect_uuid_t *uuid) const
        {
            return memcmp(effect->getUuid(), uuid, sizeof(effect_uuid_t)) == 0;
        }
    };
    /**
     * Initialise the effect session.
     * It resets the io handle attached to it and the audio source.
     */
    void init();

    /**
     * Retrieve effect by its UUID
     *
     * @param[in] uuid unique identifier of the audio effect.
     *
     * @return valid effect pointer if found, NULL otherwise.
     */
    AudioEffectStub *findEffectByUuid(const effect_uuid_t *uuid);

    int _ioHandle; /**< handle of input stream this session is linked to. */
    uint32_t _id; /**< Id of the sessions.*/
    audio_source_t  _source; /**< Audio Source (ie Use Case) associated to this session. */

    std::list<AudioEffectStub *> _effectsCreatedList; /**< created pre processors. */
    std::list<AudioEffectStub *> _effectsActiveList; /**< active pre processors - for later use?. */
    std::list<AudioEffectStub *> _effectsList; /**< Effects in the sessions. */

};