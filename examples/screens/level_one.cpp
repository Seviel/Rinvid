/**********************************************************************
 * Copyright (c) 2022 - 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <memory>
#include <utility>

#include "level_one.h"
#include "level_two.h"
#include <rinvid/core/render_context.h>
#include <rinvid/system/keyboard.h>

using namespace rinvid::system;

LevelOne::LevelOne(rinvid::Sprite* guardian_sprite, rinvid::ImageCache& images)
    : images_{images}, guardian_sprite_{guardian_sprite}
{
    images_.preload({"resources/level_1.png", "resources/level_2.png"});
}

void LevelOne::create()
{
    // Startup may wait. Subsequent preloading never waits in update().
    const auto image = images_.get("resources/level_1.png");
    bg_level_one_tex =
        std::make_unique<rinvid::Texture>(image->pixels, image->width, image->height);
    bg_level_one_sprite.setup(bg_level_one_tex.get(), 640, 480, {0.0F, 0.0F});
    images_.evict("resources/level_1.png");
}

void LevelOne::update(double delta_time)
{
    if (!next_texture_)
    {
        if (const auto image = images_.try_get("resources/level_2.png"))
        {
            // Upload on the rendering thread while the current level is still active.
            next_texture_ =
                std::make_unique<rinvid::Texture>(image->pixels, image->width, image->height);
            images_.clear();
        }
    }
    get_render_context().clear_screen(0.0F, 0.0F, 0.0F, 1.0F);

    bg_level_one_sprite.draw();
    guardian_sprite_->draw(delta_time);

    static bool last_moved_right{true};
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
        if (next_texture_)
        {
            this->get_application()->set_screen(
                std::make_unique<LevelTwo>(guardian_sprite_, std::move(next_texture_)));
        }
        else
        {
            // Keep rendering while waiting; the white bar indicates a pending transition.
            guardian_sprite_->set_position({500.0F, 425.0F});
            loading_indicator_.draw();
        }
    }
}

void LevelOne::destroy()
{
}
