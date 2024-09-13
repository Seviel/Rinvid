/**********************************************************************
 * Copyright (c) 2024, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <vector>

#include "core/include/animation.h"
#include "core/include/application.h"
#include "core/include/camera.h"
#include "core/include/light.h"
#include "core/include/light_manager.h"
#include "core/include/rinvid_gfx.h"
#include "core/include/screen.h"
#include "core/include/sprite.h"
#include "core/include/text.h"
#include "core/include/texture.h"
#include "core/include/ttf_lib.h"
#include "gui/include/button.h"
#include "sound/include/sound.h"
#include "system/include/keyboard.h"
#include "system/include/mouse.h"
#include "util/include/collision_detection.h"
#include "util/include/vector2.h"

using namespace rinvid::system;
using namespace rinvid::gui;
using namespace rinvid;
class TestingGrounds : public Screen
{
  public:
    void create() override;
    void destroy() override;

  private:
    void button_control();
    void handle_logic(double delta_time);
    void draw_objects(double delta_time);
    void update(double delta_time) override;

    Texture background_texture_{"examples/full_demo/resources/rinvid_bg.png"};
    Sprite  background_sprite_{&background_texture_, 1920, 1080, Vector2f{0.0F, 0.0F},
                              Vector2f{0.0F, 0.0F}};
    Texture logo_texture_{"examples/full_demo/resources/logo.png"};
    Sprite  logo_sprite_{&logo_texture_, 100, 100, Vector2f{150.0F, 100.0F}, Vector2f{0.0F, 0.0F}};

    Texture clock_texture_{"examples/full_demo/resources/clck.png"};
    Sprite clock_sprite_{&clock_texture_, 100, 100, Vector2f{150.0F, 250.0F}, Vector2f{0.0F, 0.0F}};

    Texture rotation_button_texture_{"examples/full_demo/resources/rotation_button.png"};
    Texture opacity_button_texture_{"examples/full_demo/resources/opacity_button.png"};
    Texture run_reset_button_texture_{"examples/full_demo/resources/run_reset_button.png"};
    Texture bell_button_texture_{"examples/full_demo/resources/bell_button.png"};
    Texture plus_button_texture_{"examples/full_demo/resources/plus_button.png"};
    Texture minus_button_texture_{"examples/full_demo/resources/minus_button.png"};

    Button logo_rotate_button_{};
    Button logo_transparency_button_{};
    Button clock_anim_button_{};
    Button light_intensity_plus_button_{};
    Button light_intensity_minus_button_{};
    Button light_falloff_plus_button_{};
    Button light_falloff_minus_button_{};

    Button bell_button_{};

    Text light_text_{"Light controls",
                     "examples/full_demo/resources/aquifer.ttf",
                     {20.0F, 400.0F},
                     0xffffffff,
                     18};

    Camera camera_{};
    Light  light_{};

    sound::Sound bell_sound_{"examples/full_demo/resources/bell.wav"};

    bool logo_rotation_    = false;
    bool logo_transparent_ = false;
    bool clock_running_    = false;
    bool play_bell_        = false;

    std::vector<Button*> buttons_;
};

void TestingGrounds::create()
{

    auto      regions = clock_sprite_.get_animation().split_animation_frames(100, 100, 12, 1);
    Animation clock_standstill{20.0, {regions[0]}, AnimationMode::Normal};
    Animation clock_animation{20.0, regions, AnimationMode::Looping};
    clock_sprite_.get_animation().add_animation("still", clock_standstill);
    clock_sprite_.get_animation().add_animation("running", clock_animation);
    clock_sprite_.get_animation().play("still");

    logo_rotate_button_.setup(&rotation_button_texture_, 100, 30, Vector2f{20.0F, 120.0F});
    auto button_regions = logo_rotate_button_.get_animation().split_animation_frames(100, 30, 3, 1);

    logo_rotate_button_.set_idle({button_regions.at(0)});
    logo_rotate_button_.set_mouse_hovering({button_regions.at(1)});
    logo_rotate_button_.set_clicked({button_regions.at(2)});

    logo_transparency_button_.setup(&opacity_button_texture_, 100, 30, Vector2f{20.0F, 160.0F});

    logo_transparency_button_.set_idle({button_regions.at(0)});
    logo_transparency_button_.set_mouse_hovering({button_regions.at(1)});
    logo_transparency_button_.set_clicked({button_regions.at(2)});

    clock_anim_button_.setup(&run_reset_button_texture_, 100, 30, Vector2f{20.0F, 285.0F});

    clock_anim_button_.set_idle({button_regions.at(0)});
    clock_anim_button_.set_mouse_hovering({button_regions.at(1)});
    clock_anim_button_.set_clicked({button_regions.at(2)});

    light_intensity_plus_button_.setup(&plus_button_texture_, 100, 30, Vector2f{20.0F, 420.0F});

    light_intensity_plus_button_.set_idle({button_regions.at(0)});
    light_intensity_plus_button_.set_mouse_hovering({button_regions.at(1)});
    light_intensity_plus_button_.set_clicked({button_regions.at(2)});

    light_intensity_minus_button_.setup(&minus_button_texture_, 100, 30, Vector2f{130.0F, 420.0F});

    light_intensity_minus_button_.set_idle({button_regions.at(0)});
    light_intensity_minus_button_.set_mouse_hovering({button_regions.at(1)});
    light_intensity_minus_button_.set_clicked({button_regions.at(2)});

    light_falloff_plus_button_.setup(&plus_button_texture_, 100, 30, Vector2f{20.0F, 455.0F});

    light_falloff_plus_button_.set_idle({button_regions.at(0)});
    light_falloff_plus_button_.set_mouse_hovering({button_regions.at(1)});
    light_falloff_plus_button_.set_clicked({button_regions.at(2)});

    light_falloff_minus_button_.setup(&minus_button_texture_, 100, 30, Vector2f{130.0F, 455.0F});

    light_falloff_minus_button_.set_idle({button_regions.at(0)});
    light_falloff_minus_button_.set_mouse_hovering({button_regions.at(1)});
    light_falloff_minus_button_.set_clicked({button_regions.at(2)});

    bell_button_.setup(&bell_button_texture_, 100, 30, Vector2f{20.0F, 530.0F});

    bell_button_.set_idle({button_regions.at(0)});
    bell_button_.set_mouse_hovering({button_regions.at(1)});
    bell_button_.set_clicked({button_regions.at(2)});

    light_.set_position(Vector2f{400.0F, 50.0F});
    light_.set_intensity(0.5F);
    light_.set_falloff(0.3F);

    LightManager::activate_ambient_light(0.5F);

    buttons_.push_back(&logo_rotate_button_);
    buttons_.push_back(&logo_transparency_button_);
    buttons_.push_back(&clock_anim_button_);
    buttons_.push_back(&light_intensity_plus_button_);
    buttons_.push_back(&light_intensity_minus_button_);
    buttons_.push_back(&light_falloff_plus_button_);
    buttons_.push_back(&light_falloff_minus_button_);
    buttons_.push_back(&bell_button_);
}

void TestingGrounds::button_control()
{
    auto mouse_pos = system::Mouse::get_mouse_pos(get_application());
    for (auto button : buttons_)
    {
        button->update(mouse_pos);
    }

    if (light_intensity_plus_button_.just_clicked())
    {
        light_.set_intensity(std::min(1.0F, light_.get_intensity() + 0.1F));
    }

    if (light_intensity_minus_button_.just_clicked())
    {
        light_.set_intensity(std::max(0.0F, light_.get_intensity() - 0.1F));
    }

    if (light_falloff_plus_button_.just_clicked())
    {
        light_.set_falloff(std::min(1.0F, light_.get_falloff() + 0.1F));
    }

    if (light_falloff_minus_button_.just_clicked())
    {
        light_.set_falloff(std::max(0.0F, light_.get_falloff() - 0.1F));
    }

    if (logo_rotate_button_.just_clicked())
    {
        logo_rotation_ = !logo_rotation_;
    }

    if (logo_transparency_button_.just_clicked())
    {
        logo_transparent_ = !logo_transparent_;
    }

    if (clock_anim_button_.just_clicked())
    {
        clock_running_ = !clock_running_;
    }

    if (bell_button_.just_clicked())
    {
        play_bell_ = true;
    }
}

void TestingGrounds::handle_logic(double delta_time)
{
    if (Keyboard::is_key_pressed(Keyboard::Key::D) ||
        Keyboard::is_key_pressed(Keyboard::Key::Right))
    {
        camera_.move(Vector2f{480.0F * static_cast<float>(delta_time), 0.0F});
    }

    if (Keyboard::is_key_pressed(Keyboard::Key::A) || Keyboard::is_key_pressed(Keyboard::Key::Left))
    {
        camera_.move(Vector2f{-480.0F * static_cast<float>(delta_time), 0.0F});
    }

    if (Keyboard::is_key_pressed(Keyboard::Key::Escape))
    {
        get_application()->exit();
    }

    camera_.update();

    light_.update(camera_.get_pos());

    button_control();

    if (logo_rotation_)
    {
        logo_sprite_.rotate(60.0F * static_cast<float>(delta_time));
    }

    if (logo_transparent_)
    {
        logo_sprite_.set_opacity(0.3F);
    }
    else
    {
        logo_sprite_.set_opacity(1.0F);
    }

    if (clock_running_)
    {
        clock_sprite_.get_animation().play("running");
    }
    else
    {
        clock_sprite_.get_animation().play("still");
    }

    if (play_bell_)
    {
        bell_sound_.play();
        play_bell_ = false;
    }
}

void TestingGrounds::draw_objects(double delta_time)
{
    RinvidGfx::clear_screen(0.0F, 0.0F, 0.0F, 1.0F);

    background_sprite_.draw();

    logo_sprite_.draw();
    clock_sprite_.draw(delta_time);

    for (auto button : buttons_)
    {
        button->draw(delta_time);
    }

    light_text_.draw();
}

void TestingGrounds::update(double delta_time)
{
    handle_logic(delta_time);
    draw_objects(delta_time);
}

void TestingGrounds::destroy()
{
    TTFLib::destroy();
}

int main()
{
    Application    testing_grounds_app{800, 600, "Rinvid"};
    TestingGrounds testing_grounds_screen{};
    testing_grounds_app.set_screen(&testing_grounds_screen);
    testing_grounds_app.set_fps(120);
    testing_grounds_app.run();

    return 0;
}
