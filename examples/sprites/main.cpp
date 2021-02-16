/**********************************************************************
 * Copyright (c) 2021, Lazar Lukic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <chrono>
#include <unistd.h>

#include <SFML/Window.hpp>

#include "core/include/animation.h"
#include "core/include/rinvid_gfx.h"
#include "core/include/sprite.h"
#include "core/include/texture.h"
#include "util/include/vector2.h"

void handle_events(sf::Window& window, sf::Event& event);

void handle_inputs(float& horizontal_delta);

bool handle_animation_state(std::string& action, std::string& direction, float horizontal_delta,
                            bool is_animation_finished);

int main()
{
    std::chrono::duration<double> delta_time{};

    sf::Window window(sf::VideoMode(800, 600), "Sprites example");
    rinvid::RinvidGfx::set_viewport(0, 0, 800, 600);

    sf::Event event;

    window.setActive(true);

    float horizontal_delta{0.0F};

    std::string action{"idle"};
    std::string direction{"right"};
    std::string current_animation_state{action + "_" + direction};

    rinvid::Color base_color{0.1078431F, 0.6215686F, 0.5745098F, 1.0F};
    rinvid::Color active_color{1.0000000F, 0.6705882F, 0.2862745F, 1.0F};

    rinvid::Texture robot_texture{"examples/sprites/resources/trashbot.png"};
    rinvid::Sprite  robot_sprite{&robot_texture, 455, 455, rinvid::Vector2{0.0F, 100.0F},
                                rinvid::Vector2{0.0F, 0.0F}};

    robot_sprite.split_animation_frames(455, 455, 9, 8);

    auto idle_right = robot_sprite.get_regions({0});
    auto running_right =
        robot_sprite.get_regions({1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15,
                                  16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30});
    auto attack_right = robot_sprite.get_regions({31, 32, 33, 34, 34, 34});

    rinvid::Animation idle_right_animation{0.0, idle_right, rinvid::AnimationMode::Normal};
    rinvid::Animation running_right_animation{20.0, running_right, rinvid::AnimationMode::Looping};
    rinvid::Animation attack_right_animation{30.0, attack_right, rinvid::AnimationMode::Normal};

    robot_sprite.add_animation("idle_right", idle_right_animation);
    robot_sprite.add_animation("running_right", running_right_animation);
    robot_sprite.add_animation("attack_right", attack_right_animation);

    auto idle_left   = robot_sprite.get_regions({35});
    auto attack_left = robot_sprite.get_regions({66, 67, 68, 69, 69, 69});
    auto running_left =
        robot_sprite.get_regions({36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
                                  51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65});

    rinvid::Animation idle_left_animation{0.0, idle_left, rinvid::AnimationMode::Normal};
    rinvid::Animation running_left_animation{20.0, running_left, rinvid::AnimationMode::Looping};
    rinvid::Animation attack_left_animation{30.0, attack_left, rinvid::AnimationMode::Normal};

    robot_sprite.add_animation("idle_left", idle_left_animation);
    robot_sprite.add_animation("running_left", running_left_animation);
    robot_sprite.add_animation("attack_left", attack_left_animation);

    robot_sprite.play(current_animation_state);

    rinvid::RinvidGfx::init();

    while (window.isOpen())
    {
        auto start = std::chrono::high_resolution_clock::now();

        handle_events(window, event);

        rinvid::RinvidGfx::clear_screen(0.2F, 0.4F, 0.4F, 1.0F);

        horizontal_delta = 0.0F;

        robot_sprite.draw(delta_time.count());

        handle_inputs(horizontal_delta);

        bool state_changed = handle_animation_state(action, direction, horizontal_delta,
                                                    robot_sprite.is_animation_finished());

        if (state_changed == true)
        {
            current_animation_state = action + "_" + direction;
            robot_sprite.play(current_animation_state);
        }

        robot_sprite.move(rinvid::Vector2{horizontal_delta, 0});

        window.display();

        usleep(10000);

        auto end   = std::chrono::high_resolution_clock::now();
        delta_time = end - start;
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

void handle_inputs(float& horizontal_delta)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        horizontal_delta += 4.8F;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        horizontal_delta -= 4.8F;
    }
}

bool handle_animation_state(std::string& action, std::string& direction, float horizontal_delta,
                            bool is_animation_finished)
{
    std::string current_animation_state{action + "_" + direction};
    std::string new_animation_state{action + "_" + direction};

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::F) ||
        (is_animation_finished == false && action == "attack"))
    {
        action = "attack";
    }
    else
    {
        if (horizontal_delta != 0)
        {
            action = "running";
        }
        else
        {
            action = "idle";
        }
    }

    if (horizontal_delta > 0)
    {
        direction = "right";
    }

    if (horizontal_delta < 0)
    {
        direction = "left";
    }

    new_animation_state = action + "_" + direction;

    if (current_animation_state != new_animation_state)
    {
        return true;
    }
    else
    {
        return false;
    }
}
