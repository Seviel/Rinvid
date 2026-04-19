/**********************************************************************
 * Copyright (c) 2021 - 2026, Filip Vasiljevic
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
        window_.setFramerateLimit(fps_);
    }
    else
    {
        window_.create(sf::VideoMode{width, height}, title);
    }

#ifdef _WIN32
    gladLoadGLLoader(reinterpret_cast<GLADloadproc>(sf::Context::getFunction));
#endif

    RinvidGfx::init(this);
    auto size = window_.getSize();
    RinvidGfx::set_viewport(0, 0, size.x, size.y);
}

Application::~Application()
{
    destroy_current_screen();

    if (window_.setActive(true))
    {
        RinvidGfx::shutdown();
    }
}

void Application::run()
{
    std::chrono::duration<double> total_frame_time{};
    sf::Event                     event;

    window_.setActive(true);

    running_ = true;
    activate_pending_screen();

    while (running_ == true)
    {
        auto start = std::chrono::high_resolution_clock::now();

        handle_events(window_, event);

        if (current_screen_ != nullptr)
        {
            current_screen_->update(total_frame_time.count());
        }

        window_.display();

        if (running_ == true)
        {
            activate_pending_screen();
        }

        auto end         = std::chrono::high_resolution_clock::now();
        total_frame_time = end - start;
    }

    destroy_current_screen();
    new_screen_.reset();

    RinvidGfx::shutdown();
}

void Application::set_screen(std::unique_ptr<Screen> screen)
{
    new_screen_ = std::move(screen);
}

void Application::set_fps(std::uint16_t fps)
{
    fps_ = fps;
    window_.setFramerateLimit(fps_);
}

void Application::exit()
{
    running_ = false;
}

void Application::activate_pending_screen()
{
    if (!new_screen_)
    {
        return;
    }

    destroy_current_screen();

    current_screen_ = std::move(new_screen_);
    current_screen_->set_application(this);
    current_screen_->create();
}

void Application::destroy_current_screen()
{
    if (current_screen_ != nullptr)
    {
        current_screen_->destroy();
        current_screen_.reset();
    }
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
