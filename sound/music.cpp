/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
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

#include <SFML/Audio/Music.hpp>

#include <rinvid/sound/music.h>

namespace rinvid::sound
{

Music::Music(const std::string& file_path) : music_{std::make_unique<sf::Music>()}
{
    if (!music_->openFromFile(file_path))
    {
        throw AudioError{"Audio: Could not open music file '" + file_path + "'"};
    }

    music_->setLooping(true);
}

Music::Music(Music&& other) noexcept = default;

Music& Music::operator=(Music&& other) noexcept
{
    if (this != &other)
    {
        music_ = std::move(other.music_);
    }

    return *this;
}

Music::~Music() = default;

sf::Music& Music::get_music()
{
    if (!music_)
    {
        throw AudioError{"Audio: Cannot use moved-from music"};
    }

    return *music_;
}

const sf::Music& Music::get_music() const
{
    if (!music_)
    {
        throw AudioError{"Audio: Cannot use moved-from music"};
    }

    return *music_;
}

void Music::play()
{
    get_music().play();
}

void Music::play(const std::string& file_path)
{
    const auto& current{get_music()};
    Music       next{file_path};
    next.music_->setVolume(current.getVolume());
    next.music_->setPitch(current.getPitch());
    next.music_->setLooping(current.isLooping());
    music_ = std::move(next.music_);
    music_->play();
}

void Music::pause()
{
    get_music().pause();
}

void Music::stop()
{
    get_music().stop();
}

PlaybackStatus Music::get_status() const
{
    switch (get_music().getStatus())
    {
        case sf::Music::Status::Stopped:
            return PlaybackStatus::Stopped;
        case sf::Music::Status::Paused:
            return PlaybackStatus::Paused;
        case sf::Music::Status::Playing:
            return PlaybackStatus::Playing;
    }

    throw AudioError{"Audio: Unknown playback status"};
}

double Music::get_duration() const
{
    return static_cast<double>(get_music().getDuration().asMicroseconds()) / 1000000.0;
}

double Music::get_playing_offset() const
{
    return static_cast<double>(get_music().getPlayingOffset().asMicroseconds()) / 1000000.0;
}

void Music::set_playing_offset(double seconds)
{
    auto&        music{get_music()};
    const double duration{get_duration()};
    if (!std::isfinite(seconds) || seconds < 0.0 || seconds > duration)
    {
        throw AudioError{"Audio: Playback position must be finite and within the music's duration"};
    }

    const auto offset{seconds == duration
                          ? music.getDuration()
                          : sf::microseconds(static_cast<std::int64_t>(seconds * 1000000.0))};
    music.setPlayingOffset(offset);
}

void Music::set_looping(bool looping)
{
    get_music().setLooping(looping);
}

bool Music::is_looping() const
{
    return get_music().isLooping();
}

void Music::set_volume(float volume)
{
    if (!std::isfinite(volume))
    {
        throw AudioError{"Audio: Volume must be finite"};
    }

    get_music().setVolume(std::clamp(volume, 0.0F, 100.0F));
}

float Music::get_volume() const
{
    return get_music().getVolume();
}

void Music::set_pitch(float pitch)
{
    if (!std::isfinite(pitch) || pitch <= 0.0F)
    {
        throw AudioError{"Audio: Pitch must be finite and positive"};
    }

    get_music().setPitch(pitch);
}

float Music::get_pitch() const
{
    return get_music().getPitch();
}

} // namespace rinvid::sound
