/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <string>

#include <gtest/gtest.h>

#include <rinvid/sound/sound.h>
#include <rinvid/util/error.h>

TEST(SoundTest, Constructor_InvalidFileThrowsAudioError)
{
    try
    {
        const rinvid::sound::Sound sound{std::string{"resources/missing_sound.wav"}};
        (void)sound;
        FAIL() << "Expected AudioError";
    }
    catch (const rinvid::AudioError& error)
    {
        EXPECT_NE(std::string{error.what()}.find("missing_sound.wav"), std::string::npos);
    }
}
