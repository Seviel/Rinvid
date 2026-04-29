/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <SFML/Window.hpp>

#include <rinvid/system/application_context.h>

namespace rinvid
{

ApplicationContext* ApplicationContext::active_context_{nullptr};

void ApplicationContext::init(const Application* application, sf::Window& window)
{
    window_         = &window;
    active_context_ = this;
    render_context_.init(application);
}

void ApplicationContext::shutdown()
{
    render_context_.shutdown();
    window_ = nullptr;

    if (active_context_ == this)
    {
        active_context_ = nullptr;
    }
}

RenderContext& ApplicationContext::get_render_context()
{
    return render_context_;
}

const RenderContext& ApplicationContext::get_render_context() const
{
    return render_context_;
}

sf::Window* ApplicationContext::get_window()
{
    return window_;
}

const sf::Window* ApplicationContext::get_window() const
{
    return window_;
}

ApplicationContext* ApplicationContext::get_active_context()
{
    return active_context_;
}

} // namespace rinvid
