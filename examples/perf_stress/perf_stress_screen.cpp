/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include "perf_stress_screen.h"

#include <memory>
#include <string>
#include <utility>

#include <rinvid/core/text.h>
#include <rinvid/core/ttf_lib.h>
#include <rinvid/system/application.h>
#include <rinvid/system/keyboard.h>

#include "stress_scene.h"

namespace
{

constexpr std::uint32_t DEFAULT_PRESET_COUNT{50U};
constexpr float         STATUS_TEXT_X{16.0F};
constexpr float         STATUS_TEXT_Y{18.0F};
constexpr float         STATUS_TEXT_WIDTH{1280.0F};

} // namespace

PerfStressScreen::PerfStressScreen(std::string font_path)
    : font_path_{std::move(font_path)}, stress_scene_{nullptr}, status_text_{nullptr},
      active_preset_count_{DEFAULT_PRESET_COUNT}
{
}

PerfStressScreen::~PerfStressScreen() = default;

void PerfStressScreen::create()
{
    const auto& render_context = get_render_context();
    stress_scene_              = std::make_unique<StressSceneContent>(
        font_path_, static_cast<std::uint32_t>(render_context.get_width()),
        static_cast<std::uint32_t>(render_context.get_height()),
        make_uniform_stress_scene_counts(100U));
    status_text_ = std::make_unique<rinvid::Text>("", font_path_,
                                                  rinvid::Vector2f{STATUS_TEXT_X, STATUS_TEXT_Y},
                                                  rinvid::Color{0U, 255U, 0U, 255U}, 20U);
    status_text_->set_max_width(STATUS_TEXT_WIDTH);

    apply_uniform_preset(active_preset_count_);
}

void PerfStressScreen::destroy()
{
    status_text_.reset();
    stress_scene_.reset();
    rinvid::TTFLib::destroy();
}

void PerfStressScreen::update(double delta_time)
{
    if (rinvid::system::Keyboard::is_key_pressed(rinvid::system::Keyboard::Key::Escape))
    {
        get_application()->exit();
        return;
    }

    if (rinvid::system::Keyboard::is_key_pressed(rinvid::system::Keyboard::Key::Num1))
    {
        apply_uniform_preset(10U);
    }
    else if (rinvid::system::Keyboard::is_key_pressed(rinvid::system::Keyboard::Key::Num2))
    {
        apply_uniform_preset(25U);
    }
    else if (rinvid::system::Keyboard::is_key_pressed(rinvid::system::Keyboard::Key::Num3))
    {
        apply_uniform_preset(50U);
    }
    else if (rinvid::system::Keyboard::is_key_pressed(rinvid::system::Keyboard::Key::Num4))
    {
        apply_uniform_preset(100U);
    }

    stress_scene_->step(delta_time);
    stress_scene_->render();
    status_text_->draw();
}

void PerfStressScreen::apply_uniform_preset(std::uint32_t count)
{
    active_preset_count_ = count;
    stress_scene_->set_active_counts(make_uniform_stress_scene_counts(count));
    update_status_text();
}

void PerfStressScreen::update_status_text()
{
    const std::string message =
        "Perf stress example. Press 1=10, 2=25, 3=50, 4=100, Esc=quit. Current: " +
        format_stress_scene_counts(make_uniform_stress_scene_counts(active_preset_count_));
    status_text_->set_text(message);
}
