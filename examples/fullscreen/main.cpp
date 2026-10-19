/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <cmath>
#include <memory>
#include <string>

#include <rinvid/core/circle_shape.h>
#include <rinvid/core/text.h>
#include <rinvid/core/texture.h>
#include <rinvid/gui/button.h>
#include <rinvid/gui/slider.h>
#include <rinvid/system/application.h>
#include <rinvid/system/keyboard.h>
#include <rinvid/system/mouse.h>

namespace
{
class FullscreenExample final : public rinvid::Screen
{
  public:
    void create() override
    {
        get_render_context().set_logical_size(800, 600);
        circle_.set_color({0.2F, 0.7F, 0.8F, 1.0F});
        pointer_.set_color({1.0F, 0.7F, 0.2F, 1.0F});
        button_.setup_from_atlas(&texture_, 100, 30, {350.0F, 450.0F});
    }

    void destroy() override
    {
    }

  private:
    void update(double delta_time) override
    {
        get_render_context().clear_screen(0.06F, 0.08F, 0.14F, 1.0F);
        if (rinvid::system::Keyboard::is_key_just_pressed(rinvid::system::Keyboard::Key::Escape))
        {
            get_application()->exit();
        }

        elapsed_ += delta_time;
        circle_.set_position({400.0F + 100.0F * static_cast<float>(std::sin(elapsed_)), 280.0F});
        circle_.draw();
        button_.update();
        slider_.update(delta_time);

        if (button_.was_activated())
        {
            ++clicks_;
        }

        status_.set_text("Clicks: " + std::to_string(clicks_) + "   Slider: " +
                         std::to_string(static_cast<std::uint32_t>(slider_.get_value())));
        heading_.draw();
        status_.draw();
        button_.draw();
        slider_.draw();

        if (const auto position{rinvid::system::Mouse::get_mouse_canvas_pos()})
        {
            pointer_.set_position(*position);
            pointer_.draw();
        }
    }

    std::uint32_t       clicks_{};
    double              elapsed_{};
    rinvid::CircleShape circle_{{400.0F, 280.0F}, 50.0F};
    rinvid::CircleShape pointer_{{0.0F, 0.0F}, 4.0F};
    rinvid::Texture     texture_{"resources/default_button.png"};
    rinvid::gui::Button button_{};
    rinvid::gui::Slider slider_{{250.0F, 510.0F}, 300.0F, 0.0F, 100.0F, 50.0F};
    rinvid::Text        heading_{
        "800 x 600 logical canvas - Escape to exit\n"
        "Resize in --windowed mode: the circle stays round.\n"
        "Click the button and drag the slider. Alt+Tab pauses motion.",
        "resources/aquifer.ttf",
        {24.0F, 35.0F},
        {1.0F, 1.0F, 1.0F, 1.0F},
        20U};
    rinvid::Text status_{
        "", "resources/aquifer.ttf", {250.0F, 420.0F}, {1.0F, 1.0F, 1.0F, 1.0F}, 20U};
};

} // namespace

int main(int argc, char** argv)
{
    bool fullscreen{true};

    for (std::int32_t i{1}; i < argc; ++i)
    {
        fullscreen = fullscreen && std::string{argv[i]} != "--windowed";
    }

    rinvid::Application application{1000U, 700U, "Rinvid fullscreen", fullscreen};
    application.set_vertical_sync(true);
    application.set_screen(std::make_unique<FullscreenExample>());
    application.run();

    return 0;
}
