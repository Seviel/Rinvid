/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <gtest/gtest.h>

#include "include/world_test.h"
#include <rinvid/platformers/object.h>
#include <rinvid/platformers/world.h>

using namespace rinvid;

// Mock collision resolver
std::int32_t resolver_call_count{0};

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
    bool result{World::collide(object_a_, object_b_, mock_resolver)};

    EXPECT_TRUE(result);
    EXPECT_EQ(resolver_call_count, 1);
}

TEST_F(WorldTest, ObjectObject_NoCollision)
{
    bool result{World::collide(object_a_, object_c_, mock_resolver)};

    EXPECT_FALSE(result);
    EXPECT_EQ(resolver_call_count, 0);
}

TEST_F(WorldTest, ObjectObject_ResolverReturnsFalse)
{
    bool result{World::collide(object_a_, object_b_, false_resolver)};

    EXPECT_FALSE(result);
    EXPECT_EQ(resolver_call_count, 1);
}

/* ------------------------------------------------------------
 * Object vs Group
 * ------------------------------------------------------------ */

TEST_F(WorldTest, ObjectGroup_OneCollision)
{
    std::vector<Object*> group{&object_b_, &object_c_};

    bool result{World::collide(object_a_, group, mock_resolver)};

    EXPECT_TRUE(result);
    EXPECT_EQ(resolver_call_count, 1);
}

TEST_F(WorldTest, ObjectGroup_TwoCollisions)
{
    std::vector<Object*> group{&object_a_, &object_b_, &object_c_};

    bool result{World::collide(object_ab_, group, mock_resolver)};

    EXPECT_TRUE(result);
    EXPECT_EQ(resolver_call_count, 2);
}

TEST_F(WorldTest, ObjectGroup_NoCollision)
{
    std::vector<Object*> group{&object_c_, &object_d_};

    bool result{World::collide(object_a_, group, mock_resolver)};

    EXPECT_FALSE(result);
    EXPECT_EQ(resolver_call_count, 0);
}

TEST_F(WorldTest, ObjectGroup_SkipsSelf)
{
    std::vector<Object*> group{&object_a_};

    bool result{World::collide(object_a_, group, mock_resolver)};

    EXPECT_FALSE(result);
    EXPECT_EQ(resolver_call_count, 0);
}

/* ------------------------------------------------------------
 * Group vs Object (delegates to Object vs Group)
 * ------------------------------------------------------------ */

TEST_F(WorldTest, GroupObject_TwoCollisions)
{
    std::vector<Object*> group{&object_a_, &object_b_, &object_c_, &object_d_};

    bool result{World::collide(group, object_ab_, mock_resolver)};

    EXPECT_TRUE(result);
    EXPECT_EQ(resolver_call_count, 2);
}

/* ------------------------------------------------------------
 * Group vs Group
 * ------------------------------------------------------------ */

TEST_F(WorldTest, GroupGroup_TwoCollisions)
{
    std::vector<Object*> group1{&object_a_, &object_b_};
    std::vector<Object*> group2{&object_ab_, &object_c_, &object_d_};

    bool result{World::collide(group1, group2, mock_resolver)};

    EXPECT_TRUE(result);
    EXPECT_EQ(resolver_call_count, 2);
}

TEST_F(WorldTest, GroupGroup_NoCollisions)
{
    std::vector<Object*> group1{&object_a_, &object_b_};
    std::vector<Object*> group2{&object_c_, &object_d_};

    bool result{World::collide(group1, group2, mock_resolver)};

    EXPECT_FALSE(result);
    EXPECT_EQ(resolver_call_count, 0);
}

TEST_F(WorldTest, GroupGroup_SkipsSameObjects_NoCollisions)
{
    std::vector<Object*> group1{&object_a_, &object_c_};
    std::vector<Object*> group2{&object_a_, &object_c_, &object_d_};

    bool result{World::collide(group1, group2, mock_resolver)};

    EXPECT_FALSE(result);
    EXPECT_EQ(resolver_call_count, 0);
}

TEST_F(WorldTest, GroupGroup_IdenticalGroups_SixCollisions)
{
    std::vector<Object*> group1{&object_a_, &object_b_, &object_ab_, &object_c_, &object_d_};

    bool result{World::collide(group1, group1, mock_resolver)};

    EXPECT_TRUE(result);
    EXPECT_EQ(resolver_call_count, 6);
}

/* ------------------------------------------------------------
 * Separation
 * ------------------------------------------------------------ */

