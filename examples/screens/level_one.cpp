/**********************************************************************
 * Copyright (c) 2022 - 2024, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include "level_one.h"
#include "system/include/keyboard.h"

using namespace rinvid::system;

LevelOne::LevelOne(rinvid::Sprite* guardian_sprite, rinvid::Screen* next_level)
    : guardian_sprite_{guardian_sprite}, next_level_{next_level}
{
}

void LevelOne::create()
{
}

void LevelOne::update(double delta_time)
{
    rinvid::RinvidGfx::clear_screen(0.0F, 0.0F, 0.0F, 1.0F);

    bg_level_one_sprite.draw();
    guardian_sprite_->draw(delta_time);

    static bool last_moved_right = true;
    if (Keyboard::is_key_pressed(Keyboard::Key::D) ||
        Keyboard::is_key_pressed(Keyboard::Key::Right))
    {
        guardian_sprite_->move({180.0F * static_cast<float>(delta_time), 0.0F});
        guardian_sprite_->get_animation().play("treading_right");
        last_moved_right = true;
    }
    else if (Keyboard::is_key_pressed(Keyboard::Key::A) ||
             Keyboard::is_key_pressed(Keyboard::Key::Left))
    {
        guardian_sprite_->move({-180.0F * static_cast<float>(delta_time), 0.0F});
        guardian_sprite_->get_animation().play("treading_left");
        last_moved_right = false;
    }
    else
    {
        if (last_moved_right)
        {
            guardian_sprite_->get_animation().play("standing_right");
        }
        else
        {
            guardian_sprite_->get_animation().play("standing_left");
        }
    }

    if (guardian_sprite_->get_origin().x > 500)
    {
        this->get_application()->set_screen(next_level_);
    }
}

void LevelOne::destroy()
{
}
