/**********************************************************************
 * Copyright (c) 2024 - 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <utility>

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include <rinvid/sound/sound.h>

namespace rinvid::sound
{

Sound::Sound(const std::string& file_path) : buffer_{std::make_unique<sf::SoundBuffer>()}
{
    if (!buffer_->loadFromFile(file_path))
    {
        throw AudioError{"Audio: Could not load sound file '" + file_path + "'"};
    }

    sound_ = std::make_unique<sf::Sound>(*buffer_);
}

Sound::Sound(const Sound& other)
    : buffer_{std::make_unique<sf::SoundBuffer>(other.get_sound().getBuffer())},
      sound_{std::make_unique<sf::Sound>(*buffer_)}
{
    sound_->setVolume(other.get_volume());
    sound_->setPitch(other.get_pitch());
    sound_->setLooping(other.is_looping());
}

Sound& Sound::operator=(const Sound& other)
{
    if (this != &other)
    {
        Sound copy{other};
        buffer_.swap(copy.buffer_);
        sound_.swap(copy.sound_);
    }

    return *this;
}

Sound::Sound(Sound&& other) noexcept = default;

Sound& Sound::operator=(Sound&& other) noexcept
{
    if (this != &other)
    {
        // The old playback must release its reference before its samples are replaced.
        sound_.reset();
        buffer_ = std::move(other.buffer_);
        sound_  = std::move(other.sound_);
    }

    return *this;
}

Sound::~Sound() = default;

sf::Sound& Sound::get_sound()
{
    if (!sound_)
    {
        throw AudioError{"Audio: Cannot use a moved-from sound"};
    }

    return *sound_;
}

const sf::Sound& Sound::get_sound() const
{
    if (!sound_)
    {
        throw AudioError{"Audio: Cannot use a moved-from sound"};
    }

    return *sound_;
}

void Sound::play()
{
    get_sound().play();
}

void Sound::pause()
{
    get_sound().pause();
}

void Sound::stop()
{
    get_sound().stop();
}

PlaybackStatus Sound::get_status() const
{
    switch (get_sound().getStatus())
    {
        case sf::Sound::Status::Stopped:
            return PlaybackStatus::Stopped;
        case sf::Sound::Status::Paused:
            return PlaybackStatus::Paused;
        case sf::Sound::Status::Playing:
            return PlaybackStatus::Playing;
    }

    throw AudioError{"Audio: Unknown playback status"};
}

double Sound::get_duration() const
{
    return static_cast<double>(get_sound().getBuffer().getDuration().asMicroseconds()) / 1000000.0;
}

double Sound::get_playing_offset() const
{
    return static_cast<double>(get_sound().getPlayingOffset().asMicroseconds()) / 1000000.0;
}

void Sound::set_playing_offset(double seconds)
{
    auto&        sound{get_sound()};
    const double duration{get_duration()};
    if (!std::isfinite(seconds) || seconds < 0.0 || seconds > duration)
    {
        throw AudioError{"Audio: Playback position must be finite and within the sound's duration"};
    }

    const auto offset{seconds == duration
                          ? sound.getBuffer().getDuration()
                          : sf::microseconds(static_cast<std::int64_t>(seconds * 1000000.0))};
    sound.setPlayingOffset(offset);
}

void Sound::set_looping(bool looping)
{
    get_sound().setLooping(looping);
}

bool Sound::is_looping() const
{
    return get_sound().isLooping();
}

void Sound::set_volume(float volume)
{
    if (!std::isfinite(volume))
    {
        throw AudioError{"Audio: Volume must be finite"};
    }

    get_sound().setVolume(std::clamp(volume, 0.0F, 100.0F));
}

float Sound::get_volume() const
{
    return get_sound().getVolume();
}

void Sound::set_pitch(float pitch)
{
    if (!std::isfinite(pitch) || pitch <= 0.0F)
    {
        throw AudioError{"Audio: Pitch must be finite and positive"};
    }

    get_sound().setPitch(pitch);
}

float Sound::get_pitch() const
{
    return get_sound().getPitch();
}

} // namespace rinvid::sound
