/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <chrono>
#include <cmath>
#include <cstdint>
#include <limits>
#include <optional>
#include <string>
#include <thread>
#include <utility>
#include <type_traits>

#include <SFML/Audio/PlaybackDevice.hpp>
#include <gtest/gtest.h>

#include <rinvid/sound/music.h>
#include <rinvid/sound/sound.h>

using rinvid::AudioError;
using rinvid::sound::Music;
using rinvid::sound::PlaybackStatus;

static_assert(!std::is_copy_constructible_v<Music>);
static_assert(!std::is_copy_assignable_v<Music>);
static_assert(std::is_nothrow_move_constructible_v<Music>);
static_assert(std::is_nothrow_move_assignable_v<Music>);

namespace
{

constexpr char music_file[]{"sound_resources/valid_sound.wav"};
constexpr char short_music_file[]{"sound_resources/short_sound.wav"};

template <typename Predicate>
bool wait_until(Predicate predicate)
{
    const auto deadline{std::chrono::steady_clock::now() + std::chrono::seconds{2}};
    while (!predicate())
    {
        if (std::chrono::steady_clock::now() >= deadline)
        {
            return false;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds{5});
    }

    return true;
}

} // namespace

class MusicTest : public ::testing::Test
{
  protected:
    static void SetUpTestSuite()
    {
        ASSERT_TRUE(sf::PlaybackDevice::setDeviceToNull());
    }
};

TEST_F(MusicTest, ConstructorOpensStoppedTrackWithLoopingEnabled)
{
    const Music music{music_file};
    EXPECT_EQ(music.get_status(), PlaybackStatus::Stopped);
    EXPECT_DOUBLE_EQ(music.get_duration(), 2.0);
    EXPECT_DOUBLE_EQ(music.get_playing_offset(), 0.0);
    EXPECT_TRUE(music.is_looping());
    EXPECT_FLOAT_EQ(music.get_volume(), 100.0F);
    EXPECT_FLOAT_EQ(music.get_pitch(), 1.0F);
}

TEST_F(MusicTest, ConstructorReportsInvalidFilesWithPath)
{
    for (const char* path :
         {"sound_resources/missing_music.ogg", "sound_resources/valid_image.png"})
    {
        try
        {
            const Music music{path};
            FAIL() << "Expected AudioError";
        }
        catch (const AudioError& error)
        {
            EXPECT_NE(std::string{error.what()}.find(path), std::string::npos);
        }
    }
}

TEST_F(MusicTest, PauseResumeRestartAndStop)
{
    Music music{music_file};
    music.pause();
    EXPECT_EQ(music.get_status(), PlaybackStatus::Stopped);
    music.play();
    ASSERT_TRUE(wait_until([&music] { return music.get_playing_offset() > 0.0; }));
    music.pause();
    EXPECT_EQ(music.get_status(), PlaybackStatus::Paused);
    music.set_playing_offset(0.75);
    music.play();
    music.pause();
    EXPECT_GE(music.get_playing_offset(), 0.75);
    music.play();
    music.play();
    music.pause();
    EXPECT_LT(music.get_playing_offset(), 0.75);
    music.stop();
    EXPECT_EQ(music.get_status(), PlaybackStatus::Stopped);
    EXPECT_DOUBLE_EQ(music.get_playing_offset(), 0.0);
    music.set_playing_offset(0.75);
    music.stop();
    EXPECT_DOUBLE_EQ(music.get_playing_offset(), 0.0);
}

TEST_F(MusicTest, SwitchingTracksStartsFromBeginningAndPreservesSettings)
{
    for (const auto state :
         {PlaybackStatus::Stopped, PlaybackStatus::Paused, PlaybackStatus::Playing})
    {
        Music music{short_music_file};
        music.set_volume(35.0F);
        music.set_pitch(1.25F);
        music.set_looping(false);
        if (state != PlaybackStatus::Stopped)
        {
            music.play();
            if (state == PlaybackStatus::Paused)
            {
                music.pause();
            }
        }
        music.set_playing_offset(0.125);
        music.play(music_file);

        EXPECT_EQ(music.get_status(), PlaybackStatus::Playing);
        EXPECT_DOUBLE_EQ(music.get_duration(), 2.0);
        EXPECT_FLOAT_EQ(music.get_volume(), 35.0F);
        EXPECT_FLOAT_EQ(music.get_pitch(), 1.25F);
        EXPECT_FALSE(music.is_looping());
        music.pause();
        EXPECT_LT(music.get_playing_offset(), 0.125);
        music.play();
        EXPECT_TRUE(wait_until([&music] { return music.get_playing_offset() > 0.125; }));
    }
}

