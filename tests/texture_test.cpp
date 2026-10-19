/**********************************************************************
 * Copyright (c) 2023 - 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <gtest/gtest.h>

#include "include/texture_test.h"
#include <rinvid/core/sprite.h>
#include <rinvid/core/texture.h>
#include <rinvid/util/error.h>
#include <rinvid/util/error_handler.h>

using namespace rinvid;

// Test the constructor with a valid image file
TEST_F(TextureTest, Constructor_ValidImageFile)
{
    auto number_of_errors{errors::get_error_count()};

    const char* file_name{"resources/valid_image.png"};
    Texture     texture{file_name};

    // Check that the number of errors did not increase.
    ASSERT_TRUE(number_of_errors == errors::get_error_count());
}

TEST_F(TextureTest, Constructor_RawPixels)
{
    auto number_of_errors{errors::get_error_count()};

    std::vector<std::uint8_t> image_data{255U, 0U, 0U,   255U, 0U,   255U, 0U,   255U,
                                         0U,   0U, 255U, 255U, 255U, 255U, 255U, 255U};
    Texture                   texture{image_data, 2, 2};
    Sprite                    sprite{&texture, 2, 2, {0.0F, 0.0F}};

    EXPECT_NO_THROW(sprite.draw());

    ASSERT_TRUE(number_of_errors == errors::get_error_count());
}

// Test the constructor with an invalid image file
TEST_F(TextureTest, Constructor_InvalidImageFile)
{
    const char* file_name{"invalid_image.png"};

    EXPECT_THROW(Texture texture{file_name}, ResourceLoadError);
}

TEST_F(TextureTest, Constructor_NullImageFileThrowsResourceLoadError)
{
    const char* file_name{nullptr};

    EXPECT_THROW(Texture texture{file_name}, ResourceLoadError);
}

// Test the move constructor
TEST_F(TextureTest, MoveConstructor)
{
    auto number_of_errors{errors::get_error_count()};

    Texture texture1{"resources/valid_image.png"};
    Texture texture2{std::move(texture1)};

    // Check that the number of errors did not increase.
    ASSERT_TRUE(number_of_errors == errors::get_error_count());
}

// Test the move assignment operator
TEST_F(TextureTest, MoveAssignmentOperator)
{
    auto number_of_errors{errors::get_error_count()};

    Texture texture1("resources/valid_image.png");
    Texture texture2{"resources/valid_image.png"};
    texture2 = std::move(texture1);

    // Check that the number of errors did not increase.
    ASSERT_TRUE(number_of_errors == errors::get_error_count());
}

TEST_F(TextureTest, MovedTextureCanStillBeUsedForDrawing)
{
    auto number_of_errors{errors::get_error_count()};

    Texture texture1{"resources/valid_image.png"};
    Texture texture2{"resources/valid_image.png"};
    texture2 = std::move(texture1);

    Sprite sprite{&texture2, 1, 1, {0.0F, 0.0F}};

    EXPECT_NO_THROW(sprite.draw());

    ASSERT_TRUE(number_of_errors == errors::get_error_count());
}

// Test the destructor
TEST_F(TextureTest, Destructor)
{
    auto number_of_errors{errors::get_error_count()};

    Texture* texture{new Texture("resources/valid_image.png")};
    delete texture;

    // Check that the number of errors did not increase.
    ASSERT_TRUE(number_of_errors == errors::get_error_count());
}

TEST_F(TextureTest, DefaultFilteringOnlyAffectsNewTextures)
{
    // Restore the startup default even if texture construction throws.
    struct ResetDefault
    {
        ~ResetDefault()
        {
            Texture::set_default_smooth(true);
        }
    } reset_default;

    const auto expect_filter = [](GLint expected)
    {
        GLint filter{};
        glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, &filter);
        EXPECT_EQ(filter, expected);
        glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, &filter);
        EXPECT_EQ(filter, expected);
    };

    Texture original{"resources/valid_image.png"};
    expect_filter(GL_LINEAR);
    Texture::set_default_smooth(false);
    expect_filter(GL_LINEAR);

    Texture from_file{"resources/valid_image.png"};
    expect_filter(GL_NEAREST);
    from_file.set_smooth(true);
    expect_filter(GL_LINEAR);

    Texture from_pixels{std::vector<std::uint8_t>{255, 0, 0, 255}, 1, 1};
    expect_filter(GL_NEAREST);
    Texture::set_default_smooth(true);
    expect_filter(GL_NEAREST);

    Texture next{"resources/valid_image.png"};
    expect_filter(GL_LINEAR);
}
