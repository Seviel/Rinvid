/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include "stress_scene.h"

#include <algorithm>
#include <array>
#include <cstddef>
#include <random>
#include <sstream>
#include <string>
#include <utility>

#include <rinvid/core/light_manager.h>
#include <rinvid/core/render_context.h>
#include <rinvid/util/color.h>

namespace
{

constexpr std::int32_t MIN_TEXTURE_SIZE{50};
constexpr std::int32_t MAX_TEXTURE_SIZE{500};
constexpr float        BACKGROUND_COLOR{0.06F};
constexpr float        MAX_SPRITE_SPEED{180.0F};
constexpr float        MAX_SHAPE_SPEED{160.0F};
constexpr float        MAX_TEXT_SPEED{120.0F};
constexpr float        MAX_LIGHT_SPEED{140.0F};
constexpr float        MIN_SPEED_MAGNITUDE{35.0F};
constexpr float        MAX_ROTATION_SPEED{90.0F};
constexpr float        TEXT_MAX_WIDTH{220.0F};
constexpr float        TEXT_HEIGHT_SCALE{1.5F};

float clamp_magnitude(float value, float minimum_magnitude)
{
    if (value >= 0.0F && value < minimum_magnitude)
    {
        return minimum_magnitude;
    }

    if (value < 0.0F && value > -minimum_magnitude)
    {
        return -minimum_magnitude;
    }

    return value;
}

float random_float(std::mt19937& generator, float minimum, float maximum)
{
    std::uniform_real_distribution<float> distribution{minimum, maximum};
    return distribution(generator);
}

std::int32_t random_int(std::mt19937& generator, std::int32_t minimum, std::int32_t maximum)
{
    std::uniform_int_distribution<std::int32_t> distribution{minimum, maximum};
    return distribution(generator);
}

rinvid::Vector2f random_velocity(std::mt19937& generator, float max_speed)
{
    rinvid::Vector2f velocity{};
    velocity.x =
        clamp_magnitude(random_float(generator, -max_speed, max_speed), MIN_SPEED_MAGNITUDE);
    velocity.y =
        clamp_magnitude(random_float(generator, -max_speed, max_speed), MIN_SPEED_MAGNITUDE);
    return velocity;
}

rinvid::Color random_color(std::mt19937& generator)
{
    return rinvid::Color{random_int(generator, 32, 255), random_int(generator, 32, 255),
                         random_int(generator, 32, 255), 255};
}

void bounce_coordinate(float& position, float& velocity, float minimum, float maximum)
{
    if (position < minimum)
    {
        position = minimum;
        velocity = std::abs(velocity);
    }
    else if (position > maximum)
    {
        position = maximum;
        velocity = -std::abs(velocity);
    }
}

const std::array<std::string, 8> k_text_fragments{"Rinvid", "Stress", "Sprites", "Shapes",
                                                  "Lights", "Text",   "Frames",  "Bench"};

std::string make_text_content(std::mt19937& generator, std::uint32_t index)
{
    const auto first_index  = static_cast<std::size_t>(random_int(generator, 0, 7));
    const auto second_index = static_cast<std::size_t>(random_int(generator, 0, 7));

    return k_text_fragments.at(first_index) + " " + k_text_fragments.at(second_index) + " " +
           std::to_string(index);
}

} // namespace

StressSceneContent::GeneratedTexture StressSceneContent::make_texture(std::mt19937& generator)
{
    GeneratedTexture texture{};
    texture.width_  = random_int(generator, MIN_TEXTURE_SIZE, MAX_TEXTURE_SIZE);
    texture.height_ = random_int(generator, MIN_TEXTURE_SIZE, MAX_TEXTURE_SIZE);
    texture.pixels_.resize(static_cast<std::size_t>(texture.width_) *
                           static_cast<std::size_t>(texture.height_) * 4U);

    for (std::size_t i{0U}; i < texture.pixels_.size(); i += 4U)
    {
        texture.pixels_[i]     = static_cast<std::uint8_t>(random_int(generator, 0, 255));
        texture.pixels_[i + 1] = static_cast<std::uint8_t>(random_int(generator, 0, 255));
        texture.pixels_[i + 2] = static_cast<std::uint8_t>(random_int(generator, 0, 255));
        texture.pixels_[i + 3] = 255U;
    }

    return texture;
}