TEST_F(MusicTest, FailedSwitchPreservesCurrentTrackAndPlaybackState)
{
    Music music{music_file};
    music.set_volume(35.0F);
    music.set_pitch(0.75F);
    music.set_playing_offset(0.75);
    music.play();
    EXPECT_THROW(music.play("sound_resources/missing_music.ogg"), AudioError);
    EXPECT_EQ(music.get_status(), PlaybackStatus::Playing);
    music.pause();
    EXPECT_GE(music.get_playing_offset(), 0.75);
    const double offset{music.get_playing_offset()};
    EXPECT_THROW(music.play("sound_resources/valid_image.png"), AudioError);
    EXPECT_EQ(music.get_status(), PlaybackStatus::Paused);
    EXPECT_DOUBLE_EQ(music.get_playing_offset(), offset);
    EXPECT_DOUBLE_EQ(music.get_duration(), 2.0);
    EXPECT_FLOAT_EQ(music.get_volume(), 35.0F);
    EXPECT_FLOAT_EQ(music.get_pitch(), 0.75F);
    EXPECT_TRUE(music.is_looping());
    music.play();
    EXPECT_TRUE(wait_until([&music, offset] { return music.get_playing_offset() > offset; }));
}

TEST_F(MusicTest, SwitchingToSameFileRestartsIt)
{
    Music music{music_file};
    music.set_playing_offset(0.75);
    music.play();
    music.play(music_file);
    EXPECT_EQ(music.get_status(), PlaybackStatus::Playing);
    music.pause();
    EXPECT_LT(music.get_playing_offset(), 0.75);
}

TEST_F(MusicTest, DefaultLoopRepeatsAndDisablingLoopLetsTrackFinish)
{
    Music music{short_music_file};
    music.play();
    double previous_offset{0.0};
    EXPECT_TRUE(wait_until(
        [&music, &previous_offset]
        {
            const double offset{music.get_playing_offset()};
            const bool   wrapped{offset < previous_offset};
            previous_offset = offset;
            return wrapped;
        }));
    EXPECT_EQ(music.get_status(), PlaybackStatus::Playing);
    music.set_looping(false);
    EXPECT_FALSE(music.is_looping());
    EXPECT_TRUE(wait_until([&music] { return music.get_status() == PlaybackStatus::Stopped; }));
}

TEST_F(MusicTest, SeekingPreservesStateAndRejectsInvalidOffsets)
{
    Music music{music_file};
    music.set_playing_offset(music.get_duration());
    EXPECT_NEAR(music.get_playing_offset(), 2.0, 0.000125);
    music.set_playing_offset(0.0);
    EXPECT_DOUBLE_EQ(music.get_playing_offset(), 0.0);
    for (const auto state :
         {PlaybackStatus::Stopped, PlaybackStatus::Paused, PlaybackStatus::Playing})
    {
        if (state != PlaybackStatus::Stopped)
        {
            music.play();
            if (state == PlaybackStatus::Paused)
            {
                music.pause();
            }
        }
        music.set_playing_offset(0.75);
        EXPECT_EQ(music.get_status(), state);
        if (state == PlaybackStatus::Playing)
        {
            music.pause();
        }
        EXPECT_NEAR(music.get_playing_offset(), 0.75, 0.05);
    }

    const double offset{music.get_playing_offset()};
    for (const double invalid :
         {-0.1, std::nextafter(2.0, 3.0), std::numeric_limits<double>::infinity(),
          std::numeric_limits<double>::quiet_NaN()})
    {
        EXPECT_THROW(music.set_playing_offset(invalid), AudioError);
        EXPECT_DOUBLE_EQ(music.get_playing_offset(), offset);
        EXPECT_EQ(music.get_status(), PlaybackStatus::Paused);
    }
}

