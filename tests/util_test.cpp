/**********************************************************************
 * Copyright (c) 2024 - 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <gtest/gtest.h>

#include "include/util_test.h"
#include <rinvid/util/collision_detection.h>
#include <rinvid/util/color.h>
#include <rinvid/util/rect.h>
#include <rinvid/util/vector2.h>

using namespace rinvid;

// Test Color construction
TEST_F(UtilTest, Color_1)
{
    Color color{};

    EXPECT_EQ(color.r, 0.0F);
    EXPECT_EQ(color.g, 0.0F);
    EXPECT_EQ(color.b, 0.0F);
    EXPECT_EQ(color.a, 0.0F);
}

TEST_F(UtilTest, Color_2)
{
    Color color{12, 108, 250, 125};

    EXPECT_EQ(color.r, static_cast<float>(12) / static_cast<float>(UINT8_MAX));
    EXPECT_EQ(color.g, static_cast<float>(108) / static_cast<float>(UINT8_MAX));
    EXPECT_EQ(color.b, static_cast<float>(250) / static_cast<float>(UINT8_MAX));
    EXPECT_EQ(color.a, static_cast<float>(125) / static_cast<float>(UINT8_MAX));
}

TEST_F(UtilTest, Color_3)
{
    Color color{12U, 108U, 250U, 125U};

    EXPECT_EQ(color.r, static_cast<float>(12U) / static_cast<float>(UINT8_MAX));
    EXPECT_EQ(color.g, static_cast<float>(108U) / static_cast<float>(UINT8_MAX));
    EXPECT_EQ(color.b, static_cast<float>(250U) / static_cast<float>(UINT8_MAX));
    EXPECT_EQ(color.a, static_cast<float>(125U) / static_cast<float>(UINT8_MAX));
}

TEST_F(UtilTest, Color_4)
{
    Color color{0xFF7FFF00};

    float tolerance = 1e-3;

    EXPECT_NEAR(color.r, 1.0F, tolerance);
    EXPECT_NEAR(color.g, 0.498F, tolerance);
    EXPECT_NEAR(color.b, 1.0F, tolerance);
    EXPECT_NEAR(color.a, 0.0F, tolerance);
}

TEST_F(UtilTest, Color_FloatConstructor)
{
    Color color{0.25F, 0.5F, 0.75F, 1.0F};

    EXPECT_FLOAT_EQ(color.r, 0.25F);
    EXPECT_FLOAT_EQ(color.g, 0.5F);
    EXPECT_FLOAT_EQ(color.b, 0.75F);
    EXPECT_FLOAT_EQ(color.a, 1.0F);
}

// Test Vector2
TEST_F(UtilTest, Vector2_Construction)
{
    Vector2f vec2{};

    EXPECT_EQ(vec2.x, 0.0F);
    EXPECT_EQ(vec2.y, 0.0F);
}

TEST_F(UtilTest, Vector2_Construction_2)
{
    Vector2f vec2{5.0F, 8.0F};

    EXPECT_EQ(vec2.x, 5.0F);
    EXPECT_EQ(vec2.y, 8.0F);
}

TEST_F(UtilTest, Vector2_Move)
{
    Vector2f vec2{1.0F, 1.0F};
    Vector2f move_vec{5.0F, 8.0F};

    vec2.move(move_vec);

    EXPECT_EQ(vec2.x, 1.0F + 5.0F);
    EXPECT_EQ(vec2.y, 1.0F + 8.0F);
}

TEST_F(UtilTest, Vector2_Set)
{
    Vector2f vec2{1.0F, 1.0F};
    Vector2f new_vec{5.0F, 8.0F};

    vec2.set(new_vec);

    EXPECT_EQ(vec2.x, new_vec.x);
    EXPECT_EQ(vec2.y, new_vec.y);
}

// Const used for testing collision detection.
static constexpr int32_t RECT_WIDTH  = 100;
static constexpr int32_t RECT_HEIGHT = 100;

// Helper function for testing collision detection. Creates 2 rects at the same position, then moves
// one of them 3 times. Expects that rects will still intersect after first two moves but won't
// after third, so pass move_vec in accordance to that.
static void test_collision_detection(Vector2f move_vec)
{
    Vector2f position{8.0F, 8.0F};
    Rect     rect_1{position, RECT_WIDTH, RECT_HEIGHT};
    Rect     rect_2{position, RECT_WIDTH, RECT_HEIGHT};

    EXPECT_TRUE(intersects(rect_1, rect_2));

    auto& rect_2_pos = rect_2.position;
    rect_2_pos.move(move_vec);

    EXPECT_TRUE(intersects(rect_1, rect_2));

    rect_2_pos.move(move_vec);

    EXPECT_TRUE(intersects(rect_1, rect_2));

    rect_2_pos.move(move_vec);

    EXPECT_FALSE(intersects(rect_1, rect_2));
}

TEST_F(UtilTest, CollisionDetection_MoveLeft)
{
    Vector2f move_vec{static_cast<float>(-RECT_WIDTH / 2), 0.0F};
    test_collision_detection(move_vec);
}

TEST_F(UtilTest, CollisionDetection_MoveRight)
{
    Vector2f move_vec{static_cast<float>(RECT_WIDTH / 2), 0.0F};
    test_collision_detection(move_vec);
}

TEST_F(UtilTest, CollisionDetection_MoveUp)
{
    Vector2f move_vec{0.0F, static_cast<float>(-RECT_HEIGHT / 2)};
    test_collision_detection(move_vec);
}

TEST_F(UtilTest, CollisionDetection_MoveDown)
{
    Vector2f move_vec{0.0F, static_cast<float>(RECT_HEIGHT / 2)};
    test_collision_detection(move_vec);
}

TEST_F(UtilTest, CollisionDetection_TouchingEdgesCountsAsIntersection)
{
    Rect rect_1{{0.0F, 0.0F}, 10, 10};
    Rect rect_2{{10.0F, 0.0F}, 10, 10};

    EXPECT_TRUE(intersects(rect_1, rect_2));
}

TEST_F(UtilTest, CollisionDetection_DiagonalGapDoesNotIntersect)
{
    Rect rect_1{{0.0F, 0.0F}, 10, 10};
    Rect rect_2{{11.0F, 11.0F}, 10, 10};

    EXPECT_FALSE(intersects(rect_1, rect_2));
}

TEST_F(UtilTest, RectScale_ShrinksAroundCenterAndReturnsSelf)
{
    Rect  rect{{10.0F, 20.0F}, 100, 60};
    Rect& scaled_rect = rect.scale(0.5F);

    EXPECT_EQ(&scaled_rect, &rect);
    EXPECT_FLOAT_EQ(rect.position.x, 35.0F);
    EXPECT_FLOAT_EQ(rect.position.y, 35.0F);
    EXPECT_EQ(rect.width, 50);
    EXPECT_EQ(rect.height, 30);
}