StressSceneCounts make_uniform_stress_scene_counts(std::uint32_t count)
{
    return StressSceneCounts{count, count, count, count};
}

std::string format_stress_scene_counts(const StressSceneCounts& counts)
{
    std::ostringstream builder{};
    builder << "sprites=" << counts.sprite_count_ << " shapes=" << counts.shape_count_
            << " text=" << counts.text_count_ << " lights=" << counts.light_count_;
    return builder.str();
}

StressSceneContent::StressSceneContent(const std::string& font_path, std::uint32_t viewport_width,
                                       std::uint32_t            viewport_height,
                                       const StressSceneCounts& max_counts)
    : viewport_width_{viewport_width}, viewport_height_{viewport_height}, max_counts_{max_counts},
      active_counts_{0U, 0U, 0U, 0U}, sprites_{}, texts_{}, shapes_{}, lights_{}
{
    std::mt19937 generator{1337U};

    rinvid::LightManager::activate_ambient_light(0.35F);

    sprites_.reserve(max_counts_.sprite_count_);
    texts_.reserve(max_counts_.text_count_);
    shapes_.reserve(max_counts_.shape_count_);
    lights_.reserve(max_counts_.light_count_);

    for (std::uint32_t index{0U}; index < max_counts_.sprite_count_; ++index)
    {
        sprites_.emplace_back(make_sprite_entity(generator));
    }

    for (std::uint32_t index{0U}; index < max_counts_.text_count_; ++index)
    {
        texts_.emplace_back(make_text_entity(generator, font_path, index));
    }

    for (std::uint32_t index{0U}; index < max_counts_.shape_count_; ++index)
    {
        shapes_.emplace_back(make_shape_entity(generator));
    }

    for (std::uint32_t index{0U}; index < max_counts_.light_count_; ++index)
    {
        lights_.emplace_back(make_light_entity(generator));
    }

    set_active_counts({0U, 0U, 0U, 0U});
}

StressSceneContent::~StressSceneContent() = default;

StressSceneContent::SpriteEntity StressSceneContent::make_sprite_entity(std::mt19937& generator)
{
    SpriteEntity     entity{};
    GeneratedTexture generated_texture = make_texture(generator);
    const auto       max_x = viewport_width_ - static_cast<float>(generated_texture.width_);
    const auto       max_y = viewport_height_ - static_cast<float>(generated_texture.height_);

    entity.texture_ = std::make_unique<rinvid::Texture>(
        generated_texture.pixels_, generated_texture.width_, generated_texture.height_);
    entity.position_ = rinvid::Vector2f{random_float(generator, 0.0F, std::max(0.0F, max_x)),
                                        random_float(generator, 0.0F, std::max(0.0F, max_y))};
    entity.velocity_ = random_velocity(generator, MAX_SPRITE_SPEED);
    entity.angular_velocity_ = random_float(generator, -MAX_ROTATION_SPEED, MAX_ROTATION_SPEED);
    entity.width_            = generated_texture.width_;
    entity.height_           = generated_texture.height_;
    entity.sprite_ = std::make_unique<rinvid::Sprite>(entity.texture_.get(), entity.width_,
                                                      entity.height_, entity.position_);
    entity.sprite_->set_rotation(random_float(generator, 0.0F, 360.0F));
    entity.sprite_->set_opacity(random_float(generator, 0.4F, 1.0F));

    return entity;
}

StressSceneContent::TextEntity StressSceneContent::make_text_entity(std::mt19937&      generator,
                                                                    const std::string& font_path,
                                                                    std::uint32_t      index)
{
    TextEntity entity{};
    const auto size = static_cast<std::uint32_t>(random_int(generator, 16, 34));

    entity.max_width_        = TEXT_MAX_WIDTH;
    entity.estimated_height_ = static_cast<float>(size) * TEXT_HEIGHT_SCALE;
    entity.position_         = rinvid::Vector2f{
        random_float(generator, 0.0F, std::max(0.0F, viewport_width_ - entity.max_width_)),
        random_float(generator, 0.0F, std::max(0.0F, viewport_height_ - entity.estimated_height_))};
    entity.velocity_ = random_velocity(generator, MAX_TEXT_SPEED);
    entity.text_ = std::make_unique<rinvid::Text>(make_text_content(generator, index), font_path,
                                                  entity.position_, random_color(generator), size);
    entity.text_->set_max_width(entity.max_width_);

    return entity;
}

