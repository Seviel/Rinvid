/**********************************************************************
 * Copyright (c) 2021, Filip Vasiljevic
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

#include "core/include/rinvid_gfx.h"
#include "include/application.h"

namespace rinvid
{

Application::Application(std::uint32_t width, std::uint32_t height, const std::string& title)
    : window_{sf::VideoMode{width, height}, title}, current_screen_{nullptr}, fps_{60U}, running_{
                                                                                             false}
{
}

void Application::run()
{
#ifdef _WIN32
    gladLoadGLLoader(reinterpret_cast<GLADloadproc>(sf::Context::getFunction));
#endif

    std::chrono::duration<double> delta_time{};
    sf::Event                     event;

    auto size = window_.getSize();
    RinvidGfx::set_viewport(0, 0, size.x, size.y);

    double target_frame_time = 1000000 / fps_;

    window_.setActive(true);

    RinvidGfx::init();

    running_ = true;
    while (running_ == true)
    {
        auto start = std::chrono::high_resolution_clock::now();

        handle_events(window_, event);

        if (current_screen_ != nullptr)
        {
            current_screen_->update(delta_time.count());
        }

        window_.display();

        auto end   = std::chrono::high_resolution_clock::now();
        delta_time = end - start;

#ifdef __unix__
        if (delta_time.count() < target_frame_time)
        {
            usleep(target_frame_time - delta_time.count());
        }
#else
        Sleep(10);
#endif
        end        = std::chrono::high_resolution_clock::now();
        delta_time = end - start;
    }
}

void Application::set_screen(Screen* screen)
{
    current_screen_ = screen;
    current_screen_->set_application(this);
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
