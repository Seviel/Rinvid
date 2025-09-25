/**********************************************************************
 * Copyright (c) 2025, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include "include/object.h"
#include "platformers/include/world.h"

namespace rinvid
{

Object::Object()
    : position_{0.0F, 0.0F}, previous_position_{0.0F, 0.0F}, velocity_{0.0F, 0.0F},
      acceleration_{0.0F, 0.0F}, drag_{800.0F, 0.0F}, max_velocity_{0.0F}, gravity_scale_{1.0F},
      width_{0}, height_{0}, active_{true}, movable_{true}, collides_{true}
{
}

void Object::update(double delta_time)
{
    if (active_)
    {
        previous_position_.x = position_.x;
        previous_position_.y = position_.y;

        if (movable_)
        {
            update_motion(delta_time);
        }
    }
}

void Object::update_motion(double delta_time)
{
    float delta;
    float velocity_delta;

    velocity_delta =
        (compute_velocity(delta_time, velocity_.x, acceleration_.x, drag_.x, max_velocity_) -
         velocity_.x) /
        2.0F;
    velocity_.x += velocity_delta;
    delta = velocity_.x * delta_time;
    velocity_.x += velocity_delta;
    position_.x += delta;

    velocity_delta =
        (compute_velocity(delta_time, velocity_.y, acceleration_.y, drag_.y, max_velocity_,
                          /* y axis is affected by gravity */ true) -
         velocity_.y) /
        2.0F;
    velocity_.y += velocity_delta;
    delta = velocity_.y * delta_time;
    velocity_.y += velocity_delta;
    position_.y += delta;
}

float Object::compute_velocity(double delta_time, float velocity, float acceleration, float drag,
                               float max_velocity, bool gravity)
{
    if (acceleration != 0.0F)
    {
        velocity += acceleration * delta_time;
    }
    else if (drag != 0.0F)
    {
        if (velocity - drag > 0.0F)
        {
            velocity -= drag;
        }
        else if (velocity + drag < 0.0F)
        {
            velocity += drag;
        }
        else
        {
            velocity = 0.0F;
        }
    }

    if (gravity && gravity_scale_ > 0.0F)
    {
        velocity += (World::gravity * gravity_scale_) * delta_time;
    }

    if ((velocity != 0.0F) && (max_velocity != 0.0F))
    {
        if (velocity > max_velocity)
            velocity = max_velocity;
        else if (velocity < -max_velocity)
            velocity = -max_velocity;
    }

    return velocity;
}

void Object::set_velocity(Vector2f velocity)
{
    velocity_ = velocity;
}

Vector2f Object::get_velocity()
{
    return velocity_;
}

void Object::set_x_velocity(float velocity)
{
    velocity_.x = velocity;
}

float Object::get_x_velocity()
{
    return velocity_.x;
}

void Object::set_y_velocity(float velocity)
{
    velocity_.y = velocity;
}

float Object::get_y_velocity()
{
    return velocity_.y;
}

void Object::set_acceleration(Vector2f acceleration)
{
    acceleration_ = acceleration;
}

Vector2f Object::get_acceleration()
{
    return acceleration_;
}

void Object::set_max_velocity(float max_velocity)
{
    max_velocity_ = max_velocity;
}

float Object::get_max_velocity()
{
    return max_velocity_;
}

void Object::set_gravity_scale(float gravity_scale)
{
    gravity_scale_ = gravity_scale;
}

float Object::get_gravity_scale()
{
    return gravity_scale_;
}

void Object::set_movable(bool movable)
{
    movable_ = movable;
}

bool Object::get_movable()
{
    return movable_;
}

void Object::reset(Vector2f position)
{
    position_ = position;
}

void Object::resize(float width, float height)
{
    width_  = width;
    height_ = height;
}

Rect Object::bounding_rect()
{
    return Rect{position_, width_, height_};
}

void Object::kill()
{
    active_ = false;
}

} // namespace rinvid
