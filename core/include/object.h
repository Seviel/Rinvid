/**********************************************************************
 * Copyright (c) 2025 - 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef CORE_INCLUDE_OBJECT_H
#define CORE_INCLUDE_OBJECT_H

#include <cstdint>

#include "util/include/rect.h"
#include "util/include/vector2.h"

namespace rinvid
{

class World;

constexpr std::uint8_t NONE  = 0x0;
constexpr std::uint8_t LEFT  = 0x1;
constexpr std::uint8_t RIGHT = 0x2;
constexpr std::uint8_t UP    = 0x4;
constexpr std::uint8_t DOWN  = 0x8;
constexpr std::uint8_t WALL  = LEFT | RIGHT;
constexpr std::uint8_t ANY   = LEFT | RIGHT | UP | DOWN;

/**************************************************************************************************
 * @brief A movable object in the world.
 *
 *************************************************************************************************/
class Object
{
  public:
    /**************************************************************************************************
     * @brief Default constructor.
     *
     *************************************************************************************************/
    Object();

    virtual ~Object()
    {
    }

    /**************************************************************************************************
     * @brief Updates object state. Should be called each frame.
     *
     *************************************************************************************************/
    virtual void update(double delta_time);

    /**************************************************************************************************
     * @brief Returns current position of the object.
     *
     * @return Current position.
     *
     *************************************************************************************************/
    Vector2f get_position();

    /**************************************************************************************************
     * @brief Sets the velocity of the object.
     *
     * @param velocity New velocity.
     *
     *************************************************************************************************/
    void set_velocity(Vector2f velocity);

    /**************************************************************************************************
     * @brief Returns current velocity of the object.
     *
     * @return Current velocity.
     *
     *************************************************************************************************/
    Vector2f get_velocity();

    /**************************************************************************************************
     * @brief Sets the velocity of the object on x axis.
     *
     * @param velocity New velocity on x axis.
     *
     *************************************************************************************************/
    void set_x_velocity(float velocity);

    /**************************************************************************************************
     * @brief Returns current velocity of the object on x axis.
     *
     * @return Current velocity on x axis.
     *
     *************************************************************************************************/
    float get_x_velocity();

    /**************************************************************************************************
     * @brief Sets the velocity of the object on y axis.
     *
     * @param velocity New velocity on y axis.
     *
     *************************************************************************************************/
    void set_y_velocity(float velocity);

    /**************************************************************************************************
     * @brief Returns current velocity of the object on y axis.
     *
     * @return Current velocity on y axis.
     *
     *************************************************************************************************/
    float get_y_velocity();

    /**************************************************************************************************
     * @brief Sets the acceleration of the object.
     *
     * @param acceleration New acceleration.
     *
     *************************************************************************************************/
    void set_acceleration(Vector2f acceleration);

    /**************************************************************************************************
     * @brief Returns current acceleration of the object.
     *
     * @return Current acceleration.
     *
     *************************************************************************************************/
    Vector2f get_acceleration();

    /**************************************************************************************************
     * @brief Sets the max_velocity of the object.
     *
     * @param max_velocity New max_velocity.
     *
     *************************************************************************************************/
    void set_max_velocity(float max_velocity);

    /**************************************************************************************************
     * @brief Returns current max_velocity of the object.
     *
     * @return Current max_velocity.
     *
     *************************************************************************************************/
    float get_max_velocity();

    /**************************************************************************************************
     * @brief Changes scale to which this object is affected by gravity.
     *
     * @param gravity_scale Scale to which extent is this object affected by gravity, 0.0 being
     * minimum.
     *
     *************************************************************************************************/
    void set_gravity_scale(float gravity_scale);

    /**************************************************************************************************
     * @brief Returns scale to which this object is affected by gravity.
     *
     * @return Gravity scale.
     *
     *************************************************************************************************/
    float get_gravity_scale();

    /**************************************************************************************************
     * @brief Changes whether the object can be moved.
     *
     * @param movable
     *
     *************************************************************************************************/
    void set_movable(bool movable);

    /**************************************************************************************************
     * @brief Returns whether the object is movable.
     *
     * @return Movable.
     *
     *************************************************************************************************/
    bool get_movable();

    /**************************************************************************************************
     * @brief Check whether object is touching any solid surface.
     *
     * @param direction For which direction do you want to check. You can check for multiple
     * directions by using bitwise or operator. For example, use:
     * direction = LEFT | UP to check if the object touches anything on its
     * left or above it.
     *
     * @return Whether object is touching solid surface in given direction.
     *
     *************************************************************************************************/
    bool is_touching(std::uint8_t direction);

    /**************************************************************************************************
     * @brief Sets in which directions can this object collide with other objects.
     *
     * @param direction You can set multiple directions by using bitwise or operator. For example,
     * use: direction = LEFT | UP to set collisions only in left and up
     * directions.
     *
     *************************************************************************************************/
    void set_allowed_collisions(std::uint8_t directions);

    /**************************************************************************************************
     * @brief Resets the position of the object.
     *
     * @param posiition New position.
     *
     *************************************************************************************************/
    void reset(Vector2f position);

    /**************************************************************************************************
     * @brief Resizes the object.
     *
     * @param width New width.
     * @param height New height.
     *
     *************************************************************************************************/
    void resize(float width, float height);

    /**************************************************************************************************
     * @brief Returns bounding box rect of the object.
     *
     * @return Bounding rect.
     *
     *************************************************************************************************/
    virtual Rect bounding_rect();

    /**************************************************************************************************
     * @brief Makes object inactive.
     *
     *************************************************************************************************/
    void kill();

  protected:
    friend class World;
    float compute_velocity(double delta_time, float velocity, float acceleration, float drag,
                           float max_velocity, bool gravity = false);

    void update_motion(double delta_time);

    Vector2f     position_;
    Vector2f     previous_position_;
    Vector2f     velocity_;
    Vector2f     acceleration_;
    Vector2f     drag_;
    float        max_velocity_;
    float        gravity_scale_;
    std::int32_t width_;
    std::int32_t height_;
    bool         active_;
    bool         movable_;
    bool         collides_;
    std::uint8_t touching_;
    std::uint8_t allowed_collisions_;
};

} // namespace rinvid

#endif // CORE_INCLUDE_OBJECT_H
