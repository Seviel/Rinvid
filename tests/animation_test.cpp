/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <gtest/gtest.h>

#include "core/include/animation.h"
#include "include/animation_test.h"

using namespace rinvid;

static std::vector<Rect> make_regions()
{
    return {
        {{0.0F, 0.0F}, 10, 10},
        {{10.0F, 0.0F}, 10, 10},
        {{20.0F, 0.0F}, 10, 10},
    };
}

TEST_F(AnimationTest, FrameIndex_UsesDeltaTimePreview)
{
    Animation animation{2.0, make_regions()};

    EXPECT_EQ(animation.frame_index(0.75), 1U);
}

TEST_F(AnimationTest, NormalAnimation_ClampsAtLastFrame)
{
    Animation animation{2.0, make_regions()};

    animation.advance(2.0);

    EXPECT_EQ(animation.frame_index(), 2U);
    EXPECT_TRUE(animation.is_finished());
}

TEST_F(AnimationTest, LoopingAnimation_WrapsAround)
{
    Animation animation{2.0, make_regions(), AnimationMode::Looping};

    animation.advance(1.5);

    EXPECT_EQ(animation.frame_index(), 0U);
    EXPECT_FALSE(animation.is_finished());
}

TEST_F(AnimationTest, Reset_ReturnsAnimationToFirstFrame)
{
    Animation animation{2.0, make_regions()};

    animation.advance(1.0);
    animation.reset();

    EXPECT_EQ(animation.frame_index(), 0U);
}

TEST_F(AnimationTest, ZeroFramerate_StaysOnFirstFrame)
{
    Animation animation{0.0, make_regions()};

    animation.advance(100.0);

    EXPECT_EQ(animation.frame_index(), 0U);
    EXPECT_FALSE(animation.is_finished());
}

TEST_F(AnimationTest, Setup_RegionsUsedForCurrentFrame)
{
    Animation animation{};
    auto      regions = make_regions();

    animation.setup(1.0, regions, AnimationMode::Normal);

    Rect frame = animation.frame(2.0);

    EXPECT_FLOAT_EQ(frame.position.x, 20.0F);
    EXPECT_FLOAT_EQ(frame.position.y, 0.0F);
    EXPECT_EQ(frame.width, 10);
    EXPECT_EQ(frame.height, 10);
}
