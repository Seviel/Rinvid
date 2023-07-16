/**********************************************************************
 * Copyright (c) 2021 - 2023, Lazar Lukic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include "core/include/application.h"
#include "core/include/circle_shape.h"
#include "core/include/quad_shape.h"
#include "core/include/rectangle_shape.h"
#include "core/include/rinvid_gfx.h"
#include "core/include/rinvid_gl.h"
#include "core/include/screen.h"
#include "core/include/triangle_shape.h"
#include "system/include/keyboard.h"
#include "util/include/vector2.h"

using namespace rinvid::system;

class ShapesScreen : public rinvid::Screen
{
  public:
    void create() override;
    void destroy() override;

  private:
    void update(double delta_time) override;
    void change_current_shape(rinvid::Shape*& current_shape, rinvid::Shape& next_shape,
                              rinvid::Color& base_color, rinvid::Color& active_color);
    void handle_movement(float& vertical_delta, float& horizontal_delta);

    float                 vertical_delta{0.0F};
    float                 horizontal_delta{0.0F};
    rinvid::Color         base_color{0.1078431F, 0.6215686F, 0.5745098F, 1.0F};
    rinvid::Color         active_color{1.0000000F, 0.6705882F, 0.2862745F, 1.0F};
    rinvid::TriangleShape triangle{rinvid::Vector2<float>{400.0F, 500.0F},
                                   rinvid::Vector2<float>{425.0F, 550.0F},
                                   rinvid::Vector2<float>{375.0F, 550.0F}};
    rinvid::QuadShape     quad{
        rinvid::Vector2<float>{100.0F, 260.0F}, rinvid::Vector2<float>{150.0F, 260.0F},
        rinvid::Vector2<float>{160.0F, 310.0F}, rinvid::Vector2<float>{90.0F, 310.0F}};
    rinvid::RectangleShape rectangle{rinvid::Vector2<float>{400.0F, 50.0F}, 100.0F, 50.0F};
    rinvid::CircleShape    circle{rinvid::Vector2<float>{670.0F, 280.0f}, 40.0F};
    rinvid::Shape*         current_shape = nullptr;
};

void ShapesScreen::create()
{
    triangle.set_color(active_color);
    quad.set_color(base_color);
    rectangle.set_color(base_color);
    circle.set_color(base_color);
    current_shape = &triangle;
}

void ShapesScreen::update(double delta_time)
{
    rinvid::RinvidGfx::clear_screen(0.2F, 0.4F, 0.4F, 1.0F);

    vertical_delta   = 0.0F;
    horizontal_delta = 0.0F;

    if (Keyboard::is_key_pressed(Keyboard::Key::Num1))
    {
        change_current_shape(current_shape, triangle, base_color, active_color);
    }
    if (Keyboard::is_key_pressed(Keyboard::Key::Num2))
    {
        change_current_shape(current_shape, quad, base_color, active_color);
    }
    if (Keyboard::is_key_pressed(Keyboard::Key::Num3))
    {
        change_current_shape(current_shape, rectangle, base_color, active_color);
    }
    if (Keyboard::is_key_pressed(Keyboard::Key::Num4))
    {
        change_current_shape(current_shape, circle, base_color, active_color);
    }

    handle_movement(vertical_delta, horizontal_delta);

    current_shape->move(rinvid::Vector2<float>{horizontal_delta * static_cast<float>(delta_time),
                                               vertical_delta * static_cast<float>(delta_time)});

    triangle.draw();
    quad.draw();
    rectangle.draw();
    circle.draw();
}

void ShapesScreen::destroy()
{
}

void ShapesScreen::change_current_shape(rinvid::Shape*& current_shape, rinvid::Shape& next_shape,
                                        rinvid::Color& base_color, rinvid::Color& active_color)
{
    current_shape->set_color(base_color);
    current_shape = &next_shape;
    current_shape->set_color(active_color);
}

void ShapesScreen::handle_movement(float& vertical_delta, float& horizontal_delta)
{
    if (Keyboard::is_key_pressed(Keyboard::Key::D) ||
        Keyboard::is_key_pressed(Keyboard::Key::Right))
    {
        horizontal_delta += 500.0F;
    }

    if (Keyboard::is_key_pressed(Keyboard::Key::A) || Keyboard::is_key_pressed(Keyboard::Key::Left))
    {
        horizontal_delta -= 500.0F;
    }

    if (Keyboard::is_key_pressed(Keyboard::Key::W) || Keyboard::is_key_pressed(Keyboard::Key::Up))
    {
        vertical_delta -= 500.0F;
    }

    if (Keyboard::is_key_pressed(Keyboard::Key::S) || Keyboard::is_key_pressed(Keyboard::Key::Down))
    {
        vertical_delta += 500.0F;
    }
}

int main()
{
    rinvid::Application shapes_app{800, 600, "Rinvid primitive shapes example"};
    ShapesScreen        shapes_screen{};
    shapes_app.set_screen(&shapes_screen);
    shapes_app.set_fps(120);
    shapes_app.run();

    return 0;
}
