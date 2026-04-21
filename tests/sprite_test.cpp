/**********************************************************************
 * Copyright (c) 2024 - 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <array>

#include <gtest/gtest.h>

#include <rinvid/core/sprite.h>
#include <rinvid/util/vector2.h>

#include "include/sprite_test.h"

using namespace rinvid;

namespace
{

std::array<float, 20> read_bound_sprite_vertices()
{
    GLint vertex_buffer_object{};
    GL_CALL(glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &vertex_buffer_object));

    std::array<float, 20> vertices{};
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, static_cast<GLuint>(vertex_buffer_object)));
    GL_CALL(glGetBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices.data()));
    return vertices;
}

} // namespace

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

TEST_F(SpriteTest, Draw_UpdatesOriginToCenter)
{
    Sprite sprite{mock_texture_, 100, 200, {10.0F, 20.0F}, {0.0F, 0.0F}};

    sprite.draw();

    EXPECT_FLOAT_EQ(sprite.get_origin().x, 60.0F);
    EXPECT_FLOAT_EQ(sprite.get_origin().y, 120.0F);
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

TEST_F(SpriteTest, Setup_UpdatesSpriteGeometry)
{
    Sprite sprite{};

    sprite.setup(mock_texture_, 64, 32, {15.0F, 25.0F}, {0.0F, 0.0F});

    EXPECT_EQ(sprite.bounding_rect().width, 64);
    EXPECT_EQ(sprite.bounding_rect().height, 32);
    EXPECT_EQ(sprite.bounding_rect().position.x, 15.0F);
    EXPECT_EQ(sprite.bounding_rect().position.y, 25.0F);
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

TEST_F(SpriteTest, AnimatedDraw_AdvancesAnimation)
{
    auto number_of_errors = errors::get_error_count();

    Sprite            sprite{mock_texture_, 100, 100, {10.0F, 20.0F}, {0.0F, 0.0F}};
    std::vector<Rect> frames{{{0.0F, 0.0F}, 1, 1}, {{1.0F, 0.0F}, 1, 1}};
    Animation         animation{1.0, frames};

    sprite.get_animation().add_animation("blink", animation);
    sprite.get_animation().play("blink");

    EXPECT_NO_THROW(sprite.draw(1.0));
    EXPECT_TRUE(sprite.get_animation().is_animation_finished());
    ASSERT_TRUE(number_of_errors == errors::get_error_count());
}

TEST_F(SpriteTest, SharedTextureSpritesKeepIndependentGeometry)
{
    auto number_of_errors = errors::get_error_count();

    Sprite sprite_1{mock_texture_, 100, 100, {10.0F, 20.0F}, {0.0F, 0.0F}};
    Sprite sprite_2{mock_texture_, 64, 32, {200.0F, 100.0F}, {5.0F, 10.0F}};

    sprite_1.draw();
    const auto sprite_1_vertices_before = read_bound_sprite_vertices();

    sprite_2.draw();
    const auto sprite_2_vertices = read_bound_sprite_vertices();

    sprite_1.draw();
    const auto sprite_1_vertices_after = read_bound_sprite_vertices();

    EXPECT_NE(sprite_1_vertices_before, sprite_2_vertices);
    EXPECT_EQ(sprite_1_vertices_before, sprite_1_vertices_after);
    ASSERT_TRUE(number_of_errors == errors::get_error_count());
}