StressSceneContent::ShapeEntity StressSceneContent::make_shape_entity(std::mt19937& generator)
{
    ShapeEntity entity{};
    const auto  shape_kind = random_int(generator, 0, 2);

    entity.velocity_         = random_velocity(generator, MAX_SHAPE_SPEED);
    entity.angular_velocity_ = random_float(generator, -MAX_ROTATION_SPEED, MAX_ROTATION_SPEED);

    if (shape_kind == 0)
    {
        const auto width  = random_float(generator, 40.0F, 160.0F);
        const auto height = random_float(generator, 40.0F, 160.0F);

        entity.extent_x_ = width / 2.0F;
        entity.extent_y_ = height / 2.0F;
        entity.center_   = rinvid::Vector2f{
            random_float(generator, entity.extent_x_, viewport_width_ - entity.extent_x_),
            random_float(generator, entity.extent_y_, viewport_height_ - entity.extent_y_)};

        auto rectangle = std::make_unique<rinvid::RectangleShape>(entity.center_, width, height);
        rectangle->set_color(random_color(generator));
        rectangle->set_rotation(random_float(generator, 0.0F, 360.0F));
        entity.shape_ = std::move(rectangle);
    }
    else if (shape_kind == 1)
    {
        const auto radius = random_float(generator, 25.0F, 90.0F);

        entity.extent_x_ = radius;
        entity.extent_y_ = radius;
        entity.center_   = rinvid::Vector2f{
            random_float(generator, entity.extent_x_, viewport_width_ - entity.extent_x_),
            random_float(generator, entity.extent_y_, viewport_height_ - entity.extent_y_)};

        auto circle = std::make_unique<rinvid::CircleShape>(entity.center_, radius);
        circle->set_color(random_color(generator));
        entity.shape_ = std::move(circle);
    }
    else
    {
        const auto half_width  = random_float(generator, 30.0F, 90.0F);
        const auto half_height = random_float(generator, 30.0F, 90.0F);

        entity.extent_x_ = half_width;
        entity.extent_y_ = half_height;
        entity.center_   = rinvid::Vector2f{
            random_float(generator, entity.extent_x_, viewport_width_ - entity.extent_x_),
            random_float(generator, entity.extent_y_, viewport_height_ - entity.extent_y_)};

        auto triangle = std::make_unique<rinvid::TriangleShape>(
            rinvid::Vector2f{entity.center_.x, entity.center_.y - half_height},
            rinvid::Vector2f{entity.center_.x - half_width, entity.center_.y + half_height},
            rinvid::Vector2f{entity.center_.x + half_width, entity.center_.y + half_height});
        triangle->set_color(random_color(generator));
        triangle->set_rotation(random_float(generator, 0.0F, 360.0F));
        entity.shape_ = std::move(triangle);
    }

    return entity;
}

StressSceneContent::LightEntity StressSceneContent::make_light_entity(std::mt19937& generator)
{
    LightEntity entity{};

    entity.position_ =
        rinvid::Vector2f{random_float(generator, 0.0F, static_cast<float>(viewport_width_)),
                         random_float(generator, 0.0F, static_cast<float>(viewport_height_))};
    entity.velocity_ = random_velocity(generator, MAX_LIGHT_SPEED);
    entity.light_    = std::make_unique<rinvid::Light>(
        entity.position_, random_float(generator, 0.2F, 1.0F), random_float(generator, 0.2F, 1.0F));
    return entity;
}

