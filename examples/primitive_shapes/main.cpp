/**********************************************************************
 * Copyright (c) 2021, Lazar Lukic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <unistd.h>

#include <SFML/Window.hpp>

#include "core/include/circle_shape.h"
#include "core/include/quad_shape.h"
#include "core/include/rectangle_shape.h"
#include "core/include/rinvid_gfx.h"
#include "core/include/triangle_shape.h"
#include "util/include/vector2.h"

void handle_events(sf::Window& window, sf::Event& event);

void change_current_shape(rinvid::Shape*& current_shape, rinvid::Shape& next_shape,
                          rinvid::Color& base_color, rinvid::Color& active_color);

void handle_movement(float& vertical_delta, float& horizontal_delta);

int main()
{
    sf::Window window(sf::VideoMode(800, 600), "Rinvid primitive shapes example");
    rinvid::RinvidGfx::set_viewport(0, 0, 800, 600);

    sf::Event event;

    window.setActive(true);

    float vertical_delta{0.0F};
    float horizontal_delta{0.0F};

    rinvid::Color base_color{0.1078431F, 0.6215686F, 0.5745098F, 1.0F};
    rinvid::Color active_color{0.3176470F, 0.8392156F, 0.7921568F, 1.0F};

    rinvid::TriangleShape triangle{rinvid::Vector2{400.0F, 500.0F}, rinvid::Vector2{425.0F, 550.0F},
                                   rinvid::Vector2{375.0F, 550.0F}};

    rinvid::QuadShape quad{rinvid::Vector2{100.0F, 260.0F}, rinvid::Vector2{150.0F, 260.0F},
                           rinvid::Vector2{160.0F, 310.0F}, rinvid::Vector2{90.0F, 310.0F}};

    rinvid::RectangleShape rectangle{rinvid::Vector2{400.0F, 50.0F}, 100.0F, 50.0F};

    rinvid::CircleShape circle{rinvid::Vector2{670.0F, 280.0f}, 40.0F};

    rinvid::Shape* current_shape = &triangle;

    triangle.set_color(active_color);
    quad.set_color(base_color);
    rectangle.set_color(base_color);
    circle.set_color(base_color);

    rinvid::RinvidGfx::init();

    while (window.isOpen())
    {
        handle_events(window, event);

        rinvid::RinvidGfx::clear_screen(0.2F, 0.4F, 0.4F, 1.0F);

        vertical_delta   = 0.0F;
        horizontal_delta = 0.0F;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
        {
            change_current_shape(current_shape, triangle, base_color, active_color);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
        {
            change_current_shape(current_shape, quad, base_color, active_color);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
        {
            change_current_shape(current_shape, rectangle, base_color, active_color);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
        {
            change_current_shape(current_shape, circle, base_color, active_color);
        }

        handle_movement(vertical_delta, horizontal_delta);

        current_shape->move(rinvid::Vector2{horizontal_delta, vertical_delta});

        triangle.draw();
        quad.draw();
        rectangle.draw();
        circle.draw();

        window.display();

        usleep(10000);
    }

    return 0;
}

void handle_events(sf::Window& window, sf::Event& event)
{
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::Resized:
                rinvid::RinvidGfx::set_viewport(0, 0, event.size.width, event.size.height);
                break;
            default:
                break;
        }
    }
}

void change_current_shape(rinvid::Shape*& current_shape, rinvid::Shape& next_shape,
                          rinvid::Color& base_color, rinvid::Color& active_color)
{
    current_shape->set_color(base_color);
    current_shape = &next_shape;
    current_shape->set_color(active_color);
}

void handle_movement(float& vertical_delta, float& horizontal_delta)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        horizontal_delta += 5.0F;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        horizontal_delta -= 5.0F;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        vertical_delta -= 5.0F;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        vertical_delta += 5.0F;
    }
}
