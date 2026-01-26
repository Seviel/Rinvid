/**********************************************************************
 * Copyright (c) 2023 - 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <gtest/gtest.h>

#include "core/include/application.h"
#include "core/include/texture.h"
#include "include/texture_test.h"
#include "util/include/error_handler.h"

using namespace rinvid;

// Test the constructor with a valid image file
TEST_F(TextureTest, Constructor_ValidImageFile)
{
    auto number_of_errors = errors::get_error_count();

    const char* file_name = "resources/valid_image.png";
    Texture     texture{file_name};

    // Check that the number of errors did not increase.
    ASSERT_TRUE(number_of_errors == errors::get_error_count());
}

// Test the constructor with an invalid image file
TEST_F(TextureTest, Constructor_InvalidImageFile)
{
#ifndef RINVID_DEBUG_MODE
    GTEST_SKIP();
#endif
    const char* file_name = "invalid_image.png";
    Texture     texture(file_name);

    std::string error{"invalid_image.png image loading failed during texture creation"};
    ASSERT_TRUE(errors::has_error_occured(error));
}

// Test the move constructor
TEST_F(TextureTest, MoveConstructor)
{
    auto number_of_errors = errors::get_error_count();

    Texture texture1{"resources/valid_image.png"};
    Texture texture2{std::move(texture1)};

    // Check that the number of errors did not increase.
    ASSERT_TRUE(number_of_errors == errors::get_error_count());
}

// Test the move assignment operator
TEST_F(TextureTest, MoveAssignmentOperator)
{
    auto number_of_errors = errors::get_error_count();

    Texture texture1("resources/valid_image.png");
    Texture texture2 = std::move(texture1);

    // Check that the number of errors did not increase.
    ASSERT_TRUE(number_of_errors == errors::get_error_count());
}

// Test the destructor
TEST_F(TextureTest, Destructor)
{
    auto number_of_errors = errors::get_error_count();

    Texture* texture = new Texture("resources/valid_image.png");
    delete texture;

    // Check that the number of errors did not increase.
    ASSERT_TRUE(number_of_errors == errors::get_error_count());
}
