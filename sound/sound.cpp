/**********************************************************************
 * Copyright (c) 2024, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include "include/sound.h"

namespace rinvid::sound
{

Sound::Sound(std::string file_path)
{
    if (!buffer_.loadFromFile(file_path))
    {
        throw "Error loading sound from file!";
    }

    sound_.setBuffer(buffer_);
}

void Sound::play()
{
    sound_.play();
}

void Sound::pause()
{
    sound_.pause();
}

void Sound::stop()
{
    sound_.stop();
}

void Sound::set_looping(bool looping)
{
    sound_.setLoop(looping);
}

void Sound::set_volume(float volume)
{
    sound_.setVolume(volume);
}

} // namespace rinvid::sound
