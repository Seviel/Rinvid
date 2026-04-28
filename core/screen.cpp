/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <rinvid/core/application.h>
#include <rinvid/core/screen.h>

namespace rinvid
{

Application* Screen::get_application()
{
    return application_;
}

RenderContext& Screen::get_render_context()
{
    return application_->get_render_context();
}

const RenderContext& Screen::get_render_context() const
{
    return application_->get_render_context();
}

void Screen::set_application(Application* application)
{
    application_ = application;
}

} // namespace rinvid
