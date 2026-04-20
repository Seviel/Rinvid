/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <gtest/gtest.h>

#include <rinvid/core/object.h>
#include <rinvid/platformers/world.h>

#include "include/world_test.h"

using namespace rinvid;

// Mock collision resolver
std::int32_t resolver_call_count = 0;

bool mock_resolver(Object&, Object&)
{
    ++resolver_call_count;
    return true;
}

bool false_resolver(Object&, Object&)
{
    ++resolver_call_count;
    return false;
}

static void disable_physics_side_effects(Object& object)
{
    object.set_drag({0.0F, 0.0F});
    object.set_acceleration({0.0F, 0.0F});
    object.set_gravity_scale(0.0F);
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

TEST_F(WorldTest, ObjectObject_ResolverReturnsFalse)
{
    bool result = World::collide(a, b, false_resolver);

    EXPECT_FALSE(result);
    EXPECT_EQ(resolver_call_count, 1);
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

/* ------------------------------------------------------------
 * Separation
 * ------------------------------------------------------------ */

TEST_F(WorldTest, SeparateX_BothMovable_SplitsOverlapAndAveragesVelocity)
{
    a.reset({0.0F, 0.0F});
    a.resize(10.0F, 10.0F);
    disable_physics_side_effects(a);
    a.set_velocity({5.0F, 0.0F});
    a.update(1.0);

    b.reset({12.0F, 0.0F});
    b.resize(10.0F, 10.0F);
    disable_physics_side_effects(b);
    b.set_velocity({-5.0F, 0.0F});
    b.update(1.0);

    EXPECT_TRUE(World::separate(a, b));

    EXPECT_FLOAT_EQ(a.get_position().x, 1.0F);
    EXPECT_FLOAT_EQ(b.get_position().x, 11.0F);
    EXPECT_FLOAT_EQ(a.get_x_velocity(), 0.0F);
    EXPECT_FLOAT_EQ(b.get_x_velocity(), 0.0F);
    EXPECT_TRUE(a.is_touching(RIGHT));
    EXPECT_TRUE(b.is_touching(LEFT));
}

TEST_F(WorldTest, SeparateY_StaticObject_MovesOnlyMovableObject)
{
    a.reset({0.0F, 0.0F});
    a.resize(10.0F, 10.0F);
    disable_physics_side_effects(a);
    a.set_velocity({0.0F, 10.0F});
    a.update(1.0);

    b.reset({0.0F, 15.0F});
    b.resize(10.0F, 10.0F);
    disable_physics_side_effects(b);
    b.set_movable(NOT);
    b.set_velocity({0.0F, 3.0F});
    b.update(1.0);

    EXPECT_TRUE(World::separate(a, b));

    EXPECT_FLOAT_EQ(a.get_position().y, 5.0F);
    EXPECT_FLOAT_EQ(b.get_position().y, 15.0F);
    EXPECT_FLOAT_EQ(a.get_y_velocity(), 3.0F);
    EXPECT_FLOAT_EQ(b.get_y_velocity(), 3.0F);
    EXPECT_TRUE(a.is_touching(DOWN));
    EXPECT_TRUE(b.is_touching(UP));
}

TEST_F(WorldTest, SeparateX_RespectsAllowedCollisions)
{
    a.reset({0.0F, 0.0F});
    a.resize(10.0F, 10.0F);
    disable_physics_side_effects(a);
    a.set_allowed_collisions(LEFT | UP | DOWN);
    a.set_velocity({5.0F, 0.0F});
    a.update(1.0);

    b.reset({12.0F, 0.0F});
    b.resize(10.0F, 10.0F);
    disable_physics_side_effects(b);
    b.set_velocity({-5.0F, 0.0F});
    b.update(1.0);

    EXPECT_FALSE(World::separate(a, b));

    EXPECT_FLOAT_EQ(a.get_position().x, 5.0F);
    EXPECT_FLOAT_EQ(b.get_position().x, 7.0F);
    EXPECT_FALSE(a.is_touching(RIGHT));
    EXPECT_FALSE(b.is_touching(LEFT));
}

TEST_F(WorldTest, Collide_DoesNotSeparateWhenObjectsMoveTogether)
{
    a.reset({0.0F, 0.0F});
    a.resize(10.0F, 10.0F);
    disable_physics_side_effects(a);
    a.set_velocity({5.0F, 0.0F});
    a.update(1.0);

    b.reset({8.0F, 0.0F});
    b.resize(10.0F, 10.0F);
    disable_physics_side_effects(b);
    b.set_velocity({5.0F, 0.0F});
    b.update(1.0);

    EXPECT_FALSE(World::collide(a, b));

    EXPECT_FLOAT_EQ(a.get_position().x, 5.0F);
    EXPECT_FLOAT_EQ(b.get_position().x, 13.0F);
    EXPECT_FALSE(a.is_touching(RIGHT));
    EXPECT_FALSE(b.is_touching(LEFT));
}
