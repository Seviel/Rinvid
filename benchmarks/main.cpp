/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <cstdint>
#include <memory>
#include <random>
#include <string>
#include <utility>
#include <vector>

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Window.hpp>

#include <benchmark/benchmark.h>

#include <rinvid/core/rinvid_gl.h>
#include <rinvid/core/sprite.h>
#include <rinvid/core/texture.h>
#include <rinvid/system/application.h>

#include "stress_scene.h"

namespace
{

constexpr std::uint32_t BENCHMARK_WINDOW_WIDTH{1280U};
constexpr std::uint32_t BENCHMARK_WINDOW_HEIGHT{720U};
constexpr std::uint32_t PINNED_CATEGORY_COUNT{100U};
constexpr double        FIXED_DELTA_TIME{1.0 / 60.0};
constexpr const char*   FONT_RESOURCE_PATH{"resources/aquifer.ttf"};
constexpr std::int32_t  MIN_TEXTURE_SIZE{50};
constexpr std::int32_t  MAX_TEXTURE_SIZE{500};

enum class VaryingCategory
{
    Sprites,
    Shapes,
    Text,
    Lights
};

struct SpriteSetup
{
    rinvid::Vector2f position_;
    std::int32_t     width_;
    std::int32_t     height_;
    float            rotation_;
    float            opacity_;
};

struct GeneratedTexture
{
    std::vector<std::uint8_t> pixels_;
    std::int32_t              width_;
    std::int32_t              height_;
};

SpriteSetup make_sprite_setup(std::mt19937& generator)
{
    std::uniform_real_distribution<float> x_distribution{
        0.0F, static_cast<float>(BENCHMARK_WINDOW_WIDTH)};
    std::uniform_real_distribution<float> y_distribution{
        0.0F, static_cast<float>(BENCHMARK_WINDOW_HEIGHT)};
    std::uniform_real_distribution<float> rotation_distribution{0.0F, 360.0F};
    std::uniform_real_distribution<float> opacity_distribution{0.35F, 1.0F};

    SpriteSetup setup{};
    setup.position_ = rinvid::Vector2f{x_distribution(generator), y_distribution(generator)};
    setup.rotation_ = rotation_distribution(generator);
    setup.opacity_  = opacity_distribution(generator);
    return setup;
}

GeneratedTexture make_texture(std::mt19937& generator)
{
    std::uniform_int_distribution<std::int32_t>  size_distribution{MIN_TEXTURE_SIZE,
                                                                  MAX_TEXTURE_SIZE};
    std::uniform_int_distribution<std::uint16_t> color_distribution{0U, 255U};

    GeneratedTexture texture{};
    texture.width_  = size_distribution(generator);
    texture.height_ = size_distribution(generator);
    texture.pixels_.resize(static_cast<std::size_t>(texture.width_) *
                           static_cast<std::size_t>(texture.height_) * 4U);

    for (std::size_t i{0U}; i < texture.pixels_.size(); i += 4U)
    {
        texture.pixels_[i]     = static_cast<std::uint8_t>(color_distribution(generator));
        texture.pixels_[i + 1] = static_cast<std::uint8_t>(color_distribution(generator));
        texture.pixels_[i + 2] = static_cast<std::uint8_t>(color_distribution(generator));
        texture.pixels_[i + 3] = 255U;
    }

    return texture;
}

class SpriteSceneBenchmark : public benchmark::Fixture
{
  public:
    void SetUp(const benchmark::State& state) override
    {
        const auto sprite_count = static_cast<std::size_t>(state.range(0));

        application_ = std::make_unique<rinvid::Application>(
            BENCHMARK_WINDOW_WIDTH, BENCHMARK_WINDOW_HEIGHT, "Rinvid benchmark", false, 0U);

        textures_.clear();
        sprites_.clear();
        textures_.reserve(sprite_count);
        sprites_.reserve(sprite_count);

        std::mt19937 generator{1337U};

        for (std::size_t i{0U}; i < sprite_count; ++i)
        {
            auto generated_texture = make_texture(generator);
            auto texture           = std::make_unique<rinvid::Texture>(
                generated_texture.pixels_, generated_texture.width_, generated_texture.height_);
            auto setup    = make_sprite_setup(generator);
            setup.width_  = generated_texture.width_;
            setup.height_ = generated_texture.height_;
            auto sprite   = std::make_unique<rinvid::Sprite>(texture.get(), setup.width_,
                                                             setup.height_, setup.position_);

            sprite->set_rotation(setup.rotation_);
            sprite->set_opacity(setup.opacity_);

            textures_.emplace_back(std::move(texture));
            sprites_.emplace_back(std::move(sprite));
        }
    }

    void TearDown(const benchmark::State&) override
    {
        sprites_.clear();
        textures_.clear();
        application_.reset();
    }

  protected:
    void draw_scene()
    {
        auto& render_context = application_->get_render_context();
        render_context.clear_screen(0.0F, 0.0F, 0.0F, 1.0F);

        for (const auto& sprite : sprites_)
        {
            sprite->draw();
        }

        GL_CALL(glFinish());
    }

    std::size_t sprite_count() const
    {
        return sprites_.size();
    }