void StressSceneContent::set_active_counts(const StressSceneCounts& counts)
{
    active_counts_.sprite_count_ = std::min(counts.sprite_count_, max_counts_.sprite_count_);
    active_counts_.shape_count_  = std::min(counts.shape_count_, max_counts_.shape_count_);
    active_counts_.text_count_   = std::min(counts.text_count_, max_counts_.text_count_);
    active_counts_.light_count_  = std::min(counts.light_count_, max_counts_.light_count_);

    for (std::size_t index{0U}; index < lights_.size(); ++index)
    {
        lights_.at(index).light_->set_active(index < active_counts_.light_count_);
    }
}

const StressSceneCounts& StressSceneContent::get_active_counts() const
{
    return active_counts_;
}

const StressSceneCounts& StressSceneContent::get_max_counts() const
{
    return max_counts_;
}

void StressSceneContent::step(double delta_time)
{
    const auto delta = static_cast<float>(delta_time);

    for (std::size_t index{0U}; index < active_counts_.sprite_count_; ++index)
    {
        auto& sprite = sprites_.at(index);
        sprite.position_.x += sprite.velocity_.x * delta;
        sprite.position_.y += sprite.velocity_.y * delta;
        bounce_coordinate(sprite.position_.x, sprite.velocity_.x, 0.0F,
                          viewport_width_ - static_cast<float>(sprite.width_));
        bounce_coordinate(sprite.position_.y, sprite.velocity_.y, 0.0F,
                          viewport_height_ - static_cast<float>(sprite.height_));
        sprite.sprite_->set_position(sprite.position_);
        sprite.sprite_->rotate(sprite.angular_velocity_ * delta);
    }

    for (std::size_t index{0U}; index < active_counts_.shape_count_; ++index)
    {
        auto& shape = shapes_.at(index);
        shape.center_.x += shape.velocity_.x * delta;
        shape.center_.y += shape.velocity_.y * delta;
        bounce_coordinate(shape.center_.x, shape.velocity_.x, shape.extent_x_,
                          viewport_width_ - shape.extent_x_);
        bounce_coordinate(shape.center_.y, shape.velocity_.y, shape.extent_y_,
                          viewport_height_ - shape.extent_y_);

        std::visit(
            [&](auto& shape_ptr)
            {
                shape_ptr->set_position(shape.center_);
                shape_ptr->rotate(shape.angular_velocity_ * delta);
            },
            shape.shape_);
    }

    for (std::size_t index{0U}; index < active_counts_.text_count_; ++index)
    {
        auto& text = texts_.at(index);
        text.position_.x += text.velocity_.x * delta;
        text.position_.y += text.velocity_.y * delta;
        bounce_coordinate(text.position_.x, text.velocity_.x, 0.0F,
                          viewport_width_ - text.max_width_);
        bounce_coordinate(text.position_.y, text.velocity_.y, text.estimated_height_,
                          viewport_height_);
        text.text_->set_position(text.position_);
    }

    for (std::size_t index{0U}; index < active_counts_.light_count_; ++index)
    {
        auto& light = lights_.at(index);
        light.position_.x += light.velocity_.x * delta;
        light.position_.y += light.velocity_.y * delta;
        bounce_coordinate(light.position_.x, light.velocity_.x, 0.0F,
                          static_cast<float>(viewport_width_));
        bounce_coordinate(light.position_.y, light.velocity_.y, 0.0F,
                          static_cast<float>(viewport_height_));
        light.light_->set_position(light.position_);
    }
}

void StressSceneContent::render()
{
    auto* render_context = rinvid::RenderContext::get_active_context();
    if (render_context == nullptr)
    {
        return;
    }

    render_context->clear_screen(BACKGROUND_COLOR, BACKGROUND_COLOR, BACKGROUND_COLOR, 1.0F);

    for (std::size_t index{0U}; index < active_counts_.sprite_count_; ++index)
    {
        sprites_.at(index).sprite_->draw();
    }

    for (std::size_t index{0U}; index < active_counts_.shape_count_; ++index)
    {
        std::visit([](auto& shape_ptr) { shape_ptr->draw(); }, shapes_.at(index).shape_);
    }

    for (std::size_t index{0U}; index < active_counts_.text_count_; ++index)
    {
        texts_.at(index).text_->draw();
    }
}
