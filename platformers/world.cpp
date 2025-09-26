/**********************************************************************
 * Copyright (c) 2025, Filip Vasiljevic
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

constexpr float OVERLAP_BIAS = 1.0F;

float World::gravity{800.0F};

bool World::collide(Object& object_1, Object& object_2)
{
    if (intersects(object_1.bounding_rect(), object_2.bounding_rect()))
    {
        if (separate(object_1, object_2))
        {
            return true;
        }
    }

    return false;
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
                if (overlap > max_overlap)
                {
                    overlap = 0.0F;
                }
                else
                {
                    object_1.touching_ =
                        static_cast<Direction>(object_1.touching_ | Direction::Right);
                    object_2.touching_ =
                        static_cast<Direction>(object_2.touching_ | Direction::Left);
                }
            }
            else if (obj1_delta < obj2_delta)
            {
                overlap = object_1.position_.x - object_2.width_ - object_2.position_.x;
                if (-overlap > max_overlap)
                {
                    overlap = 0.0F;
                }
                else
                {
                    object_1.touching_ =
                        static_cast<Direction>(object_1.touching_ | Direction::Right);
                    object_2.touching_ =
                        static_cast<Direction>(object_2.touching_ | Direction::Left);
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
                if (overlap > max_overlap)
                {
                    overlap = 0.0F;
                }
                else
                {
                    object_1.touching_ =
                        static_cast<Direction>(object_1.touching_ | Direction::Down);
                    object_2.touching_ = static_cast<Direction>(object_2.touching_ | Direction::Up);
                }
            }
            else if (obj1_delta < obj2_delta)
            {
                overlap = object_1.position_.y - object_2.height_ - object_2.position_.y;
                if (-overlap > max_overlap)
                {
                    overlap = 0.0F;
                }
                else
                {
                    object_1.touching_ = static_cast<Direction>(object_1.touching_ | Direction::Up);
                    object_2.touching_ =
                        static_cast<Direction>(object_2.touching_ | Direction::Down);
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
