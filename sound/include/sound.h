/**********************************************************************
 * Copyright (c) 2024, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef SYSTEM_INCLUDE_SOUND_H
#define SYSTEM_INCLUDE_SOUND_H

#include <string>

#include <SFML/Audio.hpp>

namespace rinvid::sound
{

/**************************************************************************************************
 * @brief Sound that can be played. Supports only WAV for now. Basically a wrapper around
 * sf::Sound.
 *
 *************************************************************************************************/
class Sound
{
  public:
    /**************************************************************************************************
     * @brief Sound constructor.
     *
     * @param file_path Path to sound file to load.
     *
     *************************************************************************************************/
    Sound(std::string file_path);

    /**************************************************************************************************
     * @brief Plays or resumes (if it has been paused) sound.
     *
     *************************************************************************************************/
    void play();

    /**************************************************************************************************
     * @brief Pauses the sound if it is being played currently.
     *
     *************************************************************************************************/
    void pause();

    /**************************************************************************************************
     * @brief Stops and rewinds the sound if it is playing.
     *
     *************************************************************************************************/
    void stop();

    /**************************************************************************************************
     * @brief Controls whether the sound should be looping when played.
     *
     *************************************************************************************************/
    void set_looping(bool looping);

    /**************************************************************************************************
     * @brief Controls the volume in [0, 100] range. Default is 100.
     *
     * @param volume The volume of the sound.
     *
     *************************************************************************************************/
    void set_volume(float volume);

  private:
    sf::SoundBuffer buffer_;
    sf::Sound       sound_;
};

} // namespace rinvid::sound

#endif // SYSTEM_INCLUDE_SOUND_H
