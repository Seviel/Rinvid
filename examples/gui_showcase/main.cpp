/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <rinvid/core/render_context.h>
#include <rinvid/core/texture.h>
#include <rinvid/core/ttf_lib.h>
#include <rinvid/gui/button.h>
#include <rinvid/gui/check_button.h>
#include <rinvid/gui/label.h>
#include <rinvid/gui/panel.h>
#include <rinvid/gui/progress_bar.h>
#include <rinvid/gui/slider.h>
#include <rinvid/gui/stack_layout.h>
#include <rinvid/gui/text_button.h>
#include <rinvid/system/application.h>
#include <rinvid/system/keyboard.h>
#include <rinvid/system/screen.h>
#include <rinvid/util/color.h>
#include <rinvid/util/vector2.h>

namespace
{

constexpr const char*   FONT_PATH{"resources/aquifer.ttf"};
constexpr std::int32_t  BUTTON_WIDTH{170};
constexpr std::int32_t  BUTTON_HEIGHT{36};
constexpr std::uint32_t BUTTON_STATE_COUNT{3U};
constexpr std::uint32_t CHECK_BUTTON_GROUP_COUNT{2U};

struct PixelColor
{
    std::uint8_t r_;
    std::uint8_t g_;
    std::uint8_t b_;
    std::uint8_t a_;
};

void set_pixel(std::vector<std::uint8_t>& pixels, std::int32_t texture_width, std::int32_t x,
               std::int32_t y, PixelColor color)
{
    const std::size_t index{static_cast<std::size_t>((y * texture_width + x) * 4)};
    pixels.at(index)     = color.r_;
    pixels.at(index + 1) = color.g_;
    pixels.at(index + 2) = color.b_;
    pixels.at(index + 3) = color.a_;
}

void fill_button_frame(std::vector<std::uint8_t>& pixels, std::int32_t texture_width,
                       std::int32_t frame_index, PixelColor fill, PixelColor border)
{
    const std::int32_t start_x{frame_index * BUTTON_WIDTH};
    for (std::int32_t y{0}; y < BUTTON_HEIGHT; ++y)
    {
        for (std::int32_t x{0}; x < BUTTON_WIDTH; ++x)
        {
            const bool is_border{x < 2 || x >= BUTTON_WIDTH - 2 || y < 2 || y >= BUTTON_HEIGHT - 2};
            set_pixel(pixels, texture_width, start_x + x, y, is_border ? border : fill);
        }
    }
}

std::vector<std::uint8_t> make_button_atlas()
{
    const std::int32_t texture_width{
        BUTTON_WIDTH * static_cast<std::int32_t>(BUTTON_STATE_COUNT * CHECK_BUTTON_GROUP_COUNT)};
    std::vector<std::uint8_t> pixels(static_cast<std::size_t>(texture_width * BUTTON_HEIGHT * 4),
                                     255U);

    fill_button_frame(pixels, texture_width, 0, PixelColor{56U, 118U, 168U, 255U},
                      PixelColor{24U, 53U, 80U, 255U});
    fill_button_frame(pixels, texture_width, 1, PixelColor{73U, 145U, 199U, 255U},
                      PixelColor{30U, 73U, 112U, 255U});
    fill_button_frame(pixels, texture_width, 2, PixelColor{35U, 88U, 130U, 255U},
                      PixelColor{18U, 42U, 65U, 255U});
    fill_button_frame(pixels, texture_width, 3, PixelColor{46U, 146U, 94U, 255U},
                      PixelColor{22U, 78U, 50U, 255U});
    fill_button_frame(pixels, texture_width, 4, PixelColor{60U, 175U, 113U, 255U},
                      PixelColor{25U, 101U, 62U, 255U});
    fill_button_frame(pixels, texture_width, 5, PixelColor{31U, 112U, 70U, 255U},
                      PixelColor{14U, 65U, 38U, 255U});

    return pixels;
}

std::string percent_text(float value)
{
    return std::to_string(static_cast<std::int32_t>(std::round(value))) + "%";
}

} // namespace

class GuiShowcaseScreen : public rinvid::Screen
{
  public:
    void create() override;
    void destroy() override;

  private:
    void update(double delta_time) override;
    void update_controls(double delta_time);
    void update_readouts();
    void draw_gui(double delta_time);

    std::vector<std::uint8_t> button_atlas_pixels_{make_button_atlas()};
    rinvid::Texture           button_texture_{
        button_atlas_pixels_,
        BUTTON_WIDTH* static_cast<std::int32_t>(BUTTON_STATE_COUNT* CHECK_BUTTON_GROUP_COUNT),
        BUTTON_HEIGHT};

