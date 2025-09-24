/**********************************************************************
 * Copyright (c) 2025, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include "core/include/animation.h"
#include "core/include/application.h"
#include "core/include/rinvid_gfx.h"
#include "core/include/rinvid_gl.h"
#include "core/include/screen.h"
#include "core/include/sprite.h"
#include "core/include/texture.h"
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
    Sprite  sphere_sprite{&sphere_texture, 200, 201, Vector2f{300.0F, 20.0F}, Vector2f{0.0F, 0.0F}};
};

void PhysixScreen::create()
{
    sphere_sprite.set_max_velocity(1200.0F);

    platform_sprite.set_gravity_scale(0.0F);
}

void PhysixScreen::update(double delta_time)
{
    RinvidGfx::clear_screen(0.2F, 0.8F, 0.8F, 1.0F);

    sphere_sprite.update(delta_time);

    sphere_sprite.draw(delta_time);
    platform_sprite.draw(delta_time);
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
