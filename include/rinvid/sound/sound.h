/**********************************************************************
 * Copyright (c) 2024 - 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef INCLUDE_RINVID_SOUND_SOUND_H
#define INCLUDE_RINVID_SOUND_SOUND_H

#include <cstdint>
#include <memory>
#include <string>

#include <rinvid/util/error.h>

namespace sf
{
class Sound;
class SoundBuffer;
} // namespace sf

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

/**************************************************************************************************
 * @brief A sound that owns its audio samples and playback.
 * Copies own independent samples and start stopped. Moves transfer live playback without
 * restarting it. Moved-from sounds can be destroyed or assigned another sound; all other
 * operations on them throw AudioError.
 *
 *************************************************************************************************/
class Sound
{
  public:
    /**************************************************************************************************
     * @brief Loads audio samples from a file and creates stopped playback.
     *
     * @param file_path Path of the audio file to load into memory.
     *
     * @throws AudioError if the file cannot be loaded.
     *
     *************************************************************************************************/
    explicit Sound(const std::string& file_path);

    /**************************************************************************************************
     * @brief Copies the audio samples, volume, pitch, and looping setting.
     * The copy starts stopped at position zero, independently of the original sound.
     *
     * @param other Sound to copy.
     *
     * @throws AudioError if the source has been moved from.
     *
     *************************************************************************************************/
    Sound(const Sound& other);

    /**************************************************************************************************
     * @brief Replaces this sound with an independent, stopped copy of another sound.
     * Copying a sound onto itself has no effect. A failed copy leaves this sound unchanged.
     *
     * @param other Sound to copy.
     *
     * @return This sound.
     *
     * @throws AudioError if the source has been moved from.
     *
     *************************************************************************************************/
    Sound& operator=(const Sound& other);

    /**************************************************************************************************
     * @brief Transfers playback and its settings without interrupting it.
     *
     * @param other Sound to move from.
     *
     *************************************************************************************************/
    Sound(Sound&& other) noexcept;

    /**************************************************************************************************
     * @brief Stops this sound and takes ownership of another sound's uninterrupted playback.
     * Moving a sound onto itself has no effect.
     *
     * @param other Sound to move from.
     *
     * @return This sound.
     *
     *************************************************************************************************/
    Sound& operator=(Sound&& other) noexcept;

    /**************************************************************************************************
     * @brief Stops and destroys playback before releasing the owned audio samples.
     *
     *************************************************************************************************/
    ~Sound();

    /**************************************************************************************************
     * @brief Starts playback or resumes a paused sound. Restarts an already playing sound.
     *
     *************************************************************************************************/
    void play();

    /**************************************************************************************************
     * @brief Pauses the sound if it is being played currently.
     *
     *************************************************************************************************/
    void pause();

    /**************************************************************************************************
     * @brief Stops and rewinds the sound, including when it is paused or already stopped.
     *
     *************************************************************************************************/
    void stop();

    /**************************************************************************************************
     * @brief Returns the current playback state.
     *
     * @return Stopped, paused, or playing.
     *
     *************************************************************************************************/
    PlaybackStatus get_status() const;

    /**************************************************************************************************
     * @brief Returns the duration of the audio at its original pitch.
     *
     * @return Duration in seconds, unaffected by looping or pitch changes.
     *
     *************************************************************************************************/
    double get_duration() const;

    /**************************************************************************************************
     * @brief Returns the current position within the audio.
     *
     * @return Playback position in seconds.
     *
     *************************************************************************************************/
    double get_playing_offset() const;

    /**************************************************************************************************
     * @brief Seeks within the audio without changing the playback state.
     * The backend rounds the position to an audio sample boundary.
     *
     * @param seconds Finite position in [0, get_duration()], in seconds.
     *
     * @throws AudioError if the position is non-finite or outside the audio's duration.
     *
     *************************************************************************************************/
    void set_playing_offset(double seconds);

    /**************************************************************************************************
     * @brief Controls whether the sound should be looping when played.
     *
     * @param looping Whether playback should repeat. Default is false.
     *
     *************************************************************************************************/
    void set_looping(bool looping);

    /**************************************************************************************************
     * @brief Returns whether playback should repeat.
     *
     * @return True if looping is enabled.
     *
     *************************************************************************************************/
    bool is_looping() const;

    /**************************************************************************************************
     * @brief Controls the volume, clamping finite values to [0, 100]. Default is 100.
     *
     * @param volume The volume of the sound.
     *
     * @throws AudioError if the volume is non-finite.
     *
     *************************************************************************************************/
    void set_volume(float volume);

    /**************************************************************************************************
     * @brief Returns the sound's volume.
     *
     * @return Volume in [0, 100].
     *
     *************************************************************************************************/
    float get_volume() const;

    /**************************************************************************************************
     * @brief Changes pitch and playback speed by the supplied factor. Default is 1.
     * A factor of 2 plays the audio twice as fast and at a higher pitch.
     *
     * @param pitch Finite, positive pitch factor.
     *
     * @throws AudioError if the pitch is non-finite or not positive.
     *
     *************************************************************************************************/
    void set_pitch(float pitch);

    /**************************************************************************************************
     * @brief Returns the pitch and playback speed factor.
     *
     * @return Positive pitch factor.
     *
     *************************************************************************************************/
    float get_pitch() const;

  private:
    sf::Sound&       get_sound();
    const sf::Sound& get_sound() const;

    // Stable sample storage lets moves preserve playback. Destroy sound_ before buffer_.
    std::unique_ptr<sf::SoundBuffer> buffer_;
    std::unique_ptr<sf::Sound>       sound_;
};

} // namespace rinvid::sound

#endif // INCLUDE_RINVID_SOUND_SOUND_H
