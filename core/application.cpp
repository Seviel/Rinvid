/**********************************************************************
 * Copyright (c) 2021 - 2024, Filip Vasiljevic
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

#ifdef _WIN32
#include "util/include/windows_utils.h"
#endif // _WIN32
#include "core/include/rinvid_gfx.h"
#include "include/application.h"
#include "util/include/vector2.h"

namespace rinvid
{

Application::Application(std::uint32_t width, std::uint32_t height, const std::string& title,
                         bool fullscreen, std::uint16_t fps)
    : window_{}, current_screen_{nullptr}, new_screen_{nullptr}, fps_{fps}, running_{false}
{
    if (fullscreen)
    {
        window_.create(sf::VideoMode::getDesktopMode(), title, sf::Style::Fullscreen);
        window_.setVerticalSyncEnabled(true);
    }
    else
    {
        window_.create(sf::VideoMode{width, height}, title);
    }

#ifdef _WIN32
    gladLoadGLLoader(reinterpret_cast<GLADloadproc>(sf::Context::getFunction));
#endif

    RinvidGfx::init();
    auto size = window_.getSize();
    RinvidGfx::set_viewport(0, 0, size.x, size.y);
}

void Application::run()
{
    constexpr std::uint32_t       microseconds_in_a_second = 1'000'000;
    std::chrono::duration<double> delta_time{};
    std::chrono::duration<double> total_frame_time{};
    sf::Event                     event;

    window_.setActive(true);

    running_ = true;
    while (running_ == true)
    {
        auto start = std::chrono::high_resolution_clock::now();

        double target_frame_time =
            fps_ ? static_cast<double>(microseconds_in_a_second) / static_cast<double>(fps_) : 0.0;

        handle_events(window_, event);

        if (current_screen_ != nullptr)
        {
            current_screen_->update(total_frame_time.count());
        }

        window_.display();

        if (new_screen_)
        {
            if (current_screen_)
            {
                current_screen_->destroy();
            }
            current_screen_ = new_screen_;
            new_screen_     = nullptr;
            current_screen_->create();
            current_screen_->set_application(this);
        }

        auto end   = std::chrono::high_resolution_clock::now();
        delta_time = end - start;

#ifdef __unix__
        if (delta_time.count() < target_frame_time)
        {
            usleep(target_frame_time - delta_time.count());
        }
#else
        windows_sleep(target_frame_time - delta_time.count());
#endif
        end              = std::chrono::high_resolution_clock::now();
        total_frame_time = end - start;
    }
}

void Application::set_screen(Screen* screen)
{
    new_screen_ = screen;
}

void Application::set_fps(std::uint16_t fps)
{
    fps_ = fps;
}

void Application::exit()
{
    running_ = false;
}

void Application::handle_events(sf::Window& window, sf::Event& event)
{
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed:
                running_ = false;
                break;
            case sf::Event::Resized:
                rinvid::RinvidGfx::set_viewport(0, 0, event.size.width, event.size.height);
                break;
            default:
                break;
        }
    }
}

} // namespace rinvid
