/**********************************************************************
 * Copyright (c) 2022, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <iostream>

#include <SFML/Window.hpp>

#include "include/button.h"
#include "util/include/collision_detection.h"
#include "util/include/vector2.h"

namespace rinvid
{

namespace gui
{

Button::Button() : idle_{}, mouse_hovering_{}, clicked_{}, is_clicked_{false}
{
    add_animation("idle", idle_);
    add_animation("mouse_hovering", mouse_hovering_);
    add_animation("clicked", clicked_);

    play("idle");
}

void Button::update(Vector2<float> mouse_pos)
{
    rinvid::Rect position_rect{mouse_pos, 1, 1};
    if (intersects(bounding_rect(), position_rect))
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            play("clicked");
            is_clicked_ = true;
        }
        else
        {
            play("mouse_hovering");
            is_clicked_ = false;
        }
    }
    else
    {
        play("idle");
        is_clicked_ = false;
    }
}

void Button::set_idle(const std::vector<Rect>& regions)
{
    idle_.setup(0.0, regions);
    animations_.find("idle")->second = idle_;
}

void Button::set_mouse_hovering(const std::vector<Rect>& regions)
{
    mouse_hovering_.setup(0.0, regions);
    animations_.find("mouse_hovering")->second = mouse_hovering_;
}

void Button::set_clicked(const std::vector<Rect>& regions)
{
    clicked_.setup(0.0, regions);
    animations_.find("clicked")->second = clicked_;
}

bool Button::is_clicked() const
{
    return is_clicked_;
}

} // namespace gui

} // namespace rinvid
