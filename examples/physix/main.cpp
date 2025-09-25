/**********************************************************************
 * Copyright (c) 2025, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include "core/include/application.h"
#include "core/include/screen.h"
#include "core/include/sprite.h"
#include "core/include/texture.h"
#include "platformers/include/world.h"
#include "system/include/keyboard.h"
#include "util/include/vector2.h"

using namespace rinvid;
using namespace rinvid::system;

class PhysixScreen : public rinvid::Screen
{
  public:
    void create() override;
    void destroy() override;

  private:
    void update(double delta_time) override;

    Texture platform_texture{"resources/plat.png"};
    Sprite  platform_sprite{&platform_texture, 597, 62, Vector2f{101.0F, 500.0F},
                           Vector2f{0.0F, 0.0F}};
    Texture sphere_texture{"resources/sphere.png"};
    Sprite  sphere_sprite{&sphere_texture, 200, 201, Vector2f{400.0F, 20.0F}, Vector2f{0.0F, 0.0F}};

    Texture man_texture{"resources/man.png"};
    Sprite  man{&man_texture, 68, 150, Vector2f{100.0F, 20.0F}, Vector2f{0.0F, 0.0F}};

    Texture box_texture{"resources/box.png"};
    Sprite  box_sprite{&box_texture, 75, 75, Vector2f{250.0F, 20.0F}, Vector2f{0.0F, 0.0F}};
};

void PhysixScreen::create()
{
    sphere_sprite.set_max_velocity(1200.0F);

    platform_sprite.set_gravity_scale(0.0F);
    platform_sprite.set_movable(false);
}

void PhysixScreen::update(double delta_time)
{
    RinvidGfx::clear_screen(0.2F, 0.8F, 0.8F, 1.0F);

    if (Keyboard::is_key_pressed(system::Keyboard::Key::Up))
    {
        man.set_y_velocity(-400.0F);
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
    platform_sprite.update(delta_time);
    box_sprite.update(delta_time);
    man.update(delta_time);

    World::collide(sphere_sprite, platform_sprite);
    World::collide(man, platform_sprite);
    World::collide(box_sprite, platform_sprite);

    World::collide(man, sphere_sprite);
    World::collide(man, box_sprite);

    World::collide(box_sprite, sphere_sprite);

    sphere_sprite.draw(delta_time);
    platform_sprite.draw(delta_time);
    box_sprite.draw(delta_time);
    man.draw(delta_time);
}

void PhysixScreen::destroy()
{
}

int main()
{
    Application  physix_app{800, 600, "Physix example"};
    PhysixScreen physix_screen{};
    physix_app.set_screen(&physix_screen);
    physix_app.run();

    return 0;
}
