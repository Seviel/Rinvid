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
#include <rinvid/util/windows_utils.h>
#endif // _WIN32

#include <rinvid/core/rinvid_gl.h>
#include <rinvid/system/application.h>
#include <rinvid/util/vector2.h>

namespace rinvid
{

Application::Application(std::uint32_t width, std::uint32_t height, const std::string& title,
                         bool fullscreen, std::uint16_t fps)
    : Application{width, height, title, fullscreen, WindowResizeMode::Resizable, fps}
{
}

Application::Application(std::uint32_t width, std::uint32_t height, const std::string& title,
                         WindowResizeMode resize_mode, std::uint16_t fps)
    : Application{width, height, title, false, resize_mode, fps}
{
}

Application::Application(std::uint32_t width, std::uint32_t height, const std::string& title,
                         bool fullscreen, WindowResizeMode resize_mode, std::uint16_t fps)
    : window_{}, context_{}, current_screen_{nullptr}, new_screen_{nullptr},
      windowed_size_{width, height}, fps_{fps}, window_resize_mode_{resize_mode}, running_{false}
{
    if (fullscreen)
    {
        window_.create(sf::VideoMode::getDesktopMode(), title, sf::State::Fullscreen);
        window_.setVerticalSyncEnabled(true);
        window_.setFramerateLimit(fps_);
    }
    else
    {
        const std::uint32_t style = resize_mode == WindowResizeMode::Fixed
                                        ? sf::Style::Titlebar | sf::Style::Close
                                        : sf::Style::Default;
        window_.create(sf::VideoMode{{width, height}}, title, style);
        enforce_fixed_window_size();
    }

#ifdef _WIN32
    gladLoadGLLoader(reinterpret_cast<GLADloadproc>(sf::Context::getFunction));
#endif

    context_.init(this, window_);
    const auto size =
        window_resize_mode_ == WindowResizeMode::Fixed ? windowed_size_ : window_.getSize();
    context_.get_render_context().set_viewport(0, 0, size.x, size.y);
}

Application::~Application()
{
    destroy_current_screen();

    if (window_.setActive(true))
    {
        context_.shutdown();
    }
}

void Application::run()
{
    std::chrono::duration<double> total_frame_time{};

    if (!window_.setActive(true))
    {
        return;
    }

    running_ = true;
    activate_pending_screen();

    while (running_ == true)
    {
        handle_events(window_);

        // Native event handlers can block while the user moves a window. Treat that time as paused
        // instead of passing one oversized time step to the next update.
        const auto start = std::chrono::steady_clock::now();

        if (current_screen_ != nullptr)
        {
            current_screen_->update(total_frame_time.count());
        }

        window_.display();

        if (running_ == true)
        {
            activate_pending_screen();
        }

        const auto end   = std::chrono::steady_clock::now();
        total_frame_time = end - start;
    }

    destroy_current_screen();
    new_screen_.reset();

    context_.shutdown();
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

ApplicationContext& Application::get_context()
{
    return context_;
}

const ApplicationContext& Application::get_context() const
{
    return context_;
}

RenderContext& Application::get_render_context()
{
    return context_.get_render_context();
}

const RenderContext& Application::get_render_context() const
{
    return context_.get_render_context();
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

void Application::enforce_fixed_window_size()
{
    if (window_resize_mode_ != WindowResizeMode::Fixed)
    {
        return;
    }

#ifdef _WIN32
    const HWND native_window = window_.getNativeHandle();
    RECT       window_rectangle{};
    RECT       client_rectangle{};

    if (native_window != nullptr && GetWindowRect(native_window, &window_rectangle) != 0 &&
        GetClientRect(native_window, &client_rectangle) != 0)
    {
        const LONG client_width  = client_rectangle.right - client_rectangle.left;
        const LONG client_height = client_rectangle.bottom - client_rectangle.top;

        if (client_width == static_cast<LONG>(windowed_size_.x) &&
            client_height == static_cast<LONG>(windowed_size_.y))
        {
            return;
        }

        const LONG window_width  = window_rectangle.right - window_rectangle.left;
        const LONG window_height = window_rectangle.bottom - window_rectangle.top;
        const LONG corrected_width =
            window_width + static_cast<LONG>(windowed_size_.x) - client_width;
        const LONG corrected_height =
            window_height + static_cast<LONG>(windowed_size_.y) - client_height;

        if (SetWindowPos(native_window, nullptr, 0, 0, corrected_width, corrected_height,
                         SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE) != 0)
        {
            return;
        }
    }
#endif

    window_.setSize(windowed_size_);
}

void Application::handle_events(sf::Window& window)
{
    while (const auto event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            running_ = false;
        }
        else if (const auto* resized = event->getIf<sf::Event::Resized>())
        {
            if (window_resize_mode_ == WindowResizeMode::Fixed)
            {
                enforce_fixed_window_size();
                context_.get_render_context().set_viewport(0, 0, windowed_size_.x,
                                                           windowed_size_.y);
            }
            else
            {
                context_.get_render_context().set_viewport(0, 0, resized->size.x, resized->size.y);
            }
        }
    }
}

} // namespace rinvid
