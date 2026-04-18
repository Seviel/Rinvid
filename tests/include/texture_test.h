/**********************************************************************
 * Copyright (c) 2023 - 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef TESTS_INCLUDE_TEXTURE_TEST_H
#define TESTS_INCLUDE_TEXTURE_TEST_H

#include <cstdlib>

#include <SFML/Window/Context.hpp>
#include <gtest/gtest.h>

#include "core/include/rinvid_gl.h"

class TextureTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
#ifdef _WIN32
        const char* skip_opengl_tests = std::getenv("RINVID_SKIP_OPENGL_TESTS");
        if (skip_opengl_tests != nullptr && skip_opengl_tests[0] != '\0' &&
            skip_opengl_tests[0] != '0')
        {
            GTEST_SKIP() << "OpenGL tests disabled by environment";
        }

        if (!context_.setActive(true))
        {
            GTEST_SKIP() << "OpenGL context activation failed";
        }

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(sf::Context::getFunction)))
        {
            GTEST_SKIP() << "OpenGL function loading failed";
        }

        if (!GLAD_GL_VERSION_3_0 || glad_glGenVertexArrays == nullptr ||
            glad_glBindVertexArray == nullptr)
        {
            GTEST_SKIP() << "Required OpenGL 3.0 functions are unavailable";
        }
#endif
    }

    void TearDown() override
    {
    }

  private:
    // Since we don't have/need window in unit tests, we need active OpenGL context to be able to
    // call OpenGL functions. This object provides that, it needs only exist and we can call OpenGL
    // functions.
    sf::Context context_{};
};

#endif // TESTS_INCLUDE_TEXTURE_TEST_H
