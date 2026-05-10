/**********************************************************************
 * Copyright (c) 2023 - 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef INCLUDE_RINVID_CORE_LIGHT_H
#define INCLUDE_RINVID_CORE_LIGHT_H

#include <cstdint>

#include <rinvid/util/vector2.h>

namespace rinvid
{

class RenderContext;

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
     * @brief Copy constructor.
     *
     * The copied light receives its own renderer slot.
     *
     *************************************************************************************************/
    Light(const Light& other);

    /**************************************************************************************************
     * @brief Copy assignment operator.
     *
     *************************************************************************************************/
    Light& operator=(const Light& other);

    /**************************************************************************************************
     * @brief Move constructor.
     *
     *************************************************************************************************/
    Light(Light&& other) noexcept;

    /**************************************************************************************************
     * @brief Move assignment operator.
     *
     *************************************************************************************************/
    Light& operator=(Light&& other) noexcept;

    /**************************************************************************************************
     * @brief Destructor.
     *
     *************************************************************************************************/
    ~Light();

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
     * @brief Sets whether light contributes to default shader lighting.
     *
     * @param active True if light should contribute to lighting.
     *
     *************************************************************************************************/
    void set_active(bool active);

    /**************************************************************************************************
     * @brief Returns whether light contributes to default shader lighting.
     *
     * @return True if light is active.
     *
     *************************************************************************************************/
    bool is_active() const;

    /**************************************************************************************************
     * @brief Returns light's position.
     *
     * @return Current light position.
     *
     *************************************************************************************************/
    Vector2f get_position() const;

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
     * @brief Synchronizes light state with renderer.
     *
     * Usually not needed after calling setters. The camera_pos parameter is kept for
     * compatibility and ignored because default lighting uses world-space positions.
     *
     * @param camera_pos Ignored.
     *
     *************************************************************************************************/
    void update(Vector2f camera_pos = {0.0F, 0.0F});

  private:
    /**************************************************************************************************
     * @brief Helper function to remap a floating point value from one range to another.
     *
     *************************************************************************************************/
    static float remap(float value, float low1, float high1, float low2, float high2);

    void acquire_slot();
    void release_slot();
    void upload();

    Vector2f       position_;
    float          intensity_;
    float          falloff_;
    std::int32_t   ordinal_;
    bool           active_;
    RenderContext* render_context_;
};

} // namespace rinvid

#endif // INCLUDE_RINVID_CORE_LIGHT_H
