/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <gtest/gtest.h>

#include "core/include/object.h"
#include "platformers/include/world.h"
#include "tests/include/world_test.h"

using namespace rinvid;

// Mock collision resolver
std::int32_t resolver_call_count = 0;

bool mock_resolver(Object&, Object&)
{
    ++resolver_call_count;
    return true;
}

/* ------------------------------------------------------------
 * Object vs Object
 * ------------------------------------------------------------ */

TEST_F(WorldTest, ObjectObject_Collides)
{
    bool result = World::collide(a, b, mock_resolver);

    EXPECT_TRUE(result);
    EXPECT_EQ(resolver_call_count, 1);
}

TEST_F(WorldTest, ObjectObject_NoCollision)
{
    bool result = World::collide(a, c, mock_resolver);

    EXPECT_FALSE(result);
    EXPECT_EQ(resolver_call_count, 0);
}

/* ------------------------------------------------------------
 * Object vs Group
 * ------------------------------------------------------------ */

TEST_F(WorldTest, ObjectGroup_OneCollision)
{
    std::vector<Object*> group{&b, &c};

    bool result = World::collide(a, group, mock_resolver);

    EXPECT_TRUE(result);
    EXPECT_EQ(resolver_call_count, 1);
}

TEST_F(WorldTest, ObjectGroup_TwoCollisions)
{
    std::vector<Object*> group{&a, &b, &c};

    bool result = World::collide(ab, group, mock_resolver);

    EXPECT_TRUE(result);
    EXPECT_EQ(resolver_call_count, 2);
}

TEST_F(WorldTest, ObjectGroup_NoCollision)
{
    std::vector<Object*> group{&c, &d};

    bool result = World::collide(a, group, mock_resolver);

    EXPECT_FALSE(result);
    EXPECT_EQ(resolver_call_count, 0);
}

TEST_F(WorldTest, ObjectGroup_SkipsSelf)
{
    std::vector<Object*> group{&a};

    bool result = World::collide(a, group, mock_resolver);

    EXPECT_FALSE(result);
    EXPECT_EQ(resolver_call_count, 0);
}

/* ------------------------------------------------------------
 * Group vs Object (delegates to Object vs Group)
 * ------------------------------------------------------------ */

TEST_F(WorldTest, GroupObject_TwoCollisions)
{
    std::vector<Object*> group{&a, &b, &c, &d};

    bool result = World::collide(group, ab, mock_resolver);

    EXPECT_TRUE(result);
    EXPECT_EQ(resolver_call_count, 2);
}

/* ------------------------------------------------------------
 * Group vs Group
 * ------------------------------------------------------------ */

TEST_F(WorldTest, GroupGroup_TwoCollisions)
{
    std::vector<Object*> group1{&a, &b};
    std::vector<Object*> group2{&ab, &c, &d};

    bool result = World::collide(group1, group2, mock_resolver);

    EXPECT_TRUE(result);
    EXPECT_EQ(resolver_call_count, 2);
}

TEST_F(WorldTest, GroupGroup_NoCollisions)
{
    std::vector<Object*> group1{&a, &b};
    std::vector<Object*> group2{&c, &d};

    bool result = World::collide(group1, group2, mock_resolver);

    EXPECT_FALSE(result);
    EXPECT_EQ(resolver_call_count, 0);
}

TEST_F(WorldTest, GroupGroup_SkipsSameObjects_NoCollisions)
{
    std::vector<Object*> group1{&a, &c};
    std::vector<Object*> group2{&a, &c, &d};

    bool result = World::collide(group1, group2, mock_resolver);

    EXPECT_FALSE(result);
    EXPECT_EQ(resolver_call_count, 0);
}

TEST_F(WorldTest, GroupGroup_IdenticalGroups_SixCollisions)
{
    std::vector<Object*> group1{&a, &b, &ab, &c, &d};

    bool result = World::collide(group1, group1, mock_resolver);

    EXPECT_TRUE(result);
    EXPECT_EQ(resolver_call_count, 6);
}
