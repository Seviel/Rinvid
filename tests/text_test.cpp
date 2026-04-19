/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <string>

#include <gtest/gtest.h>

#include "core/include/text.h"
#include "core/include/ttf_lib.h"
#include "tests/include/opengl_test.h"

namespace
{

std::string get_font_path()
{
    std::string file_path{__FILE__};
    const auto  last_separator = file_path.find_last_of("/\\");
    file_path.resize(last_separator);

    const auto tests_separator = file_path.find_last_of("/\\");
    file_path.resize(tests_separator);

    return file_path + "/examples/full_demo/resources/aquifer.ttf";
}

} // namespace

TEST_F(OpenGLTest, TextDestroy_IsSafeAfterExplicitTTFLibDestroyRequest)
{
    const auto font_path = get_font_path();

    auto* text =
        new rinvid::Text{"Hello", font_path, {0.0F, 0.0F}, rinvid::Color{255, 255, 255, 255}, 18U};

    rinvid::TTFLib::destroy();

    EXPECT_NO_THROW(delete text);
    EXPECT_NO_THROW(rinvid::TTFLib::destroy());
}

TEST_F(OpenGLTest, TextSetSize_RecreatesGlyphResources)
{
    const auto font_path = get_font_path();

    rinvid::Text text{"Hello", font_path, {0.0F, 0.0F}, rinvid::Color{255, 255, 255, 255}, 18U};

    EXPECT_NO_THROW(text.set_size(24U));
    EXPECT_NO_THROW(text.set_size(12U));

    rinvid::TTFLib::destroy();
}
