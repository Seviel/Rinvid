/**********************************************************************
 * Copyright (c) 2023, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <SFML/Window/Context.hpp>
#include <gtest/gtest.h>

#include "core/include/rinvid_gl.h"

class TextureTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
#ifdef _WIN32
        gladLoadGLLoader(reinterpret_cast<GLADloadproc>(sf::Context::getFunction));
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
