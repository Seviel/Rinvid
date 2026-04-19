/**********************************************************************
 * Copyright (c) 2025 - 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <rinvid/core/application.h>
#include <rinvid/core/screen.h>
#include <rinvid/core/sprite.h>
#include <rinvid/core/sprite_object.h>
#include <rinvid/core/texture.h>
#include <rinvid/platformers/world.h>
#include <rinvid/system/keyboard.h>
#include <rinvid/util/vector2.h>

using namespace rinvid;
using namespace rinvid::system;

class PhysixScreen : public rinvid::Screen
{
  public:
    void create() override;
    void destroy() override;

  private:
    void update(double delta_time) override;

    Texture      platform_texture{"resources/plat.png"};
    SpriteObject platform_1{&platform_texture, 597, 62, Vector2f{101.0F, 500.0F},
                            Vector2f{0.0F, 0.0F}};
    SpriteObject platform_2{&platform_texture, 597, 62, Vector2f{-400.0F, 300.0F},
                            Vector2f{0.0F, 0.0F}};
    Texture      sphere_texture{"resources/sphere.png"};
    SpriteObject sphere_sprite{&sphere_texture, 200, 201, Vector2f{400.0F, 20.0F},
                               Vector2f{0.0F, 0.0F}};

    Texture      man_texture{"resources/man.png"};
    SpriteObject man{&man_texture, 68, 150, Vector2f{100.0F, 20.0F}, Vector2f{0.0F, 0.0F}};

    Texture      box_texture{"resources/box.png"};
    SpriteObject box_sprite{&box_texture, 75, 75, Vector2f{250.0F, 20.0F}, Vector2f{0.0F, 0.0F}};
};

void PhysixScreen::create()
{
    sphere_sprite.set_max_velocity(1200.0F);

    platform_1.set_gravity_scale(0.0F);
    platform_1.set_movable(NOT);
    platform_2.set_gravity_scale(0.0F);
    platform_2.set_movable(NOT);
    platform_2.set_allowed_collisions(UP);
}

void PhysixScreen::update(double delta_time)
{
    RinvidGfx::clear_screen(0.2F, 0.8F, 0.8F, 1.0F);

    if (Keyboard::is_key_pressed(system::Keyboard::Key::Up))
    {
        if (man.is_touching(DOWN))
        {
            man.set_y_velocity(-500.0F);
        }
    }
    if (Keyboard::is_key_pressed(system::Keyboard::Key::Right))
    {
        man.set_x_velocity(400.0F);
    }
    else if (Keyboard::is_key_pressed(system::Keyboard::Key::Left))
    {
        man.set_x_velocity(-400.0F);
    }
    else
    {
        man.set_x_velocity(0.0F);
    }

    sphere_sprite.update(delta_time);
    platform_1.update(delta_time);
    platform_2.update(delta_time);
    box_sprite.update(delta_time);
    man.update(delta_time);

    World::collide(sphere_sprite, platform_1);
    World::collide(man, platform_1);
    World::collide(man, platform_2);
    World::collide(box_sprite, platform_1);

    World::collide(man, sphere_sprite);
    World::collide(man, box_sprite);

    World::collide(box_sprite, sphere_sprite);

    sphere_sprite.draw(delta_time);
    platform_1.draw(delta_time);
    box_sprite.draw(delta_time);
    man.draw(delta_time);
    platform_2.draw(delta_time);
}

void PhysixScreen::destroy()
{
}

int main()
{
    Application physix_app{800, 600, "Physix example"};
    physix_app.set_screen(std::make_unique<PhysixScreen>());
    physix_app.run();

    return 0;
}
