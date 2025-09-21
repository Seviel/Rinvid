/**********************************************************************
 * Copyright (c) 2024 - 2025, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef TESTS_INCLUDE_SPRITE_TEST_H
#define TESTS_INCLUDE_SPRITE_TEST_H

#include <gtest/gtest.h>

#include <SFML/Window/Context.hpp>

#include "core/include/rinvid_gl.h"
#include "core/include/texture.h"

using namespace rinvid;

class SpriteTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
#ifdef _WIN32
        gladLoadGLLoader(reinterpret_cast<GLADloadproc>(sf::Context::getFunction));
#endif
        Application* application{nullptr};
        RinvidGfx::init(application);

        const char* file_name = {"tests/resources/valid_image.png"};
        mock_texture_         = new Texture{file_name};
    }

    void TearDown() override
    {
        delete mock_texture_;
    }

    Texture* mock_texture_;

  private:
    // Since we don't have/need window in unit tests, we need active OpenGL context to be able to
    // call OpenGL functions. This object provides that, it needs only exist and we can call OpenGL
    // functions.
    sf::Context context_{};
};

#endif // TESTS_INCLUDE_SPRITE_TEST_H