TEST_F(WorldTest, SeparateX_BothMovable_SplitsOverlapAndAveragesVelocity)
{
    object_a_.reset({0.0F, 0.0F});
    object_a_.resize(10.0F, 10.0F);
    disable_physics_side_effects(object_a_);
    object_a_.set_velocity({5.0F, 0.0F});
    object_a_.update(1.0);

    object_b_.reset({12.0F, 0.0F});
    object_b_.resize(10.0F, 10.0F);
    disable_physics_side_effects(object_b_);
    object_b_.set_velocity({-5.0F, 0.0F});
    object_b_.update(1.0);

    EXPECT_TRUE(World::separate(object_a_, object_b_));

    EXPECT_FLOAT_EQ(object_a_.get_position().x, 1.0F);
    EXPECT_FLOAT_EQ(object_b_.get_position().x, 11.0F);
    EXPECT_FLOAT_EQ(object_a_.get_x_velocity(), 0.0F);
    EXPECT_FLOAT_EQ(object_b_.get_x_velocity(), 0.0F);
    EXPECT_TRUE(object_a_.is_touching(RIGHT));
    EXPECT_TRUE(object_b_.is_touching(LEFT));
}

TEST_F(WorldTest, SeparateY_StaticObject_MovesOnlyMovableObject)
{
    object_a_.reset({0.0F, 0.0F});
    object_a_.resize(10.0F, 10.0F);
    disable_physics_side_effects(object_a_);
    object_a_.set_velocity({0.0F, 10.0F});
    object_a_.update(1.0);

    object_b_.reset({0.0F, 15.0F});
    object_b_.resize(10.0F, 10.0F);
    disable_physics_side_effects(object_b_);
    object_b_.set_movable(NOT);
    object_b_.set_velocity({0.0F, 3.0F});
    object_b_.update(1.0);

    EXPECT_TRUE(World::separate(object_a_, object_b_));

    EXPECT_FLOAT_EQ(object_a_.get_position().y, 5.0F);
    EXPECT_FLOAT_EQ(object_b_.get_position().y, 15.0F);
    EXPECT_FLOAT_EQ(object_a_.get_y_velocity(), 3.0F);
    EXPECT_FLOAT_EQ(object_b_.get_y_velocity(), 3.0F);
    EXPECT_TRUE(object_a_.is_touching(DOWN));
    EXPECT_TRUE(object_b_.is_touching(UP));
}

TEST_F(WorldTest, SeparateX_RespectsAllowedCollisions)
{
    object_a_.reset({0.0F, 0.0F});
    object_a_.resize(10.0F, 10.0F);
    disable_physics_side_effects(object_a_);
    object_a_.set_allowed_collisions(LEFT | UP | DOWN);
    object_a_.set_velocity({5.0F, 0.0F});
    object_a_.update(1.0);

    object_b_.reset({12.0F, 0.0F});
    object_b_.resize(10.0F, 10.0F);
    disable_physics_side_effects(object_b_);
    object_b_.set_velocity({-5.0F, 0.0F});
    object_b_.update(1.0);

    EXPECT_FALSE(World::separate(object_a_, object_b_));

    EXPECT_FLOAT_EQ(object_a_.get_position().x, 5.0F);
    EXPECT_FLOAT_EQ(object_b_.get_position().x, 7.0F);
    EXPECT_FALSE(object_a_.is_touching(RIGHT));
    EXPECT_FALSE(object_b_.is_touching(LEFT));
}

TEST_F(WorldTest, Collide_DoesNotSeparateWhenObjectsMoveTogether)
{
    object_a_.reset({0.0F, 0.0F});
    object_a_.resize(10.0F, 10.0F);
    disable_physics_side_effects(object_a_);
    object_a_.set_velocity({5.0F, 0.0F});
    object_a_.update(1.0);

    object_b_.reset({8.0F, 0.0F});
    object_b_.resize(10.0F, 10.0F);
    disable_physics_side_effects(object_b_);
    object_b_.set_velocity({5.0F, 0.0F});
    object_b_.update(1.0);

    EXPECT_FALSE(World::collide(object_a_, object_b_));

    EXPECT_FLOAT_EQ(object_a_.get_position().x, 5.0F);
    EXPECT_FLOAT_EQ(object_b_.get_position().x, 13.0F);
    EXPECT_FALSE(object_a_.is_touching(RIGHT));
    EXPECT_FALSE(object_b_.is_touching(LEFT));
}
