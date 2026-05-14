/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <stdexcept>
#include <type_traits>

#include <gtest/gtest.h>

#include <rinvid/util/error.h>

TEST(ErrorTest, ErrorTypesInheritFromRinvidError)
{
    static_assert(std::is_base_of<std::runtime_error, rinvid::Error>::value,
                  "rinvid::Error must be catchable as std::runtime_error");
    static_assert(std::is_base_of<rinvid::Error, rinvid::ResourceLoadError>::value,
                  "ResourceLoadError must be catchable as rinvid::Error");
    static_assert(std::is_base_of<rinvid::Error, rinvid::GraphicsError>::value,
                  "GraphicsError must be catchable as rinvid::Error");
    static_assert(std::is_base_of<rinvid::Error, rinvid::AudioError>::value,
                  "AudioError must be catchable as rinvid::Error");
}

TEST(ErrorTest, ErrorPreservesDiagnosticMessage)
{
    const rinvid::ResourceLoadError error{"resource failed"};

    EXPECT_STREQ("resource failed", error.what());
}