  private:
    std::unique_ptr<rinvid::Application>          application_;
    std::vector<std::unique_ptr<rinvid::Texture>> textures_;
    std::vector<std::unique_ptr<rinvid::Sprite>>  sprites_;
};

class FullStressBenchmarkRuntime
{
  public:
    FullStressBenchmarkRuntime()
        : application_{BENCHMARK_WINDOW_WIDTH, BENCHMARK_WINDOW_HEIGHT, "Rinvid full stress", false,
                       0U},
          stress_scene_{FONT_RESOURCE_PATH, BENCHMARK_WINDOW_WIDTH, BENCHMARK_WINDOW_HEIGHT,
                        make_uniform_stress_scene_counts(100U)}
    {
        stress_scene_.set_active_counts(make_uniform_stress_scene_counts(10U));
    }

    void run_frame(const StressSceneCounts& counts)
    {
        stress_scene_.set_active_counts(counts);

        sf::Event event{};
        while (application_.get_context().get_window()->pollEvent(event))
        {
        }

        stress_scene_.step(FIXED_DELTA_TIME);
        stress_scene_.render();
        application_.get_context().get_window()->display();
    }

  private:
    rinvid::Application application_;
    StressSceneContent  stress_scene_;
};

FullStressBenchmarkRuntime& get_full_stress_runtime()
{
    static FullStressBenchmarkRuntime runtime{};
    return runtime;
}

StressSceneCounts make_counts_with_varying_category(std::uint32_t   fixed_count,
                                                    std::uint32_t   varying_count,
                                                    VaryingCategory varying_category)
{
    StressSceneCounts counts = make_uniform_stress_scene_counts(fixed_count);

    switch (varying_category)
    {
        case VaryingCategory::Sprites:
            counts.sprite_count_ = varying_count;
            break;
        case VaryingCategory::Shapes:
            counts.shape_count_ = varying_count;
            break;
        case VaryingCategory::Text:
            counts.text_count_ = varying_count;
            break;
        case VaryingCategory::Lights:
            counts.light_count_ = varying_count;
            break;
    }

    return counts;
}

void run_full_stress_benchmark_case(benchmark::State& state, const StressSceneCounts& counts)
{
    auto& runtime = get_full_stress_runtime();

    runtime.run_frame(counts);
    state.SetLabel(format_stress_scene_counts(counts));

    for (auto _ : state)
    {
        runtime.run_frame(counts);
    }

    state.counters["frames_per_second"] =
        benchmark::Counter(static_cast<double>(state.iterations()), benchmark::Counter::kIsRate);
}

BENCHMARK_DEFINE_F(SpriteSceneBenchmark, DrawRandomSpriteScene)(benchmark::State& state)
{
    for (auto _ : state)
    {
        draw_scene();
    }

    state.SetItemsProcessed(static_cast<std::int64_t>(state.iterations()) *
                            static_cast<std::int64_t>(sprite_count()));
}

BENCHMARK_REGISTER_F(SpriteSceneBenchmark, DrawRandomSpriteScene)
    ->Arg(100)
    ->Arg(250)
    ->Arg(500)
    ->Arg(1000)
    ->Unit(benchmark::kMillisecond);

static void FullStressFrame(benchmark::State& state)
{
    const StressSceneCounts counts =
        make_uniform_stress_scene_counts(static_cast<std::uint32_t>(state.range(0)));

    run_full_stress_benchmark_case(state, counts);
}

static void FullStressFrameVarySprites(benchmark::State& state)
{
    const StressSceneCounts counts = make_counts_with_varying_category(
        PINNED_CATEGORY_COUNT, static_cast<std::uint32_t>(state.range(0)),
        VaryingCategory::Sprites);

    run_full_stress_benchmark_case(state, counts);
}

static void FullStressFrameVaryShapes(benchmark::State& state)
{
    const StressSceneCounts counts = make_counts_with_varying_category(
        PINNED_CATEGORY_COUNT, static_cast<std::uint32_t>(state.range(0)), VaryingCategory::Shapes);

    run_full_stress_benchmark_case(state, counts);
}

static void FullStressFrameVaryText(benchmark::State& state)
{
    const StressSceneCounts counts = make_counts_with_varying_category(
        PINNED_CATEGORY_COUNT, static_cast<std::uint32_t>(state.range(0)), VaryingCategory::Text);

    run_full_stress_benchmark_case(state, counts);
}

static void FullStressFrameVaryLights(benchmark::State& state)
{
    const StressSceneCounts counts = make_counts_with_varying_category(
        PINNED_CATEGORY_COUNT, static_cast<std::uint32_t>(state.range(0)), VaryingCategory::Lights);

    run_full_stress_benchmark_case(state, counts);
}

BENCHMARK(FullStressFrame)
    ->Arg(10)
    ->Arg(25)
    ->Arg(50)
    ->Arg(100)
    ->UseRealTime()
    ->Unit(benchmark::kMillisecond);

BENCHMARK(FullStressFrameVarySprites)
    ->Arg(10)
    ->Arg(25)
    ->Arg(50)
    ->Arg(100)
    ->UseRealTime()
    ->Unit(benchmark::kMillisecond);

BENCHMARK(FullStressFrameVaryShapes)
    ->Arg(10)
    ->Arg(25)
    ->Arg(50)
    ->Arg(100)
    ->UseRealTime()
    ->Unit(benchmark::kMillisecond);

BENCHMARK(FullStressFrameVaryText)
    ->Arg(10)
    ->Arg(25)
    ->Arg(50)
    ->Arg(100)
    ->UseRealTime()
    ->Unit(benchmark::kMillisecond);

BENCHMARK(FullStressFrameVaryLights)
    ->Arg(10)
    ->Arg(25)
    ->Arg(50)
    ->Arg(100)
    ->UseRealTime()
    ->Unit(benchmark::kMillisecond);

} // namespace

BENCHMARK_MAIN();
