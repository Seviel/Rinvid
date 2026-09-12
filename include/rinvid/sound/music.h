/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef INCLUDE_RINVID_SOUND_MUSIC_H
#define INCLUDE_RINVID_SOUND_MUSIC_H

#include <memory>
#include <string>

#include <rinvid/sound/playback_status.h>
#include <rinvid/util/error.h>

namespace sf
{
class Music;
}

namespace rinvid::sound
{

/**************************************************************************************************
 * @brief Streams one music track from a file, with looping enabled by default.
 * Owns the stream and stops playback on destruction. The file must remain accessible until
 * another track is opened or the music is destroyed. Playback needs no per-frame update.
 * Moves transfer live playback without restarting it. Moved-from music can be destroyed or
 * reassigned; other operations throw AudioError.
 *
 *************************************************************************************************/
class Music
{
  public:
    /**************************************************************************************************
     * @brief Opens a music file for streaming. Playback starts when play() is called.
     *
     * @param file_path Path of the music file.
     *
     * @throws AudioError if the file cannot be opened.
     *
     *************************************************************************************************/
    explicit Music(const std::string& file_path);

    /**************************************************************************************************
     * @brief Copy construction is disabled because music owns an active file stream.
     *
     *************************************************************************************************/
    Music(const Music& other) = delete;

    /**************************************************************************************************
     * @brief Copy assignment is disabled.
     *
     *************************************************************************************************/
    Music& operator=(const Music& other) = delete;

    /**************************************************************************************************
     * @brief Transfers the stream, playback position, and settings without interrupting playback.
     *
     * @param other Music to move from.
     *
     *************************************************************************************************/
    Music(Music&& other) noexcept;

    /**************************************************************************************************
     * @brief Stops this music and takes ownership of another music's uninterrupted playback.
     * Moving music onto itself has no effect.
     *
     * @param other Music to move from.
     *
     * @return This music.
     *
     *************************************************************************************************/
    Music& operator=(Music&& other) noexcept;

    /**************************************************************************************************
     * @brief Stops playback and closes the streamed file.
     *
     *************************************************************************************************/
    ~Music();

    /**************************************************************************************************
     * @brief Starts playback or resumes paused music. Restarts already playing music.
     *
     *************************************************************************************************/
    void play();

    /**************************************************************************************************
     * @brief Switches to another file and starts it from the beginning.
     * Preserves volume, pitch, and looping. The old track stops before the new one starts.
     * Opening the same file again restarts it. A failed open leaves the current track unchanged.
     *
     * @param file_path Path of the next music file.
     *
     * @throws AudioError if the file cannot be opened.
     *
     *************************************************************************************************/
    void play(const std::string& file_path);

    /**************************************************************************************************
     * @brief Pauses playing music, preserving its position.
     *
     *************************************************************************************************/
    void pause();

    /**************************************************************************************************
     * @brief Stops and rewinds the music, including when paused or already stopped.
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
     * @brief Returns the original track duration, unaffected by looping or pitch.
     *
     * @return Duration in seconds.
     *
     *************************************************************************************************/
    double get_duration() const;

    /**************************************************************************************************
     * @brief Returns the current position within the track.
     *
     * @return Playback position in seconds.
     *
     *************************************************************************************************/
    double get_playing_offset() const;

    /**************************************************************************************************
     * @brief Seeks within the track without changing playback state.
     * The backend rounds the position to an audio sample boundary.
     *
     * @param seconds Finite position in [0, get_duration()], in seconds.
     *
     * @throws AudioError if the position is non-finite or outside the track's duration.
     *
     *************************************************************************************************/
    void set_playing_offset(double seconds);

    /**************************************************************************************************
     * @brief Controls whether the entire track repeats.
     *
     * @param looping Whether to repeat. Default is true.
     *
     *************************************************************************************************/
    void set_looping(bool looping);

    /**************************************************************************************************
     * @brief Returns whether the track repeats.
     *
     * @return True if looping is enabled.
     *
     *************************************************************************************************/
    bool is_looping() const;

    /**************************************************************************************************
     * @brief Controls volume, clamping finite values to [0, 100]. Default is 100.
     *
     * @param volume The music volume.
     *
     * @throws AudioError if the volume is non-finite.
     *
     *************************************************************************************************/
    void set_volume(float volume);

    /**************************************************************************************************
     * @brief Returns the music volume.
     *
     * @return Volume in [0, 100].
     *
     *************************************************************************************************/
    float get_volume() const;

    /**************************************************************************************************
     * @brief Changes pitch and playback speed together. Default is 1.
     *
     * @param pitch Finite, positive pitch factor; 2 plays twice as fast and at a higher pitch.
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
    sf::Music&       get_music();
    const sf::Music& get_music() const;

    std::unique_ptr<sf::Music> music_;
};

} // namespace rinvid::sound

#endif // INCLUDE_RINVID_SOUND_MUSIC_H
