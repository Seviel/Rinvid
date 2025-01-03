/**********************************************************************
 * Copyright (c) 2023 - 2024, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef CORE_INCLUDE_LIGHTS_H
#define CORE_INCLUDE_LIGHTS_H

#include <cstdint>

#include "util/include/color.h"
#include "util/include/vector2.h"
#include "util/include/vector3.h"

#define MAX_NUMBER_OF_LIGHTS 100

namespace rinvid
{

/**************************************************************************************************
 * @brief A light source. Only works if you're using default shaders.
 *
 *************************************************************************************************/
class Light
{
  public:
    /**************************************************************************************************
     * @brief Light constructor.
     *
     *************************************************************************************************/
    Light();

    /**************************************************************************************************
     * @brief Light constructor.
     *
     * @param position
     * @param intensity How bright is the light.
     * @param falloff How quickly does light lose strength with distance.
     *
     *************************************************************************************************/
    Light(Vector2f position, float intensity, float falloff);

    /**************************************************************************************************
     * @brief Moves the light by adding move_vector to its position vector.
     *
     * @param move_vector Vector to be added to light's position vector
     *
     *************************************************************************************************/
    void move(const Vector2f move_vector);

    /**************************************************************************************************
     * @brief Sets light's position to the position of passed vector.
     *
     * @param vector A new position vector of the light
     *
     *************************************************************************************************/
    void set_position(const Vector2f vector);

    /**************************************************************************************************
     * @brief Sets light's intensity.
     *
     * @param intensity Intensity in range [0.0, 1.0]
     *
     *************************************************************************************************/
    void set_intensity(float intensity);

    /**************************************************************************************************
     * @brief Sets light's falloff. Higher falloff value makes light diminish more as distance
     * increases.
     *
     * @param falloff Falloff in the range [0.0, 1.0]
     *
     *************************************************************************************************/
    void set_falloff(float falloff);

    /**************************************************************************************************
     * @brief Switches light on or off.
     *
     * @param on Set to true if you want to switch the light on, false otherwise.
     *
     *************************************************************************************************/
    void switch_it(bool on);

    /**************************************************************************************************
     * @brief Returns light's intensity.
     *
     * @return Current light intensity.
     *
     *************************************************************************************************/
    float get_intensity() const;

    /**************************************************************************************************
     * @brief Returns light's falloff.
     *
     * @return Current light falloff.
     *
     *************************************************************************************************/
    float get_falloff() const;

    /**************************************************************************************************
     * @brief Updates the light, should be called every frame.
     *
     * @param camera_pos Position of the camera if there is any.
     *
     *************************************************************************************************/
    void update(Vector2f camera_pos = {0.0F, 0.0F});

  private:
    /**************************************************************************************************
     * @brief Helper function to remap a floating point value from one range to another.
     *
     *************************************************************************************************/
    float remap(float value, float low1, float high1, float low2, float high2);

    Vector2f            position_;
    float               intensity_;
    float               falloff_;
    std::int32_t        ordinal_;
    static std::int32_t number_of_lights_;
};

} // namespace rinvid

#endif // CORE_INCLUDE_LIGHTS_H
