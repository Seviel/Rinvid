/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <rinvid/gui/widget.h>

namespace rinvid
{

namespace gui
{

void Widget::update(double delta_time)
{
    (void)delta_time;
}

void Widget::draw(double delta_time)
{
    (void)delta_time;
    draw();
}

void Widget::set_enabled(bool enabled)
{
    (void)enabled;
}

bool Widget::is_enabled() const
{
    return true;
}

} // namespace gui

} // namespace rinvid