    rinvid::gui::Label title_{"GUI Showcase", FONT_PATH, {32.0F, 42.0F}, 0xFFFFFFFF, 28U};
    rinvid::gui::Label subtitle_{"A compact playground for Rinvid's basic GUI controls.",
                                 FONT_PATH,
                                 {32.0F, 72.0F},
                                 0xD8E6FFFF,
                                 14U,
                                 820.0F};

    rinvid::gui::Panel       controls_panel_{{32.0F, 110.0F}, 380.0F, 430.0F};
    rinvid::gui::Panel       preview_panel_{{440.0F, 110.0F}, 380.0F, 430.0F};
    rinvid::gui::StackLayout controls_layout_{{0.0F, 0.0F}};
    rinvid::gui::StackLayout preview_layout_{{0.0F, 0.0F}};

    rinvid::gui::Label       controls_title_{"Controls", FONT_PATH, {0.0F, 0.0F},
                                       0xFFFFFFFF, 18U,       320.0F};
    rinvid::gui::Label       slider_label_{"Slider value", FONT_PATH, {0.0F, 0.0F},
                                     0xBFD2E6FF,     13U,       320.0F};
    rinvid::gui::Slider      value_slider_{{0.0F, 0.0F}, 320.0F, 0.0F, 100.0F, 55.0F};
    rinvid::gui::Label       speed_label_{"Auto-fill speed", FONT_PATH, {0.0F, 0.0F},
                                    0xBFD2E6FF,        13U,       320.0F};
    rinvid::gui::Slider      speed_slider_{{0.0F, 0.0F}, 320.0F, 0.1F, 1.0F, 0.35F};
    rinvid::gui::CheckButton auto_fill_check_{"Auto fill", FONT_PATH, 0xFFFFFFFF, 13U};
    rinvid::gui::CheckButton controls_active_check_{"Controls active", FONT_PATH, 0xFFFFFFFF, 13U};
    rinvid::gui::TextButton  reset_button_{"Reset", FONT_PATH, 0xFFFFFFFF, 13U};

    rinvid::gui::Label preview_title_{"Preview", FONT_PATH, {0.0F, 0.0F}, 0xFFFFFFFF, 18U, 320.0F};
    rinvid::gui::Label value_readout_{"Slider value: 55%", FONT_PATH, {0.0F, 0.0F},
                                      0xD8E6FFFF,          13U,       320.0F};
    rinvid::gui::ProgressBar value_bar_{{0.0F, 0.0F}, 320.0F, 20.0F, 0.0F, 100.0F, 55.0F};
    rinvid::gui::Label       timer_readout_{"Auto progress: 0%", FONT_PATH, {0.0F, 0.0F},
                                      0xD8E6FFFF,          13U,       320.0F};
    rinvid::gui::ProgressBar auto_bar_{{0.0F, 0.0F}, 320.0F, 20.0F};
    rinvid::gui::Label       plain_button_label_{"Plain Button pulse", FONT_PATH, {0.0F, 0.0F},
                                           0xBFD2E6FF,           13U,       320.0F};
    rinvid::gui::Button      pulse_button_{};
    rinvid::gui::Label status_label_{"Ready", FONT_PATH, {0.0F, 0.0F}, 0xFFFFFFFF, 13U, 320.0F};

    float auto_progress_{0.0F};
};

void GuiShowcaseScreen::create()
{
    controls_panel_.set_background_color(0x18202DCC);
    controls_panel_.set_border_color(0x5D7390FF);
    controls_panel_.set_border_thickness(2.0F);
    controls_panel_.set_padding(18.0F);

    preview_panel_.set_background_color(0x151B24CC);
    preview_panel_.set_border_color(0x5D7390FF);
    preview_panel_.set_border_thickness(2.0F);
    preview_panel_.set_padding(18.0F);

    controls_layout_.set_spacing(14.0F);
    controls_layout_.add(&controls_title_);
    controls_layout_.add(&slider_label_);
    controls_layout_.add(&value_slider_);
    controls_layout_.add(&speed_label_);
    controls_layout_.add(&speed_slider_);
    controls_layout_.add(&auto_fill_check_);
    controls_layout_.add(&controls_active_check_);
    controls_layout_.add(&reset_button_);

    preview_layout_.set_spacing(14.0F);
    preview_layout_.add(&preview_title_);
    preview_layout_.add(&value_readout_);
    preview_layout_.add(&value_bar_);
    preview_layout_.add(&timer_readout_);
    preview_layout_.add(&auto_bar_);
    preview_layout_.add(&plain_button_label_);
    preview_layout_.add(&pulse_button_);
    preview_layout_.add(&status_label_);

    controls_panel_.add(&controls_layout_, {0.0F, 0.0F});
    preview_panel_.add(&preview_layout_, {0.0F, 0.0F});

    auto_fill_check_.setup_from_atlas(&button_texture_, BUTTON_WIDTH, BUTTON_HEIGHT, {0.0F, 0.0F});
    controls_active_check_.setup_from_atlas(&button_texture_, BUTTON_WIDTH, BUTTON_HEIGHT,
                                            {0.0F, 0.0F});
    reset_button_.setup_from_atlas(&button_texture_, BUTTON_WIDTH, BUTTON_HEIGHT, {0.0F, 0.0F});
    pulse_button_.setup_from_atlas(&button_texture_, BUTTON_WIDTH, BUTTON_HEIGHT, {0.0F, 0.0F});
    controls_layout_.layout();
    preview_layout_.layout();

    controls_active_check_.set_checked(true);

    value_slider_.set_step(1.0F);
    speed_slider_.set_step(0.05F);

    value_bar_.set_border_color(0x92A8C0FF);
    value_bar_.set_border_thickness(1.0F);
    auto_bar_.set_border_color(0x92A8C0FF);
    auto_bar_.set_border_thickness(1.0F);
}

