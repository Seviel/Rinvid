/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <cstdint>
#include <vector>

#include <gtest/gtest.h>

#include <rinvid/core/light.h>
#include <rinvid/core/rectangle_shape.h>
#include <rinvid/util/error_handler.h>

#include "include/opengl_test.h"

using namespace rinvid;

TEST_F(OpenGLTest, MoreThanLegacyLimitLightsCanBeCreatedAndRendered)
{
    constexpr std::uint32_t light_count{128U};
    const auto              number_of_errors = errors::get_error_count();

    std::vector<Light> lights{};
    lights.reserve(light_count);

    EXPECT_NO_THROW({
        for (std::uint32_t i{0U}; i < light_count; ++i)
        {
            lights.emplace_back(
                Vector2f{static_cast<float>(i % 16U) * 2.0F, static_cast<float>(i / 16U) * 2.0F},
                0.5F, 0.5F);
        }
    });

    RectangleShape rectangle{{16.0F, 16.0F}, 16.0F, 16.0F};

    EXPECT_NO_THROW(rectangle.draw());
    EXPECT_EQ(number_of_errors, errors::get_error_count());
}

TEST_F(OpenGLTest, LightStateAccessorsReflectMutations)
{
    Light light{{4.0F, 8.0F}, 0.5F, 0.5F};

    EXPECT_TRUE(light.is_active());
    EXPECT_FLOAT_EQ(light.get_position().x, 4.0F);
    EXPECT_FLOAT_EQ(light.get_position().y, 8.0F);

    light.set_active(false);
    EXPECT_FALSE(light.is_active());

    light.move({2.0F, 3.0F});
    EXPECT_FLOAT_EQ(light.get_position().x, 6.0F);
    EXPECT_FLOAT_EQ(light.get_position().y, 11.0F);
}
