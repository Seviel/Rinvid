/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <stdexcept>
#include <string>
#include <type_traits>

#include <gtest/gtest.h>

#include <rinvid/util/error.h>
#include <rinvid/util/error_handler.h>

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

TEST(ErrorTest, ErrorLogRecordsUniqueMessages)
{
    rinvid::errors::clear_errors();

    rinvid::errors::put_error_to_log("duplicate diagnostic");
    rinvid::errors::put_error_to_log(std::string{"duplicate diagnostic"});

    EXPECT_EQ(1U, rinvid::errors::get_error_count());
    EXPECT_TRUE(rinvid::errors::has_error_occurred("duplicate diagnostic"));

    rinvid::errors::clear_errors();
    EXPECT_EQ(0U, rinvid::errors::get_error_count());
}

TEST(ErrorTest, ErrorLogAcceptsNullDescription)
{
    rinvid::errors::clear_errors();

    const char* description{nullptr};
    rinvid::errors::put_error_to_log(description);

    EXPECT_TRUE(rinvid::errors::has_error_occurred("Rinvid error: null error description"));

    rinvid::errors::clear_errors();
}
