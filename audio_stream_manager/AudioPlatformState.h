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

#include "AudioBand.h"
#include <NonCopyable.hpp>
#include <Direction.hpp>
#include <list>
#include <map>
#include <string>
#include <utils/Errors.h>
#include <utils/String8.h>
#include <vector>

class CParameterMgrPlatformConnector;
class Criterion;
class CriterionType;
class ParameterCriterion;
class cnode;

namespace android_audio_legacy
{

class ParameterMgrPlatformConnectorLogger;
class AudioStream;



class AudioPlatformState : public audio_comms::utilities::NonCopyable
{
private:
    class CheckAndSetParameter;
    class SetParamToCriterionPair;

    typedef std::pair<const char *, const char *> ParamToCriterionValuePair;
    typedef std::pair<int, const char *> CriterionTypeValuePair;
    typedef std::vector<ParameterCriterion *>::iterator CriteriaVectorIterator;

    typedef std::map<std::string, Criterion *>::iterator CriterionMapIterator;
    typedef std::map<std::string, Criterion *>::const_iterator CriterionMapConstIterator;
    typedef std::map<std::string, CriterionType *>::iterator CriterionTypeMapIterator;
    typedef std::map<std::string, CriterionType *>::const_iterator CriteriaTypeMapConstIterator;

    typedef std::list<AudioStream *>::iterator StreamListIterator;
    typedef std::list<const AudioStream *>::const_iterator StreamListConstIterator;
public:
    AudioPlatformState();
    virtual ~AudioPlatformState();

    /**
     * Apply the configuration of the platform on the route parameter manager.
     * Once all the criteria have been set, the client of the platform state must call
     * this function in order to have the route PFW taking into account these criteria.
     */
    void applyPlatformConfiguration();

    /**
     * Generic setParameter handler.
     * It can for example:
     *      -Set the TTY mode.
     * (Direction of the TTY is a bitfield with Downlink and Uplink fields.)
     *      - Set the HAC mode.
     *      - Set the BT headset NREC. (BT device embeds its acoustic algorithms).
     *      - Set the BT headset negociated Band Type.
     * (Band Type results of the negociation between device and the BT HFP headset.)
     *      - Set the BT Enabled flag.
     *      - Set the context awareness status.
     *      - Set the FM state.
     *      - Set the screen state.
     *
     */
    android::status_t setParameters(const android::String8 &keyValuePairs);

    /**
     * Checks if the platform state was correctly started (ie the route parameter manager
     * has been instantiated and started correctly).
     *
     * @return true if platform state is started correctly, false otherwise.
     */
    bool isStarted();

    /**
     * Set the modem status.
     *
     * @param[in] isAlive true if modem is UP, false otherwise (DOWN or RESET).
     */
    void setModemAlive(bool isAlive)
    {
        setValue(isAlive, MODEM_STATE);
    }
    /**
     * Get the modem status.
     *
     * @return true if modem is UP, false otherwise (DOWN or RESET).
     */
    bool isModemAlive() const
    {
        return getValue(MODEM_STATE);
    }

    /**
     * Set the modem audio call status.
     *
     * @param[in] isAudioAvailable true if modem is ready for enabling audio link.
     */
    void setModemAudioAvailable(bool isAudioAvailable)
    {
        setValue(isAudioAvailable, MODEM_AUDIO_STATUS);
    }
    /**
     * Get the modem audio call status.
     *
     * @return true if modem is ready for enabling audio link.
     */
    bool isModemAudioAvailable() const
    {
        return getValue(MODEM_AUDIO_STATUS);
    }

    /**
     * Set the modem embedded status.
     *
     * @param[in] isPresent true if platform embeds a modem, false otherwise.
     */
    void setModemEmbedded(bool isPresent)
    {
        setValue(isPresent, HAS_MODEM);
    }

    /**
     * Get the modem embedded status.
     *
     * @return true if platform embeds a modem, false otherwise.
     */
    bool isModemEmbedded() const
    {
        return getValue(HAS_MODEM);
    }

    /**
     * Set the android telephony mode.
     * Mode are defined by AudioSystem.
     *
     * @param[in] mode android mode selected by the policy.
     */
    void setMode(int mode)
    {
        setValue(mode, ANDROID_MODE);
    }

    /**
     * Get the android telephony mode.
     * Mode are defined by AudioSystem.
     *
     * @return android mode selected by the policy.
     */
    int getMode() const
    {
        return getValue(ANDROID_MODE);
    }

    /**
     * Set the devices.
     *
     * @param[in] devices: devices enabled mask
     * @param[in] isOut: true for output devices, false for input devices.
     */
    void setDevices(uint32_t devices, bool isOut)
    {
        setValue(devices, isOut ? OUTPUT_DEVICE : INPUT_DEVICE);
    }
    /**
     * Get the devices.
     *
     * @param[in] isOut: true for output devices, false for input devices.
     * @return devices enabled mask.
     */
    uint32_t getDevices(bool isOut) const
    {
        return getValue(isOut ? OUTPUT_DEVICE : INPUT_DEVICE);
    }

