/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef INCLUDE_RINVID_SOUND_PLAYBACK_STATUS_H
#define INCLUDE_RINVID_SOUND_PLAYBACK_STATUS_H

#include <cstdint>

namespace rinvid::sound
{

/**************************************************************************************************
 * @brief Current state of audio playback.
 *
 *************************************************************************************************/
enum class PlaybackStatus : std::uint8_t
{
    Stopped, //!< Playback has not started, has finished, or was stopped.
    Paused,  //!< Playback is paused and can be resumed.
    Playing  //!< Playback is in progress.
};

} // namespace rinvid::sound

#endif // INCLUDE_RINVID_SOUND_PLAYBACK_STATUS_H
