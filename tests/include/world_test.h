/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef TESTS_INCLUDE_WORLD_TEST_H
#define TESTS_INCLUDE_WORLD_TEST_H

#include <gtest/gtest.h>

#include <rinvid/platformers/object.h>

using namespace rinvid;

extern std::int32_t resolver_call_count;

class WorldTest : public ::testing::Test
{
  protected:
    Object object_a_;
    Object object_b_;
    Object object_ab_;
    Object object_c_;
    Object object_d_;

    void SetUp() override
    {
        resolver_call_count = 0;

        // Object A
        object_a_.reset(Vector2f{0.0F, 0.0F});
        object_a_.resize(10.0F, 10.0F);

        // Object B (overlaps with A)
        object_b_.reset(Vector2f{5.0F, 5.0F});
        object_b_.resize(10.0F, 10.0F);

        // Object AB (overlaps with A and b)
        object_ab_.reset(Vector2f{2.5F, 2.5F});
        object_ab_.resize(10.0F, 10.0F);

        // Object C (no overlap)
        object_c_.reset(Vector2f{100.0F, 100.0F});
        object_c_.resize(10.0F, 10.0F);

        // Object D (no overlap)
        object_d_.reset(Vector2f{200.0F, 200.0F});
        object_d_.resize(10.0F, 10.0F);
    }
};

#endif // TESTS_INCLUDE_WORLD_TEST_H