    /**
     * Set the CSV Band Type.
     * Voice Call Band Type is given by the modem itself.
     *
     * @param[in] bandType: the band type to be used for Voice Call.
     */
    void setCsvBandType(CAudioBand::Type bandType)
    {
        setValue(bandType, CSV_BAND);
    }
    /**
     * Get the CSV Band Type.
     * Voice Call Band Type is given by the modem itself.
     *
     * @return the band type to be used for Voice Call.
     */
    CAudioBand::Type getCsvBandType() const
    {
        return static_cast<CAudioBand::Type>(getValue(CSV_BAND));
    }

    /**
     * Set the VoIP Band Type.
     * VoIP band type is inferred by the rate of the input stream (which is a "direct" stream, ie
     * running at the same rate than the VoIP application).
     *
     * @param[in] bandType: the band type to be set for VoIP call.
     */
    void setVoIPBandType(CAudioBand::Type bandType)
    {
        setValue(bandType, VOIP_BAND);
    }
    /**
     * Get the VoIP Band Type.
     * VoIP band type is inferred by the rate of the input stream (which is a "direct" stream, ie
     * running at the same rate than the VoIP application).
     *
     * @return the band type to be set for VoIP call.
     */
    CAudioBand::Type getVoIPBandType() const
    {
        return static_cast<CAudioBand::Type>(getValue(VOIP_BAND));
    }

    /**
     * Update Input Sources.
     * It computes the input sources criteria as a mask of input source of all active input streams.
     */
    void updateActiveInputSources();
    /**
     * Get Input Sources.
     *
     * @return computed input sources mask ie input source of all active input streams.
     */
    uint32_t getInputSource() const
    {
        return getValue(INPUT_SOURCES);
    }

    /**
     * Update Output flags.
     * It computes the output flags criteria as a mask of output flags of all active output streams.
     */
    void updateActiveOutputFlags();
    /**
     * Get Output flags.
     * return computed output flags maks ie mask of output flags of all active output streams.
     */
    uint32_t getOutputFlags() const
    {
        return getValue(OUTPUT_FLAGS);
    }

    /**
     * Set the mic to muted/unmuted state.
     *
     * @param[in] muted: true to mute, false to unmute.
     */
    void setMicMute(bool muted)
    {
        setValue(muted, MIC_MUTE);
    }
    /**
     * Get the mic to muted/unmuted state.
     *
     * @return true if muted, false if unmuted.
     */
    bool isMicMuted() const
    {
        return getValue(MIC_MUTE);
    }

    /**
     * Informs that a stream is started.
     * It adds the stream to active stream list.
     * Platform states use this list to provide the outputflags/inputsource bitfield criteria
     * only when the stream using the flag/source is active.
     *
     * @param startedStream stream to be added to the list.
     */
    void startStream(const AudioStream *startedStream);

    /**
     * Informs that a stream is stopped.
     * It removes the stream to active stream list and update the outputflags or input sources.
     * criterion according to the direction of the stream.
     *
     * @param startedStream stream to be added to the list.
     */
    void stopStream(const AudioStream *stoppedStream);

    /**
     * Set the BT headset negociated Band Type.
     * Band Type results of the negociation between device and the BT HFP headset.
     *
     * @param[in] eBtHeadsetBandType: the band type to be set.
     */
    bool hasPlatformStateChanged(int iEvents = -1) const;
private:
    /**
     * Load the criterion configuration file.
     *
     * @param[in] path Criterion conf file path.
     *
     * @return OK is parsing successfull, error code otherwise.
     */
    android::status_t loadAudioCriterionConfig(const char *path);

    /**
     * Parse and load the inclusive criterion type from configuration file.
     *
     * @param[in] root node of the configuration file.
     */
    void loadInclusiveCriterionType(cnode *root);

    /**
     * Parse and load the exclusive criterion type from configuration file.
     *
     * @param[in] root node of the configuration file.
     */
    void loadExclusiveCriterionType(cnode *root);

    /**
     * Parse and load the criteria from configuration file.
     *
     * @param[in] root node of the configuration file.
     */
    void loadCriteria(cnode *root);

    /**
     * Parse and load a criterion from configuration file.
     *
     * @param[in] root node of the configuration file.
     */
    void loadCriterion(cnode *root);

    /**
     * Parse and load parameter criteria from configuration file.
     *
     * @param[in] root node of the configuration file.
     */
    void loadParameterCriteria(cnode *root);

    /**
     * Parse and load a parameter criterion from configuration file.
     *
     * @param[in] root node of the configuration file.
     */
    void loadParameterCriterion(cnode *root);

