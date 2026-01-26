/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef TESTS_INCLUDE_UTIL_TEST_H
#define TESTS_INCLUDE_UTIL_TEST_H

#include <gtest/gtest.h>

#include "core/include/object.h"

using namespace rinvid;

extern std::int32_t resolver_call_count;

class WorldTest : public ::testing::Test
{
  protected:
    Object a;
    Object b;
    Object ab;
    Object c;
    Object d;

    void SetUp() override
    {
        resolver_call_count = 0;

        // Object A
        a.reset(Vector2f{0.0F, 0.0F});
        a.resize(10.0F, 10.0F);

        // Object B (overlaps with A)
        b.reset(Vector2f{5.0F, 5.0F});
        b.resize(10.0F, 10.0F);

        // Object AB (overlaps with A and b)
        ab.reset(Vector2f{2.5F, 2.5F});
        ab.resize(10.0F, 10.0F);

        // Object C (no overlap)
        c.reset(Vector2f{100.0F, 100.0F});
        c.resize(10.0F, 10.0F);

        // Object D (no overlap)
        d.reset(Vector2f{200.0F, 200.0F});
        d.resize(10.0F, 10.0F);
    }
};

#endif // TESTS_INCLUDE_UTIL_TEST_H
