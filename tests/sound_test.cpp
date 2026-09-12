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
#include <limits>
#include <optional>
#include <string>
#include <thread>
#include <utility>
#include <vector>
#include <type_traits>

#include <SFML/Audio/PlaybackDevice.hpp>
#include <gtest/gtest.h>

#include <rinvid/sound/sound.h>
#include <rinvid/util/error.h>

using rinvid::AudioError;
using rinvid::sound::PlaybackStatus;
using rinvid::sound::Sound;

static_assert(std::is_copy_constructible_v<Sound>);
static_assert(std::is_copy_assignable_v<Sound>);
static_assert(std::is_nothrow_move_constructible_v<Sound>);
static_assert(std::is_nothrow_move_assignable_v<Sound>);

namespace
{

constexpr char sound_file[]{"sound_resources/valid_sound.wav"};
constexpr char short_sound_file[]{"sound_resources/short_sound.wav"};

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

class SoundTest : public ::testing::Test
{
  protected:
    static void SetUpTestSuite()
    {
        // Exercise the real audio backend without requiring speakers or a physical audio device.
        ASSERT_TRUE(sf::PlaybackDevice::setDeviceToNull());
    }
};

TEST_F(SoundTest, Constructor_InvalidFileIncludesPathInAudioError)
{
    try
    {
        const Sound sound{"resources/missing_sound.wav"};
        FAIL() << "Expected AudioError";
    }
    catch (const AudioError& error)
    {
        EXPECT_NE(std::string{error.what()}.find("missing_sound.wav"), std::string::npos);
    }
}

TEST_F(SoundTest, Constructor_RejectsNonAudioFile)
{
    EXPECT_THROW(Sound{"sound_resources/valid_image.png"}, AudioError);
}

TEST_F(SoundTest, Constructor_DefaultSettingsAndDuration)
{
    const Sound sound{sound_file};

    EXPECT_EQ(sound.get_status(), PlaybackStatus::Stopped);
    EXPECT_DOUBLE_EQ(sound.get_duration(), 2.0);
    EXPECT_DOUBLE_EQ(sound.get_playing_offset(), 0.0);
    EXPECT_FLOAT_EQ(sound.get_volume(), 100.0F);
    EXPECT_FLOAT_EQ(sound.get_pitch(), 1.0F);
    EXPECT_FALSE(sound.is_looping());
}

TEST_F(SoundTest, CopyConstructorOwnsSamplesAfterOriginalIsDestroyed)
{
    std::optional<Sound> copy;
    {
        Sound original{sound_file};
        original.set_looping(true);
        original.set_volume(30.0F);
        original.set_pitch(1.25F);
        original.set_playing_offset(0.75);
        original.play();
        copy.emplace(original);

        EXPECT_EQ(original.get_status(), PlaybackStatus::Playing);
        EXPECT_EQ(copy->get_status(), PlaybackStatus::Stopped);
        EXPECT_DOUBLE_EQ(copy->get_playing_offset(), 0.0);
        original.set_volume(80.0F);
    }

    EXPECT_DOUBLE_EQ(copy->get_duration(), 2.0);
    EXPECT_FLOAT_EQ(copy->get_volume(), 30.0F);
    EXPECT_FLOAT_EQ(copy->get_pitch(), 1.25F);
    EXPECT_TRUE(copy->is_looping());
    copy->play();
    EXPECT_TRUE(wait_until([&copy] { return copy->get_playing_offset() > 0.0; }));
}

TEST_F(SoundTest, CopyAssignmentReplacesLivePlaybackAndOwnsIndependentSamples)
{
    Sound copy{short_sound_file};
    copy.set_looping(true);
    copy.play();
    ASSERT_TRUE(wait_until([&copy] { return copy.get_playing_offset() > 0.0; }));
    {
        Sound original{sound_file};
        original.set_volume(40.0F);
        original.set_pitch(0.75F);
        original.set_looping(true);
        original.play();
        original.pause();
        original.set_playing_offset(0.75);
        copy = original;
        EXPECT_EQ(original.get_status(), PlaybackStatus::Paused);
        EXPECT_NEAR(original.get_playing_offset(), 0.75, 0.000125);
        EXPECT_EQ(copy.get_status(), PlaybackStatus::Stopped);
        EXPECT_DOUBLE_EQ(copy.get_playing_offset(), 0.0);
        original = Sound{short_sound_file};
    }

    EXPECT_DOUBLE_EQ(copy.get_duration(), 2.0);
    EXPECT_FLOAT_EQ(copy.get_volume(), 40.0F);
    EXPECT_FLOAT_EQ(copy.get_pitch(), 0.75F);
    EXPECT_TRUE(copy.is_looping());
    copy.play();
    EXPECT_TRUE(wait_until([&copy] { return copy.get_playing_offset() > 0.0; }));
}

TEST_F(SoundTest, SelfCopyAssignmentPreservesPlayback)
{
    Sound sound{sound_file};
    sound.set_looping(true);
    sound.set_playing_offset(0.75);
    sound.play();
    const Sound& alias{sound};
    sound = alias;

    EXPECT_EQ(sound.get_status(), PlaybackStatus::Playing);
    sound.pause();
    EXPECT_GE(sound.get_playing_offset(), 0.75);
}

TEST_F(SoundTest, FailedCopyFromMovedFromSoundLeavesDestinationUnchanged)
{
    Sound       original{sound_file};
    const Sound moved{std::move(original)};
    Sound       destination{short_sound_file};
    destination.set_looping(true);
    destination.play();

    EXPECT_THROW(Sound{original}, AudioError);
    EXPECT_THROW(destination = original, AudioError);
    EXPECT_DOUBLE_EQ(destination.get_duration(), 0.25);
    EXPECT_TRUE(destination.is_looping());
    EXPECT_EQ(destination.get_status(), PlaybackStatus::Playing);
}

TEST_F(SoundTest, DestructionDuringPlaybackReleasesLastAudioResourceSafely)
{
    // Exercise both a single active sound and overlapping sounds, including device teardown.
    for (std::uint32_t iteration{0}; iteration < 3; ++iteration)
    {
        Sound sound{short_sound_file};
        sound.set_looping(true);
        sound.play();
        ASSERT_TRUE(wait_until([&sound] { return sound.get_playing_offset() > 0.0; }));
        {
            Sound other{sound_file};
            other.set_looping(true);
            other.play();
            ASSERT_TRUE(wait_until([&other] { return other.get_playing_offset() > 0.0; }));
        }
        EXPECT_EQ(sound.get_status(), PlaybackStatus::Playing);
    }
}

TEST_F(SoundTest, MoveConstructorPreservesPausedPlaybackAndSamples)
{
    std::optional<Sound> moved;
    {
        Sound original{sound_file};
        original.set_looping(true);
        original.set_volume(30.0F);
        original.set_pitch(1.25F);
        original.play();
        original.pause();
        original.set_playing_offset(0.75);
        moved.emplace(std::move(original));
    }

    EXPECT_EQ(moved->get_status(), PlaybackStatus::Paused);
    EXPECT_NEAR(moved->get_playing_offset(), 0.75, 0.000125);
    EXPECT_FLOAT_EQ(moved->get_volume(), 30.0F);
    EXPECT_FLOAT_EQ(moved->get_pitch(), 1.25F);
    EXPECT_TRUE(moved->is_looping());
    EXPECT_DOUBLE_EQ(moved->get_duration(), 2.0);
    moved->play();
    EXPECT_TRUE(wait_until([&moved] { return moved->get_playing_offset() > 0.75; }));
}

TEST_F(SoundTest, MoveAssignmentPreservesLivePlaybackAndReplacesDestination)
{
    Sound destination{short_sound_file};
    destination.set_looping(true);
    destination.play();
    ASSERT_TRUE(wait_until([&destination] { return destination.get_playing_offset() > 0.0; }));
    {
        Sound original{sound_file};
        original.set_looping(true);
        original.set_volume(40.0F);
        original.set_pitch(0.75F);
        original.set_playing_offset(0.75);
        original.play();
        destination = std::move(original);
    }

    EXPECT_EQ(destination.get_status(), PlaybackStatus::Playing);
    destination.pause();
    EXPECT_GE(destination.get_playing_offset(), 0.75);
    EXPECT_DOUBLE_EQ(destination.get_duration(), 2.0);
    EXPECT_FLOAT_EQ(destination.get_volume(), 40.0F);
    EXPECT_FLOAT_EQ(destination.get_pitch(), 0.75F);
    EXPECT_TRUE(destination.is_looping());
}

TEST_F(SoundTest, VectorReallocationPreservesLivePlayback)
{
    std::vector<Sound> sounds;
    sounds.reserve(1);
    sounds.emplace_back(sound_file);
    sounds.front().set_looping(true);
    sounds.front().set_playing_offset(0.75);
    sounds.front().play();
    sounds.reserve(sounds.capacity() + 1);

    EXPECT_EQ(sounds.front().get_status(), PlaybackStatus::Playing);
    sounds.front().pause();
    EXPECT_GE(sounds.front().get_playing_offset(), 0.75);
}

TEST_F(SoundTest, MovedFromSoundRejectsOperationsAndCanBeReassigned)
{
    Sound       original{sound_file};
    const Sound moved{std::move(original)};

    EXPECT_THROW(original.play(), AudioError);
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

    original = Sound{short_sound_file};
    EXPECT_EQ(original.get_status(), PlaybackStatus::Stopped);
    EXPECT_DOUBLE_EQ(original.get_duration(), 0.25);
    EXPECT_DOUBLE_EQ(moved.get_duration(), 2.0);
}

TEST_F(SoundTest, SelfMoveAssignmentPreservesPlayback)
{
    Sound sound{sound_file};
    sound.set_volume(30.0F);
    sound.set_looping(true);
    sound.play();
    Sound& alias{sound};
    sound = std::move(alias);

    EXPECT_EQ(sound.get_status(), PlaybackStatus::Playing);
    EXPECT_FLOAT_EQ(sound.get_volume(), 30.0F);
    EXPECT_TRUE(sound.is_looping());
}

TEST_F(SoundTest, PauseResumeRestartAndStopHaveDistinctBehavior)
{
    Sound sound{sound_file};
    sound.set_looping(true);
    sound.pause();
    EXPECT_EQ(sound.get_status(), PlaybackStatus::Stopped);
    sound.play();
    EXPECT_EQ(sound.get_status(), PlaybackStatus::Playing);
    sound.pause();
    EXPECT_EQ(sound.get_status(), PlaybackStatus::Paused);
    sound.set_playing_offset(0.75);
    sound.play();
    sound.pause();
    EXPECT_GE(sound.get_playing_offset(), 0.75);

    sound.play();
    sound.play();
    sound.pause();
    EXPECT_LT(sound.get_playing_offset(), 0.75);
    sound.stop();
    EXPECT_EQ(sound.get_status(), PlaybackStatus::Stopped);
    EXPECT_DOUBLE_EQ(sound.get_playing_offset(), 0.0);
    sound.set_playing_offset(0.75);
    sound.stop();
    EXPECT_DOUBLE_EQ(sound.get_playing_offset(), 0.0);
}

TEST_F(SoundTest, SeekingAcceptsEndpointsAndRejectsInvalidPositionsWithoutChangingOffset)
{
    Sound sound{sound_file};
    sound.set_playing_offset(sound.get_duration());
    EXPECT_NEAR(sound.get_playing_offset(), 2.0, 0.000125);
    sound.set_playing_offset(0.0);
    EXPECT_DOUBLE_EQ(sound.get_playing_offset(), 0.0);
    sound.set_playing_offset(0.75);

    for (const double invalid :
         {-0.1, std::nextafter(2.0, 3.0), std::numeric_limits<double>::max(),
          std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(),
          std::numeric_limits<double>::quiet_NaN()})
    {
        EXPECT_THROW(sound.set_playing_offset(invalid), AudioError);
        EXPECT_NEAR(sound.get_playing_offset(), 0.75, 0.000125);
        EXPECT_EQ(sound.get_status(), PlaybackStatus::Stopped);
    }
}

TEST_F(SoundTest, VolumeClampsFiniteValuesAndRejectsNonFiniteValues)
{
    Sound sound{sound_file};
    sound.set_volume(-10.0F);
    EXPECT_FLOAT_EQ(sound.get_volume(), 0.0F);
    sound.set_volume(110.0F);
    EXPECT_FLOAT_EQ(sound.get_volume(), 100.0F);
    sound.set_volume(35.0F);

    for (const float invalid :
         {std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(),
          std::numeric_limits<float>::quiet_NaN()})
    {
        EXPECT_THROW(sound.set_volume(invalid), AudioError);
        EXPECT_FLOAT_EQ(sound.get_volume(), 35.0F);
    }
}

TEST_F(SoundTest, PitchRejectsInvalidFactorsWithoutChangingPitchOrDuration)
{
    Sound sound{sound_file};
    sound.set_pitch(1.5F);
    EXPECT_FLOAT_EQ(sound.get_pitch(), 1.5F);

    for (const float invalid :
         {0.0F, -1.0F, std::numeric_limits<float>::infinity(),
          -std::numeric_limits<float>::infinity(), std::numeric_limits<float>::quiet_NaN()})
    {
        EXPECT_THROW(sound.set_pitch(invalid), AudioError);
        EXPECT_FLOAT_EQ(sound.get_pitch(), 1.5F);
        EXPECT_DOUBLE_EQ(sound.get_duration(), 2.0);
    }
}

TEST_F(SoundTest, NonLoopingSoundFinishesNaturally)
{
    Sound sound{short_sound_file};
    sound.play();
    EXPECT_TRUE(wait_until([&sound] { return sound.get_status() == PlaybackStatus::Stopped; }));
}

TEST_F(SoundTest, LoopingSoundWrapsUntilLoopingIsDisabled)
{
    Sound sound{short_sound_file};
    sound.set_looping(true);
    sound.play();
    double previous_offset{0.0};
    EXPECT_TRUE(wait_until(
        [&sound, &previous_offset]
        {
            const double offset{sound.get_playing_offset()};
            const bool   wrapped{offset < previous_offset};
            previous_offset = offset;
            return wrapped;
        }));
    EXPECT_EQ(sound.get_status(), PlaybackStatus::Playing);
    sound.set_looping(false);
    EXPECT_FALSE(sound.is_looping());
    EXPECT_TRUE(wait_until([&sound] { return sound.get_status() == PlaybackStatus::Stopped; }));
}
