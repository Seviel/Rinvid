/**********************************************************************
 * Copyright (c) 2025 - 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include "include/world.h"
#include "core/include/object.h"
#include "util/include/collision_detection.h"

namespace rinvid
{

static constexpr float DEFAULT_GRAVITY      = 800.0F;
static constexpr float DEFAULT_OVERLAP_BIAS = 2.0F;

static float OVERLAP_BIAS = DEFAULT_OVERLAP_BIAS;

float World::gravity{DEFAULT_GRAVITY};

void World::set_gravity(float gravity)
{
    World::gravity = gravity;

    std::int32_t overlap_bias_factor = static_cast<std::int32_t>(gravity / DEFAULT_GRAVITY);
    OVERLAP_BIAS = static_cast<std::int32_t>(DEFAULT_OVERLAP_BIAS) << overlap_bias_factor;
}

bool World::collide(Object& object_1, Object& object_2, CollisionResolver resolve)
{
    if (intersects(object_1.bounding_rect(), object_2.bounding_rect()))
    {
        if (resolve(object_1, object_2))
        {
            return true;
        }
    }

    return false;
}

bool World::collide(Object& object, const std::vector<Object*>& group, CollisionResolver resolve)
{
    bool result = false;

    for (auto* group_object : group)
    {
        if (group_object == &object)
        {
            continue;
        }

        result |= World::collide(object, *group_object, resolve);
    }

    return result;
}

bool World::collide(const std::vector<Object*>& group, Object& object, CollisionResolver resolve)
{
    return World::collide(object, group, resolve);
}

/// @todo Think about optimizing this if it gets heavily used with many objects in the future.
bool World::collide(const std::vector<Object*>& group_1, const std::vector<Object*>& group_2,
                    CollisionResolver resolve)
{
    bool result = false;

    for (auto* group_1_object : group_1)
    {
        for (auto* group_2_object : group_2)
        {
            if (group_1_object == group_2_object)
            {
                continue;
            }

            result |= World::collide(*group_1_object, *group_2_object, resolve);
        }
    }

    return result;
}

bool World::separate(Object& object_1, Object& object_2)
{
    bool x_separated = separate_x(object_1, object_2);
    bool y_separated = separate_y(object_1, object_2);

    return x_separated || y_separated;
}

bool World::separate_x(Object& object_1, Object& object_2)
{
    if (!object_1.movable_ && !object_2.movable_)
    {
        return false;
    }

    float overlap{0.0F};
    float obj1_delta = object_1.position_.x - object_1.previous_position_.x;
    float obj2_delta = object_2.position_.x - object_2.previous_position_.x;

    if (obj1_delta != obj2_delta)
    {
        // Check if there is actually overlap on x axis
        float obj1_delta_abs = (obj1_delta > 0.0F) ? obj1_delta : -obj1_delta;
        float obj2_delta_abs = (obj2_delta > 0.0F) ? obj2_delta : -obj2_delta;
        Rect  obj1_rect =
            Rect{Vector2f{object_1.position_.x - ((obj1_delta > 0.0F) ? obj1_delta : 0.0F),
                          object_1.previous_position_.y},
                 object_1.width_ + static_cast<std::int32_t>(obj1_delta_abs), object_1.height_};
        Rect obj2_rect =
            Rect{Vector2f{object_2.position_.x - ((obj2_delta > 0.0F) ? obj2_delta : 0.0F),
                          object_2.previous_position_.y},
                 object_2.width_ + static_cast<std::int32_t>(obj2_delta_abs), object_2.height_};

        if (intersects(obj1_rect, obj2_rect))
        {
            float max_overlap = obj1_delta_abs + obj2_delta_abs + OVERLAP_BIAS;

            if (obj1_delta > obj2_delta)
            {
                overlap = object_1.position_.x + object_1.width_ - object_2.position_.x;
                if ((overlap > max_overlap) || !(object_1.allowed_collisions_ & RIGHT) ||
                    !(object_2.allowed_collisions_ & LEFT))
                {
                    overlap = 0.0F;
                }
                else
                {
                    object_1.touching_ = (object_1.touching_ | RIGHT);
                    object_2.touching_ = (object_2.touching_ | LEFT);
                }
            }
            else if (obj1_delta < obj2_delta)
            {
                overlap = object_1.position_.x - object_2.width_ - object_2.position_.x;
                if ((-overlap > max_overlap) || !(object_1.allowed_collisions_ & LEFT) ||
                    !(object_2.allowed_collisions_ & RIGHT))
                {
                    overlap = 0.0F;
                }
                else
                {
                    object_1.touching_ = (object_1.touching_ | RIGHT);
                    object_2.touching_ = (object_2.touching_ | LEFT);
                }
            }
        }
    }

    // Adjust positions and velocities
    if (overlap != 0.0F)
    {
        float obj1_velocity = object_1.velocity_.x;
        float obj2_velocity = object_2.velocity_.x;

        if (object_1.movable_ && object_2.movable_)
        {
            overlap *= 0.5F;
            object_1.position_.x -= overlap;
            object_2.position_.x += overlap;

            float average = (obj1_velocity + obj2_velocity) * 0.5F;

            object_1.velocity_.x = average;
            object_2.velocity_.x = average;
        }
        else if (object_1.movable_)
        {
            object_1.position_.x -= overlap;
            object_1.velocity_.x = obj2_velocity;
        }
        else if (object_2.movable_)
        {
            object_2.position_.x += overlap;
            object_2.velocity_.x = obj1_velocity;
        }

        return true;
    }

    return false;
}

bool World::separate_y(Object& object_1, Object& object_2)
{
    if (!object_1.movable_ && !object_2.movable_)
    {
        return false;
    }

    float overlap{0.0F};
    float obj1_delta = object_1.position_.y - object_1.previous_position_.y;
    float obj2_delta = object_2.position_.y - object_2.previous_position_.y;

    if (obj1_delta != obj2_delta)
    {
        // Check if there is actually overlap on x axis
        float obj1_delta_abs = (obj1_delta > 0.0F) ? obj1_delta : -obj1_delta;
        float obj2_delta_abs = (obj2_delta > 0.0F) ? obj2_delta : -obj2_delta;
        Rect  obj1_rect =
            Rect{Vector2f{object_1.position_.x,
                          object_1.position_.y - ((obj1_delta > 0.0F) ? obj1_delta : 0.0F)},
                 object_1.width_, object_1.height_ + static_cast<std::int32_t>(obj1_delta_abs)};
        Rect obj2_rect =
            Rect{Vector2f{object_2.position_.x,
                          object_2.position_.y - ((obj2_delta > 0.0F) ? obj2_delta : 0.0F)},
                 object_2.width_, object_2.height_ + static_cast<std::int32_t>(obj2_delta_abs)};

        if (intersects(obj1_rect, obj2_rect))
        {
            float max_overlap = obj1_delta_abs + obj2_delta_abs + OVERLAP_BIAS;

            if (obj1_delta > obj2_delta)
            {
                overlap = object_1.position_.y + object_1.height_ - object_2.position_.y;
                if ((overlap > max_overlap) || !(object_1.allowed_collisions_ & DOWN) ||
                    !(object_2.allowed_collisions_ & UP))
                {
                    overlap = 0.0F;
                }
                else
                {
                    object_1.touching_ = (object_1.touching_ | DOWN);
                    object_2.touching_ = (object_2.touching_ | UP);
                }
            }
            else if (obj1_delta < obj2_delta)
            {
                overlap = object_1.position_.y - object_2.height_ - object_2.position_.y;
                if ((-overlap > max_overlap) || !(object_1.allowed_collisions_ & UP) ||
                    !(object_2.allowed_collisions_ & DOWN))
                {
                    overlap = 0.0F;
                }
                else
                {
                    object_1.touching_ = (object_1.touching_ | UP);
                    object_2.touching_ = (object_2.touching_ | DOWN);
                }
            }
        }
    }

    // Adjust positions and velocities
    if (overlap != 0.0F)
    {
        float obj1_velocity = object_1.velocity_.y;
        float obj2_velocity = object_2.velocity_.y;

        if (object_1.movable_ && object_2.movable_)
        {
            overlap *= 0.5F;
            object_1.position_.y -= overlap;
            object_2.position_.y += overlap;

            float average = (obj1_velocity + obj2_velocity) * 0.5F;

            object_1.velocity_.y = average;
            object_2.velocity_.y = average;
        }
        else if (object_1.movable_)
        {
            object_1.position_.y -= overlap;
            object_1.velocity_.y = obj2_velocity;
        }
        else if (object_2.movable_)
        {
            object_2.position_.y += overlap;
            object_2.velocity_.y = obj1_velocity;
        }

        return true;
    }

    return false;
}

} // namespace rinvid
