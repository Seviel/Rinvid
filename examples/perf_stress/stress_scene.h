/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef EXAMPLES_PERF_STRESS_STRESS_SCENE_H
#define EXAMPLES_PERF_STRESS_STRESS_SCENE_H

#include <cstdint>
#include <memory>
#include <random>
#include <string>
#include <variant>
#include <vector>

#include <rinvid/core/circle_shape.h>
#include <rinvid/core/light.h>
#include <rinvid/core/rectangle_shape.h>
#include <rinvid/core/sprite.h>
#include <rinvid/core/text.h>
#include <rinvid/core/texture.h>
#include <rinvid/core/triangle_shape.h>
#include <rinvid/util/vector2.h>

struct StressSceneCounts
{
    std::uint32_t sprite_count_;
    std::uint32_t shape_count_;
    std::uint32_t text_count_;
    std::uint32_t light_count_;
};

StressSceneCounts make_uniform_stress_scene_counts(std::uint32_t count);

std::string format_stress_scene_counts(const StressSceneCounts& counts);

class StressSceneContent
{
  public:
    StressSceneContent(const std::string& font_path, std::uint32_t viewport_width,
                       std::uint32_t viewport_height, const StressSceneCounts& max_counts);
    ~StressSceneContent();

    StressSceneContent(const StressSceneContent& other)            = delete;
    StressSceneContent& operator=(const StressSceneContent& other) = delete;

    StressSceneContent(StressSceneContent&& other)            = delete;
    StressSceneContent& operator=(StressSceneContent&& other) = delete;

    void set_active_counts(const StressSceneCounts& counts);

    const StressSceneCounts& get_active_counts() const;

    const StressSceneCounts& get_max_counts() const;

    void step(double delta_time);

    void render();

  private:
    struct GeneratedTexture
    {
        std::vector<std::uint8_t> pixels_;
        std::int32_t              width_;
        std::int32_t              height_;
    };

    struct SpriteEntity
    {
        std::unique_ptr<rinvid::Texture> texture_;
        std::unique_ptr<rinvid::Sprite>  sprite_;
        rinvid::Vector2f                 position_;
        rinvid::Vector2f                 velocity_;
        float                            angular_velocity_;
        std::int32_t                     width_;
        std::int32_t                     height_;
    };

    struct TextEntity
    {
        std::unique_ptr<rinvid::Text> text_;
        rinvid::Vector2f              position_;
        rinvid::Vector2f              velocity_;
        float                         max_width_;
        float                         estimated_height_;
    };

    using ShapeVariant =
        std::variant<std::unique_ptr<rinvid::RectangleShape>, std::unique_ptr<rinvid::CircleShape>,
                     std::unique_ptr<rinvid::TriangleShape>>;

    struct ShapeEntity
    {
        ShapeVariant     shape_;
        rinvid::Vector2f center_;
        rinvid::Vector2f velocity_;
        float            angular_velocity_;
        float            extent_x_;
        float            extent_y_;
    };

    struct LightEntity
    {
        std::unique_ptr<rinvid::Light> light_;
        rinvid::Vector2f               position_;
        rinvid::Vector2f               velocity_;
    };

    GeneratedTexture make_texture(std::mt19937& generator);
    SpriteEntity     make_sprite_entity(std::mt19937& generator);
    TextEntity       make_text_entity(std::mt19937& generator, const std::string& font_path,
                                      std::uint32_t index);
    ShapeEntity      make_shape_entity(std::mt19937& generator);
    LightEntity      make_light_entity(std::mt19937& generator);

    std::uint32_t             viewport_width_;
    std::uint32_t             viewport_height_;
    StressSceneCounts         max_counts_;
    StressSceneCounts         active_counts_;
    std::vector<SpriteEntity> sprites_;
    std::vector<TextEntity>   texts_;
    std::vector<ShapeEntity>  shapes_;
    std::vector<LightEntity>  lights_;
};

#endif // EXAMPLES_PERF_STRESS_STRESS_SCENE_H
