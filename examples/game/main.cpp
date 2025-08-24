/**********************************************************************
 * Copyright (c) 2025, Filip Vasiljevic
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

#include "core/include/application.h"
#include "core/include/screen.h"
#include "core/include/sprite.h"
#include "core/include/texture.h"

using namespace rinvid::system;
using namespace rinvid;

class IntroScreen : public rinvid::Screen
{
  public:
    void create() override;
    void destroy() override;

  private:
    void update(double delta_time) override;

    rinvid::Texture background_texture{"examples/game/resources/intro.png"};
    rinvid::Sprite  background_sprite{&background_texture, 800, 600, rinvid::Vector2f{0.0F, 0.0F},
                                     rinvid::Vector2f{0.0F, 0.0F}};
};

void IntroScreen::create()
{
}

void IntroScreen::update(double delta_time)
{
    (void)delta_time;
    rinvid::RinvidGfx::clear_screen(0.0F, 0.0F, 0.0F, 1.0F);

    background_sprite.draw();
}

void IntroScreen::destroy()
{
}

int main()
{
    rinvid::Application main_app{800, 600, "TSF", false};
    IntroScreen         intro_screen{};
    main_app.set_screen(&intro_screen);
    main_app.set_fps(120);
    main_app.run();

    return 0;
}
