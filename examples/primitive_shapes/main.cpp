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

int main()
{
    sf::Window window(sf::VideoMode(800, 600), "Rinvid primitive shapes example");
    rinvid::RinvidGfx::set_viewport(0, 0, 800, 600);

    sf::Event event;

    rinvid::Color base_color{0.1078431F, 0.6215686F, 0.5745098F, 1.0F};
    rinvid::Color active_color{0.3176470F, 0.8392156F, 0.7921568F, 1.0F};

    window.setActive(true);

    rinvid::TriangleShape triangle{rinvid::Vector2{400.0F, 500.0F}, rinvid::Vector2{425.0F, 550.0F},
                                   rinvid::Vector2{375.0F, 550.0F}};

    rinvid::QuadShape quad{rinvid::Vector2{100.0F, 260.0F}, rinvid::Vector2{150.0F, 260.0F},
                           rinvid::Vector2{160.0F, 310.0F}, rinvid::Vector2{90.0F, 310.0F}};

    rinvid::RectangleShape rectangle{rinvid::Vector2{400.0F, 50.0F}, 100.0F, 50.0F};

    rinvid::CircleShape circle{rinvid::Vector2{670.0F, 280.0f}, 40.0F};

    rinvid::Shape* current_shape = &triangle;

    triangle.set_color(base_color);
    quad.set_color(base_color);
    rectangle.set_color(base_color);
    circle.set_color(base_color);

    rinvid::RinvidGfx::init();

    while (window.isOpen())
    {
        // Cycle reset
        handle_events(window, event);

        rinvid::RinvidGfx::clear_screen(0.2F, 0.4F, 0.4F, 1.0F);

        // Handle inputs
        // Section change
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
        {
            current_shape = &triangle;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
        {
            current_shape = &quad;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
        {
            current_shape = &rectangle;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
        {
            current_shape = &circle;
        }

        // Reset colors
        triangle.set_color(base_color);
        quad.set_color(base_color);
        rectangle.set_color(base_color);
        circle.set_color(base_color);
        (*current_shape).set_color(active_color);

        // Draw shapes
        triangle.draw();
        quad.draw();
        rectangle.draw();
        circle.draw();

        // Cycle end
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
