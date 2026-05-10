/**********************************************************************
 * Copyright (c) 2023 - 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <algorithm>

#include <rinvid/core/light.h>
#include <rinvid/core/render_context.h>

/// @todo Revisit these constants
constexpr float falloff_low{10.0F};
constexpr float falloff_high{100.0F};

constexpr float intensity_low{0.1F};
constexpr float intensity_high{5.0F};

namespace rinvid
{

namespace
{

RenderContext* get_render_context()
{
    return RenderContext::get_active_context();
}

} // namespace

float Light::remap(float value, float low1, float high1, float low2, float high2)
{
    value = std::clamp(value, low1, high1);
    return (value - low1) / (high1 - low1) * (high2 - low2) + low2;
}

Light::Light()
    : position_{0.0F, 0.0F}, intensity_{0.5F}, falloff_{0.5F}, ordinal_{-1}, active_{true},
      render_context_{nullptr}
{
    acquire_slot();
    upload();
}

Light::Light(Vector2f position, float intensity, float falloff)
    : position_{position}, intensity_{std::clamp(intensity, 0.0F, 1.0F)},
      falloff_{1.0F - std::clamp(falloff, 0.0F, 1.0F)}, ordinal_{-1}, active_{true},
      render_context_{nullptr}
{
    acquire_slot();
    upload();
}

Light::Light(const Light& other)
    : position_{other.position_}, intensity_{other.intensity_}, falloff_{other.falloff_},
      ordinal_{-1}, active_{other.active_}, render_context_{nullptr}
{
    acquire_slot();
    upload();
}

Light& Light::operator=(const Light& other)
{
    if (this == &other)
    {
        return *this;
    }

    if (ordinal_ < 0)
    {
        acquire_slot();
    }

    position_  = other.position_;
    intensity_ = other.intensity_;
    falloff_   = other.falloff_;
    active_    = other.active_;

    if (ordinal_ >= 0 && render_context_ != nullptr)
    {
        render_context_->set_default_light_active(ordinal_, this, active_);
    }

    upload();

    return *this;
}

Light::Light(Light&& other) noexcept
    : position_{other.position_}, intensity_{other.intensity_}, falloff_{other.falloff_},
      ordinal_{other.ordinal_}, active_{other.active_}, render_context_{other.render_context_}
{
    if (render_context_ != nullptr)
    {
        render_context_->replace_default_light_slot(ordinal_, &other, this);
    }

    other.ordinal_        = -1;
    other.active_         = false;
    other.render_context_ = nullptr;
}

Light& Light::operator=(Light&& other) noexcept
{
    if (this == &other)
    {
        return *this;
    }

    release_slot();

    position_       = other.position_;
    intensity_      = other.intensity_;
    falloff_        = other.falloff_;
    ordinal_        = other.ordinal_;
    active_         = other.active_;
    render_context_ = other.render_context_;

    if (render_context_ != nullptr)
    {
        render_context_->replace_default_light_slot(ordinal_, &other, this);
    }

    other.ordinal_        = -1;
    other.active_         = false;
    other.render_context_ = nullptr;

    return *this;
}

Light::~Light()
{
    release_slot();
}

void Light::acquire_slot()
{
    if (ordinal_ >= 0)
    {
        return;
    }

    render_context_ = get_render_context();
    if (render_context_ == nullptr)
    {
        return;
    }

    ordinal_ = render_context_->acquire_default_light_slot(this, active_);
}

void Light::release_slot()
{
    if (ordinal_ < 0 || render_context_ == nullptr)
    {
        return;
    }

    render_context_->release_default_light_slot(ordinal_, this);

    ordinal_        = -1;
    active_         = false;
    render_context_ = nullptr;
}

void Light::upload()
{
    if (ordinal_ >= 0 && render_context_ != nullptr &&
        !render_context_->owns_default_light_slot(ordinal_, this))
    {
        ordinal_        = -1;
        render_context_ = nullptr;
    }

    if (ordinal_ < 0)
    {
        acquire_slot();
    }

    if (ordinal_ < 0 || render_context_ == nullptr)
    {
        return;
    }

    const float shader_intensity =
        active_ ? remap(intensity_, 0.0F, 1.0F, intensity_low, intensity_high) : 0.0F;
    const float shader_falloff = remap(falloff_, 0.0F, 1.0F, falloff_low, falloff_high);

    render_context_->set_default_light(static_cast<std::uint32_t>(ordinal_), position_.x,
                                       position_.y, shader_intensity, shader_falloff);
}

void Light::move(const Vector2f move_vector)
{
    position_.x += move_vector.x;
    position_.y += move_vector.y;
    upload();
}

void Light::set_position(const Vector2f vector)
{
    position_ = vector;
    upload();
}

void Light::set_intensity(float intensity)
{
    intensity_ = std::clamp(intensity, 0.0F, 1.0F);
    upload();
}

void Light::set_falloff(float falloff)
{
    falloff_ = 1.0F - std::clamp(falloff, 0.0F, 1.0F);
    upload();
}

void Light::switch_it(bool on)
{
    set_active(on);
}

void Light::set_active(bool active)
{
    active_ = active;

    if (ordinal_ < 0)
    {
        acquire_slot();
    }

    if (ordinal_ >= 0 && render_context_ != nullptr)
    {
        render_context_->set_default_light_active(ordinal_, this, active_);
    }

    upload();
}

bool Light::is_active() const
{
    return active_;
}

Vector2f Light::get_position() const
{
    return position_;
}

float Light::get_intensity() const
{
    return std::clamp(intensity_, 0.0F, 1.0F);
}

float Light::get_falloff() const
{
    return 1.0F - std::clamp(falloff_, 0.0F, 1.0F);
}

void Light::update(Vector2f camera_pos)
{
    (void)camera_pos;
    upload();
}

} // namespace rinvid