TEST_F(MusicTest, VolumeAndPitchValidateParameters)
{
    Music music{music_file};
    music.set_volume(-10.0F);
    EXPECT_FLOAT_EQ(music.get_volume(), 0.0F);
    music.set_volume(110.0F);
    EXPECT_FLOAT_EQ(music.get_volume(), 100.0F);
    music.set_volume(35.0F);
    music.set_pitch(1.5F);
    EXPECT_FLOAT_EQ(music.get_pitch(), 1.5F);
    EXPECT_DOUBLE_EQ(music.get_duration(), 2.0);
    for (const float invalid :
         {std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(),
          std::numeric_limits<float>::quiet_NaN()})
    {
        EXPECT_THROW(music.set_volume(invalid), AudioError);
        EXPECT_THROW(music.set_pitch(invalid), AudioError);
    }
    EXPECT_THROW(music.set_pitch(0.0F), AudioError);
    EXPECT_THROW(music.set_pitch(-1.0F), AudioError);
    EXPECT_FLOAT_EQ(music.get_volume(), 35.0F);
    EXPECT_FLOAT_EQ(music.get_pitch(), 1.5F);
}

TEST_F(MusicTest, MovingTransfersLiveStreamBeyondOriginalLifetime)
{
    std::optional<Music> moved;
    {
        Music original{music_file};
        original.set_volume(30.0F);
        original.set_pitch(0.75F);
        original.set_playing_offset(0.75);
        original.play();
        moved.emplace(std::move(original));
    }
    EXPECT_EQ(moved->get_status(), PlaybackStatus::Playing);
    EXPECT_TRUE(wait_until([&moved] { return moved->get_playing_offset() > 0.75; }));
    EXPECT_FLOAT_EQ(moved->get_volume(), 30.0F);
    EXPECT_FLOAT_EQ(moved->get_pitch(), 0.75F);
    EXPECT_TRUE(moved->is_looping());
    EXPECT_DOUBLE_EQ(moved->get_duration(), 2.0);
}

TEST_F(MusicTest, MoveAssignmentReplacesPlayingTrackAndPreservesPausedPosition)
{
    Music destination{short_music_file};
    destination.play();
    ASSERT_TRUE(wait_until([&destination] { return destination.get_playing_offset() > 0.0; }));
    {
        Music original{music_file};
        original.play();
        original.pause();
        original.set_playing_offset(0.75);
        destination = std::move(original);
    }
    EXPECT_EQ(destination.get_status(), PlaybackStatus::Paused);
    EXPECT_NEAR(destination.get_playing_offset(), 0.75, 0.000125);
    EXPECT_DOUBLE_EQ(destination.get_duration(), 2.0);
    destination.play();
    EXPECT_TRUE(wait_until([&destination] { return destination.get_playing_offset() > 0.75; }));
    Music& alias{destination};
    destination = std::move(alias);
    EXPECT_EQ(destination.get_status(), PlaybackStatus::Playing);
}

TEST_F(MusicTest, MovedFromMusicRejectsAccessAndCanBeReassigned)
{
    Music       original{music_file};
    const Music moved{std::move(original)};
    EXPECT_THROW(original.play(), AudioError);
    EXPECT_THROW(original.play(short_music_file), AudioError);
    EXPECT_THROW(original.pause(), AudioError);
    EXPECT_THROW(original.stop(), AudioError);
    EXPECT_THROW(original.get_status(), AudioError);
    EXPECT_THROW(original.get_duration(), AudioError);
    EXPECT_THROW(original.get_playing_offset(), AudioError);
    EXPECT_THROW(original.set_playing_offset(0.0), AudioError);
    EXPECT_THROW(original.set_looping(true), AudioError);
    EXPECT_THROW(original.is_looping(), AudioError);
    EXPECT_THROW(original.set_volume(50.0F), AudioError);
    EXPECT_THROW(original.get_volume(), AudioError);
    EXPECT_THROW(original.set_pitch(1.0F), AudioError);
    EXPECT_THROW(original.get_pitch(), AudioError);
    original = Music{short_music_file};
    EXPECT_DOUBLE_EQ(original.get_duration(), 0.25);
    EXPECT_EQ(original.get_status(), PlaybackStatus::Stopped);
}

TEST_F(MusicTest, DestructionDuringPlaybackAndAlongsideSoundIsSafe)
{
    for (std::uint32_t iteration{0}; iteration < 3; ++iteration)
    {
        Music music{music_file};
        music.play();
        ASSERT_TRUE(wait_until([&music] { return music.get_playing_offset() > 0.0; }));
        {
            rinvid::sound::Sound effect{short_music_file};
            effect.set_looping(true);
            effect.play();
            music.play(short_music_file);
            ASSERT_TRUE(wait_until([&music] { return music.get_playing_offset() > 0.0; }));
        }
        EXPECT_EQ(music.get_status(), PlaybackStatus::Playing);
    }
}