    /**
     * Parse and load the criterion types from configuration file.
     *
     * @param[in] root node of the configuration file
     * @param[in] isInclusive true if inclusive, false is exclusive.
     */
    void loadCriterionType(cnode *root, bool isInclusive);

    /**
     * Parse and load the mapping table of a criterion from configuration file.
     * A mapping table associates the Android Parameter values to the criterion values.
     *
     * @param[in] values string of the list of param value/criterion values coma separated to parse.
     *
     * @return vector of value pairs.
     */
    std::vector<ParamToCriterionValuePair> parseMappingTable(const char *values);

    /**
     * Retrieve an element from a map by its name.
     *
     * @tparam T type of element to search.
     * @param[in] name name of the element to find.
     * @param[in] elementsMap maps of elements to search into.
     *
     * @return valid pointer on element if found, NULL otherwise.
     */
    template <typename T>
    T *getElement(const std::string &name, std::map<std::string, T *> &elementsMap);

    /**
     * Retrieve an element from a map by its name. Const version.
     *
     * @tparam T type of element to search.
     * @param[in] name name of the element to find.
     * @param[in] elementsMap maps of elements to search into.
     *
     * @return valid pointer on element if found, NULL otherwise.
     */
    template <typename T>
    const T *getElement(const std::string &name,
                        const std::map<std::string, T *> &elementsMap) const;

    /**
     * set the value of a component state.
     *
     * @param[in] value new value to set to the component state.
     * @param[in] stateName of the component state.
     */
    void setValue(int value, const char *stateName);

    /**
     * get the value of a component state.
     *
     * @param[in] name of the component state.
     *
     * @return value of the component state
     */
    int getValue(const char *stateName) const;

    /**
     * Resets the platform state events.
     */
    void clearPlatformStateEvents();

    /**
     * Sets a platform state event.
     *
     * @param[in] eventStateName name of the event that happened.
     */
    void setPlatformStateEvent(const std::string &eventStateName);

    /**
     * Update the streams mask.
     * This function parses all active streams and concatenate their mask into a bit field.
     *
     * @param[in] isOut direction of streams.
     *
     * @return concatenated mask of active streams.
     */
    uint32_t updateStreamsMask(bool isOut);

    uint32_t _directStreamsRefCount; /**< reference counter of direct streams. */

    /**
     * Input/Output Streams list.
     */
    std::list<const AudioStream *>
    _activeStreamsList[audio_comms::utilities::Direction::_nbDirections];

    std::map<std::string, CriterionType *> _criterionTypeMap;
    std::map<std::string, Criterion *> _criterionMap;
    std::vector<ParameterCriterion *> _parameterCriteriaVector; /**< Map of parameter criteria. */

    CParameterMgrPlatformConnector *_routePfwConnector; /**< Route Parameter Manager connector. */
    ParameterMgrPlatformConnectorLogger *_routePfwConnectorLogger; /**< Route PFW logger. */

    /**
     * Defines the name of the Android property describing the name of the PFW configuration file.
     */
    static const char *const ROUTE_PFW_CONF_FILE_NAME_PROP_NAME;
    static const char *const ROUTE_PFW_DEFAULT_CONF_FILE_NAME; /**< default PFW conf file name. */

    static const char *const AUDIO_CRITERION_CONF_FILE_PATH; /**< Criterion conf file path. */
    static const char *const INCLUSIVE_CRITERION_TYPE_TAG; /**< tag for inclusive criterion. */
    static const char *const EXCLUSIVE_CRITERION_TYPE_TAG; /**< tag for exclusive criterion. */
    static const char *const CRITERION_TAG; /**< tag for criterion. */

    static const char *const OUTPUT_DEVICE; /**< Output device criterion name. */
    static const char *const INPUT_DEVICE; /**< Input device criterion name. */
    static const char *const INPUT_SOURCES; /**< Input sources criterion name. */
    static const char *const OUTPUT_FLAGS; /**< Output flags criterion name. */
    static const char *const MODEM_AUDIO_STATUS; /**< Modem audio status criterion name. */
    static const char *const ANDROID_MODE; /**< Android Mode criterion name. */
    static const char *const HAS_MODEM; /**< has modem criterion name. */
    static const char *const MODEM_STATE; /**< Modem State criterion name. */
    static const char *const STATE_CHANGED; /**< State Changed criterion name. */
    static const char *const CSV_BAND; /**< CSV Band criterion name. */
    static const char *const VOIP_BAND; /**< VoIP band criterion name. */
    static const char *const MIC_MUTE; /**< Mic Mute criterion name. */

    /**
     * provide a compile time error if no specialization is provided for a given type.
     *
     * @tparam T: type of the parameter manager element. Supported one are:
     *                      - Criterion
     *                      - CriterionType.
     */
    template <typename T>
    struct parameterManagerElementSupported;
};
}         // namespace android
