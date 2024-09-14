/**********************************************************************
 * Copyright (c) 2023 - 2024, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <algorithm>

#include "core/include/light.h"
#include "core/include/rinvid_gfx.h"

/// @todo Revisit these constants
constexpr float falloff_low{10.0F};
constexpr float falloff_high{100.0F};

constexpr float intensity_low{0.1F};
constexpr float intensity_high{5.0F};

namespace rinvid
{

std::int32_t Light::number_of_lights_{0};

float Light::remap(float value, float low1, float high1, float low2, float high2)
{
    value = std::clamp(value, low1, high1);
    return (value - low1) / (high1 - low1) * (high2 - low2) + low2;
}

Light::Light() : position_{0.0F, 0.0F}, intensity_{0.5F}, falloff_{0.5F}
{
    ordinal_ = number_of_lights_;
    ++number_of_lights_;

    if (number_of_lights_ > MAX_NUMBER_OF_LIGHTS)
    {
        throw "Maximum number of light sources exceeded!";
    }

    const auto texture_shader = RinvidGfx::get_texture_default_shader();
    texture_shader.use();
    texture_shader.set_bool("light_active[" + std::to_string(ordinal_) + "]", true);
    texture_shader.set_float2("light_pos[" + std::to_string(ordinal_) + "]", position_.x,
                              RinvidGfx::get_height() - position_.y);
    texture_shader.set_float("light_intensity[" + std::to_string(ordinal_) + "]",
                             remap(intensity_, 0.0F, 1.0F, intensity_low, intensity_high));
    texture_shader.set_float("light_falloff[" + std::to_string(ordinal_) + "]",
                             remap(falloff_, 0.0F, 1.0F, falloff_low, falloff_high));

    const auto shape_shader = RinvidGfx::get_shape_default_shader();
    shape_shader.use();
    shape_shader.set_bool("light_active[" + std::to_string(ordinal_) + "]", true);
    shape_shader.set_float2("light_pos[" + std::to_string(ordinal_) + "]", position_.x,
                            RinvidGfx::get_height() - position_.y);
    shape_shader.set_float("light_intensity[" + std::to_string(ordinal_) + "]",
                           remap(intensity_, 0.0F, 1.0F, intensity_low, intensity_high));
    shape_shader.set_float("light_falloff[" + std::to_string(ordinal_) + "]",
                           remap(falloff_, 0.0F, 1.0F, falloff_low, falloff_high));
}

Light::Light(Vector2f position, float intensity, float falloff)
    : position_{position}, intensity_{intensity}, falloff_{falloff}
{
    ordinal_ = number_of_lights_;
    ++number_of_lights_;

    intensity_ = std::clamp(intensity, 0.0F, 1.0F);
    falloff_   = 1.0F - std::clamp(falloff, 0.0F, 1.0F);

    if (number_of_lights_ > MAX_NUMBER_OF_LIGHTS)
    {
        throw "Maximum number of light sources exceeded!";
    }

    const auto texture_shader = RinvidGfx::get_texture_default_shader();
    texture_shader.use();
    texture_shader.set_bool("light_active[" + std::to_string(ordinal_) + "]", true);
    texture_shader.set_float2("light_pos[" + std::to_string(ordinal_) + "]", position_.x,
                              RinvidGfx::get_height() - position_.y);
    texture_shader.set_float("light_intensity[" + std::to_string(ordinal_) + "]",
                             remap(intensity_, 0.0F, 1.0F, intensity_low, intensity_high));
    texture_shader.set_float("light_falloff[" + std::to_string(ordinal_) + "]",
                             remap(falloff_, 0.0F, 1.0F, falloff_low, falloff_high));

    const auto shape_shader = RinvidGfx::get_shape_default_shader();
    shape_shader.use();
    shape_shader.set_bool("light_active[" + std::to_string(ordinal_) + "]", true);
    shape_shader.set_float2("light_pos[" + std::to_string(ordinal_) + "]", position_.x,
                            RinvidGfx::get_height() - position_.y);
    shape_shader.set_float("light_intensity[" + std::to_string(ordinal_) + "]",
                           remap(intensity_, 0.0F, 1.0F, intensity_low, intensity_high));
    shape_shader.set_float("light_falloff[" + std::to_string(ordinal_) + "]",
                           remap(falloff_, 0.0F, 1.0F, falloff_low, falloff_high));
}

void Light::move(const Vector2f move_vector)
{
    position_.x += move_vector.x;
    position_.y += move_vector.y;
}

void Light::set_position(const Vector2f vector)
{
    position_ = vector;

    const auto texture_shader = RinvidGfx::get_texture_default_shader();
    texture_shader.use();
    texture_shader.set_float2("light_pos[" + std::to_string(ordinal_) + "]", position_.x,
                              RinvidGfx::get_height() - position_.y);

    const auto shape_shader = RinvidGfx::get_shape_default_shader();
    shape_shader.use();
    shape_shader.set_float2("light_pos[" + std::to_string(ordinal_) + "]", position_.x,
                            RinvidGfx::get_height() - position_.y);
}

void Light::set_intensity(float intensity)
{
    intensity_ = std::clamp(intensity, 0.0F, 1.0F);

    const auto texture_shader = RinvidGfx::get_texture_default_shader();
    texture_shader.use();
    texture_shader.set_float("light_intensity[" + std::to_string(ordinal_) + "]",
                             remap(intensity_, 0.0F, 1.0F, intensity_low, intensity_high));

    const auto shape_shader = RinvidGfx::get_shape_default_shader();
    shape_shader.use();
    shape_shader.set_float("light_intensity[" + std::to_string(ordinal_) + "]",
                           remap(intensity_, 0.0F, 1.0F, intensity_low, intensity_high));
}

void Light::set_falloff(float falloff)
{
    falloff_ = 1.0F - std::clamp(falloff, 0.0F, 1.0F);

    const auto texture_shader = RinvidGfx::get_texture_default_shader();
    texture_shader.use();
    texture_shader.set_float("light_falloff[" + std::to_string(ordinal_) + "]",
                             remap(falloff_, 0.0F, 1.0F, falloff_low, falloff_high));

    const auto shape_shader = RinvidGfx::get_shape_default_shader();
    shape_shader.use();
    shape_shader.set_float("light_falloff[" + std::to_string(ordinal_) + "]",
                           remap(falloff_, 0.0F, 1.0F, falloff_low, falloff_high));
}

void Light::switch_it(bool on)
{
    const auto texture_shader = RinvidGfx::get_texture_default_shader();
    texture_shader.use();
    texture_shader.set_bool("light_active[" + std::to_string(ordinal_) + "]", on);

    const auto shape_shader = RinvidGfx::get_shape_default_shader();
    shape_shader.use();
    shape_shader.set_bool("light_active[" + std::to_string(ordinal_) + "]", on);
}

float Light::get_intensity() const
{
    return std::clamp(intensity_, 0.0F, 1.0F);
}

float Light::get_falloff() const
{
    return 1.0F - std::clamp(falloff_, 0.0F, 1.0F);
}

/// @todo This is bullshit, this should probably be handled in the shader and update method should
/// not exist, revisit this later.
void Light::update(Vector2f camera_pos)
{
    Vector2f position = position_;
    position.x -= camera_pos.x;
    position.y -= camera_pos.y;
    const auto texture_shader = RinvidGfx::get_texture_default_shader();
    texture_shader.use();
    texture_shader.set_float2("light_pos[" + std::to_string(ordinal_) + "]", position.x,
                              RinvidGfx::get_height() - position.y);

    const auto shape_shader = RinvidGfx::get_shape_default_shader();
    shape_shader.use();
    shape_shader.set_float2("light_pos[" + std::to_string(ordinal_) + "]", position.x,
                            RinvidGfx::get_height() - position.y);
}

} // namespace rinvid
