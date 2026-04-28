/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef TESTS_INCLUDE_OPENGL_TEST_H
#define TESTS_INCLUDE_OPENGL_TEST_H

#include <cstdlib>

#include <SFML/Window/Context.hpp>
#include <gtest/gtest.h>

#include <rinvid/core/render_context.h>
#include <rinvid/core/rinvid_gl.h>
#include <rinvid/core/ttf_lib.h>

class OpenGLTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
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

#ifdef _WIN32
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

        render_context_.set_viewport(0, 0, 32, 32);
        render_context_.init(nullptr);
    }

    void TearDown() override
    {
        render_context_.shutdown();
        rinvid::TTFLib::destroy();
    }

    rinvid::RenderContext render_context_{};

  private:
    sf::Context context_{};
};

#endif // TESTS_INCLUDE_OPENGL_TEST_H
