/**********************************************************************
 * Copyright (c) 2020 - 2022, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifdef __unix__
#include <unistd.h>
#else
#include <windows.h>
#endif

#include <chrono>
#include <iostream>

#include <SFML/Window.hpp>

#include "core/include/animation.h"
#include "core/include/application.h"
#include "core/include/camera.h"
#include "core/include/circle_shape.h"
#include "core/include/quad_shape.h"
#include "core/include/rectangle_shape.h"
#include "core/include/rinvid_gfx.h"
#include "core/include/rinvid_gl.h"
#include "core/include/screen.h"
#include "core/include/sprite.h"
#include "core/include/texture.h"
#include "core/include/triangle_shape.h"
#include "gui/include/button.h"
#include "util/include/collision_detection.h"
#include "util/include/vector3.h"

class TestingGrounds : public rinvid::Screen
{
  public:
    void create() override;
    void destroy() override;

  private:
    void update(double delta_time) override;

    bool                  quad_alive{true};
    rinvid::TriangleShape triangle{rinvid::Vector2<float>{400.0F, 200.0F},
                                   rinvid::Vector2<float>{300.0F, 100.0F},
                                   rinvid::Vector2<float>{500.0F, 100.0F}};
    rinvid::QuadShape     quad{
        rinvid::Vector2<float>{100.0F, 40.0F}, rinvid::Vector2<float>{150.0F, 40.0F},
        rinvid::Vector2<float>{160.0F, 90.0F}, rinvid::Vector2<float>{90.0F, 90.0F}};
    rinvid::RectangleShape rectangle{rinvid::Vector2<float>{350.0F, 35.0F}, 100.0F, 50.0F};
    rinvid::CircleShape    circle{rinvid::Vector2<float>{500.0F, 300.0f}, 100.0F};
    rinvid::Texture        texture{"examples/testing_grounds/resources/logo.png"};
    rinvid::Sprite         sprite{&texture, 100, 100, rinvid::Vector2<float>{200.0F, 200.0F},
                          rinvid::Vector2<float>{0.0F, 0.0F}};

    rinvid::Texture clock_texture{"examples/testing_grounds/resources/clck.png"};
    rinvid::Sprite  clock_sprite{&clock_texture, 100, 100, rinvid::Vector2<float>{650.0F, 450.0F},
                                rinvid::Vector2<float>{0.0F, 0.0F}};

    rinvid::Texture     button_texture{"examples/testing_grounds/resources/default_button.png"};
    rinvid::gui::Button button{};

    rinvid::Camera camera{};
};

void TestingGrounds::create()
{
    triangle.set_color(rinvid::Color{0.1F, 0.8F, 0.3F, 1.0F});
    quad.set_color(rinvid::Color{0.8F, 0.1F, 0.3F, 1.0F});
    rectangle.set_color(rinvid::Color{0.2F, 0.2F, 0.8F, 1.0F});
    rectangle.set_rotation(90.0F);
    circle.set_color(rinvid::Color{0.1F, 0.7F, 0.8F, 1.0F});

    auto              regions = clock_sprite.split_animation_frames(100, 100, 12, 1);
    rinvid::Animation clock_animation{20.0, regions, rinvid::AnimationMode::Looping};
    clock_sprite.add_animation("anim", clock_animation);
    clock_sprite.play("anim");
    clock_sprite.set_scale(1.5F);
    clock_sprite.set_opacity(0.3);

    button.setup(&button_texture, 100, 30, rinvid::Vector2<float>{250.0F, 450.0F});
    auto button_regions = button.split_animation_frames(100, 30, 3, 1);

    button.set_idle({button_regions.at(0)});
    button.set_mouse_hovering({button_regions.at(1)});
    button.set_clicked({button_regions.at(2)});
}

void TestingGrounds::update(double delta_time)
{
    rinvid::RinvidGfx::clear_screen(0.1F, 0.2F, 0.2F, 1.0F);

    if (intersects(triangle.bounding_rect(), quad.bounding_rect()))
    {
        quad_alive = false;
    }

    button.update(get_application()->get_mouse_pos());

    if (button.is_clicked())
    {
        quad.set_position(rinvid::Vector2<float>{100.0F, 40.0F});
        quad_alive = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        camera.move(rinvid::Vector2<float>{-480.0F * static_cast<float>(delta_time), 0.0F});
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        camera.move(rinvid::Vector2<float>{480.0F * static_cast<float>(delta_time), 0.0F});
    }

    camera.update();

    triangle.draw();
    if (quad_alive)
    {
        quad.draw();
    }
    rectangle.draw();
    circle.draw();
    sprite.draw();
    clock_sprite.draw(delta_time);
    button.draw(delta_time);

    triangle.move(rinvid::Vector2<float>{120.0F * static_cast<float>(delta_time), 0.0F});
    rinvid::Vector2<float> triangle_origin = triangle.get_origin();
    if (triangle_origin.x >= rinvid::RinvidGfx::get_width())
    {
        triangle.set_position(rinvid::Vector2<float>{0.0F, triangle_origin.y});
    }

    quad.move(rinvid::Vector2<float>{0.0F, 240.0F * static_cast<float>(delta_time)});
    rinvid::Vector2<float> quad_origin = quad.get_origin();
    if (quad_origin.y >= rinvid::RinvidGfx::get_height())
    {
        quad.set_position(rinvid::Vector2<float>{quad_origin.x, 0.0F});
    }

    sprite.rotate(60.0F * static_cast<float>(delta_time));

    circle.scale(0.99F);
}

void TestingGrounds::destroy()
{
}

int main()
{
    rinvid::Application testing_grounds_app{800, 600, "Rinvid testing grounds"};
    TestingGrounds      testing_grounds_screen{};
    testing_grounds_app.set_screen(&testing_grounds_screen);
    testing_grounds_app.set_fps(120);
    testing_grounds_app.run();

    return 0;
}