void GuiShowcaseScreen::update(double delta_time)
{
    get_render_context().clear_screen(0.055F, 0.074F, 0.102F, 1.0F);

    if (rinvid::system::Keyboard::is_key_pressed(rinvid::system::Keyboard::Key::Escape))
    {
        get_application()->exit();
    }

    update_controls(delta_time);
    update_readouts();
    draw_gui(delta_time);
}

void GuiShowcaseScreen::update_controls(double delta_time)
{
    controls_panel_.update(delta_time);
    preview_panel_.update(delta_time);

    const bool controls_active{controls_active_check_.is_checked()};
    value_slider_.set_enabled(controls_active);
    speed_slider_.set_enabled(controls_active);
    auto_fill_check_.set_enabled(controls_active);
    reset_button_.set_enabled(controls_active);
    pulse_button_.set_enabled(controls_active);

    if (reset_button_.was_activated())
    {
        value_slider_.set_value(55.0F);
        speed_slider_.set_value(0.35F);
        auto_progress_ = 0.0F;
        status_label_.set_text("Reset clicked");
    }

    if (pulse_button_.was_activated())
    {
        auto_progress_ += 0.15F;
        if (auto_progress_ > 1.0F)
        {
            auto_progress_ -= 1.0F;
        }
        status_label_.set_text("Plain button clicked");
    }

    if (auto_fill_check_.was_toggled())
    {
        status_label_.set_text(auto_fill_check_.is_checked() ? "Auto fill on" : "Auto fill off");
    }

    if (controls_active_check_.was_toggled())
    {
        status_label_.set_text(controls_active ? "Controls enabled" : "Controls disabled");
    }

    if (controls_active && auto_fill_check_.is_checked())
    {
        auto_progress_ += static_cast<float>(delta_time) * speed_slider_.get_value();
        if (auto_progress_ > 1.0F)
        {
            auto_progress_ -= 1.0F;
        }
    }

    value_bar_.set_value(value_slider_.get_value());
    auto_bar_.set_progress(auto_progress_);
}

void GuiShowcaseScreen::update_readouts()
{
    const bool  controls_active{controls_active_check_.is_checked()};
    const float value{value_slider_.get_value()};

    slider_label_.set_text("Slider value: " + percent_text(value));
    value_readout_.set_text("Slider value: " + percent_text(value));
    timer_readout_.set_text("Auto progress: " + percent_text(auto_progress_ * 100.0F));

    value_bar_.set_fill_color(controls_active ? rinvid::Color{0x4AA3FFFF}
                                              : rinvid::Color{0x777E88FF});
    auto_bar_.set_fill_color(auto_fill_check_.is_checked() ? rinvid::Color{0x4BD18DFF}
                                                           : rinvid::Color{0x8793A1FF});
}

void GuiShowcaseScreen::draw_gui(double delta_time)
{
    title_.draw();
    subtitle_.draw();
    controls_panel_.draw(delta_time);
    preview_panel_.draw(delta_time);
}

void GuiShowcaseScreen::destroy()
{
    rinvid::TTFLib::destroy();
}

int main()
{
    rinvid::Application gui_showcase_app{852, 580, "Rinvid GUI showcase"};
    gui_showcase_app.set_screen(std::make_unique<GuiShowcaseScreen>());
    gui_showcase_app.set_fps(120);
    gui_showcase_app.run();

    return 0;
}
