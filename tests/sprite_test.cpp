/**********************************************************************
 * Copyright (c) 2024, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <gtest/gtest.h>

#include "core/include/sprite.h"
#include "include/sprite_test.h"
#include "util/include/vector2.h"

using namespace rinvid;

// Test default constructor
TEST_F(SpriteTest, DefaultConstructor)
{
    Sprite sprite{};

    EXPECT_EQ(sprite.bounding_rect().position.x, 0.0F);
    EXPECT_EQ(sprite.bounding_rect().position.y, 0.0F);
    EXPECT_EQ(sprite.bounding_rect().height, 0);
    EXPECT_EQ(sprite.bounding_rect().width, 0);
}

// Test parameterized constructor
TEST_F(SpriteTest, ParameterizedConstructor)
{
    Vector2f top_left{10.0F, 20.0F};
    Sprite   sprite{mock_texture_, 100, 100, top_left};

    EXPECT_EQ(sprite.bounding_rect().width, 100);
    EXPECT_EQ(sprite.bounding_rect().height, 100);
    EXPECT_EQ(sprite.bounding_rect().position.x, top_left.x);
    EXPECT_EQ(sprite.bounding_rect().position.y, top_left.y);
}

// Assure we get no errors when drawing
TEST_F(SpriteTest, Draw)
{
    auto number_of_errors = errors::get_error_count();

    Vector2f top_left{10.0F, 20.0F};
    Sprite   sprite{mock_texture_, 100, 200, top_left, {0.0F, 0.0F}};

    EXPECT_NO_THROW(sprite.draw());

    // Check that the number of errors did not increase.
    ASSERT_TRUE(number_of_errors == errors::get_error_count());
}

// Test move
TEST_F(SpriteTest, Move)
{
    Vector2f top_left{10.0F, 20.0F};
    Sprite   sprite{mock_texture_, 100, 100, top_left, {0.0F, 0.0F}};

    Vector2f move_vector{5.0F, 5.0F};
    sprite.move(move_vector);

    EXPECT_EQ(sprite.bounding_rect().position.x, top_left.x + move_vector.x);
    EXPECT_EQ(sprite.bounding_rect().position.y, top_left.y + move_vector.y);
}

// Test set_position
TEST_F(SpriteTest, SetPosition)
{
    Sprite sprite{mock_texture_, 100, 100, {10.0F, 20.0F}, {0.0F, 0.0F}};

    Vector2f new_position{30.0F, 40.0F};
    sprite.set_position(new_position);

    EXPECT_EQ(sprite.bounding_rect().position.x, new_position.x);
    EXPECT_EQ(sprite.bounding_rect().position.y, new_position.y);
}

// Test bounding_rect method without transformation
TEST_F(SpriteTest, BoundingRect)
{
    Sprite sprite{mock_texture_, 100, 100, {10.0F, 20.0F}, {0.0F, 0.0F}};

    Rect bounding_rect = sprite.bounding_rect();

    EXPECT_EQ(bounding_rect.position.x, 10.0F);
    EXPECT_EQ(bounding_rect.position.y, 20.0F);
    EXPECT_EQ(bounding_rect.width, 100);
    EXPECT_EQ(bounding_rect.height, 100);
}

// Test transform
TEST_F(SpriteTest, Transformed)
{
    Sprite sprite{mock_texture_, 100, 100, {10.0F, 20.0F}, {0.0F, 0.0F}};

    // Apply some transformation
    sprite.rotate(10.0F);

    EXPECT_TRUE(sprite.is_transformed());
}
