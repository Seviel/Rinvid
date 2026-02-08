/**********************************************************************
 * Copyright (c) 2024 - 2026, Filip Vasiljevic
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
#include "core/include/object.h"
#include "core/include/rectangle_shape.h"
#include "core/include/rinvid_gfx.h"
#include "core/include/screen.h"
#include "core/include/sprite.h"
#include "core/include/sprite_object.h"
#include "core/include/text.h"
#include "core/include/texture.h"
#include "core/include/ttf_lib.h"
#include "gui/include/button.h"
#include "platformers/include/world.h"
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

    Texture background_texture_{"resources/rinvid_bg.png"};
    Sprite  background_sprite_{&background_texture_, 1920, 1080, Vector2f{0.0F, 0.0F},
                              Vector2f{0.0F, 0.0F}};
    Texture logo_texture_{"resources/logo.png"};
    Sprite  logo_sprite_{&logo_texture_, 100, 100, Vector2f{150.0F, 100.0F}, Vector2f{0.0F, 0.0F}};

    Texture clock_texture_{"resources/clck.png"};
    Sprite clock_sprite_{&clock_texture_, 100, 100, Vector2f{150.0F, 250.0F}, Vector2f{0.0F, 0.0F}};

    Texture      player_texture_{"resources/guardian.png"};
    SpriteObject player_sprite_{&player_texture_, 30, 45, Vector2f{400.0F, 250.0F},
                                Vector2f{0.0F, 0.0F}};

    Object         platform_{};
    RectangleShape platform_gfx_{{0.0F, 0.0F}, 200.0F, 50.0F};

    Texture      box_texture_{"resources/box.png"};
    SpriteObject box_sprite_{&box_texture_, 75, 75, Vector2f{475.0F, 20.0F}, Vector2f{0.0F, 0.0F}};

    Texture rotation_button_texture_{"resources/rotation_button.png"};
    Texture opacity_button_texture_{"resources/opacity_button.png"};
    Texture run_reset_button_texture_{"resources/run_reset_button.png"};
    Texture bell_button_texture_{"resources/bell_button.png"};
    Texture plus_button_texture_{"resources/plus_button.png"};
    Texture minus_button_texture_{"resources/minus_button.png"};

    Button logo_rotate_button_{};
    Button logo_transparency_button_{};
    Button clock_anim_button_{};
    Button light_intensity_plus_button_{};
    Button light_intensity_minus_button_{};
    Button light_falloff_plus_button_{};
    Button light_falloff_minus_button_{};

    Button bell_button_{};

    Text light_intensity_label_{
        "Light intensity", "resources/aquifer.ttf", {20.0F, 400.0F}, 0xffffffff, 18};

    Text light_falloff_label_{
        "Light falloff", "resources/aquifer.ttf", {20.0F, 475.0F}, 0xffffffff, 18};

    Camera camera_{};
    Light  light_{};

    sound::Sound bell_sound_{"resources/bell.wav"};

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

    light_falloff_plus_button_.setup(&plus_button_texture_, 100, 30, Vector2f{20.0F, 495.0F});

    light_falloff_plus_button_.set_idle({button_regions.at(0)});
    light_falloff_plus_button_.set_mouse_hovering({button_regions.at(1)});
    light_falloff_plus_button_.set_clicked({button_regions.at(2)});

    light_falloff_minus_button_.setup(&minus_button_texture_, 100, 30, Vector2f{130.0F, 495.0F});

    light_falloff_minus_button_.set_idle({button_regions.at(0)});
    light_falloff_minus_button_.set_mouse_hovering({button_regions.at(1)});
    light_falloff_minus_button_.set_clicked({button_regions.at(2)});

    bell_button_.setup(&bell_button_texture_, 100, 30, Vector2f{20.0F, 550.0F});

    bell_button_.set_idle({button_regions.at(0)});
    bell_button_.set_mouse_hovering({button_regions.at(1)});
    bell_button_.set_clicked({button_regions.at(2)});

    light_.set_position(Vector2f{400.0F, 50.0F});
    light_.set_intensity(0.5F);
    light_.set_falloff(0.3F);

    LightManager::activate_ambient_light(0.5F);

    regions = player_sprite_.get_animation().split_animation_frames(30, 45, 52, 1);
    rinvid::Animation standing_right{14.0,
                                     {regions[26], regions[27], regions[28], regions[29],
                                      regions[30], regions[31], regions[32], regions[33],
                                      regions[34], regions[35], regions[36], regions[37]},
                                     rinvid::AnimationMode::Looping};
    rinvid::Animation standing_left{14.0,
                                    {regions[14], regions[15], regions[16], regions[17],
                                     regions[18], regions[19], regions[20], regions[21],
                                     regions[22], regions[23], regions[24], regions[25]},
                                    rinvid::AnimationMode::Looping};
    rinvid::Animation treading_left{14.0,
                                    {regions[2], regions[3], regions[4], regions[5], regions[6],
                                     regions[7], regions[8], regions[9], regions[10], regions[11],
                                     regions[12], regions[13]},
                                    rinvid::AnimationMode::Looping};
    rinvid::Animation treading_right{14.0,
                                     {regions[38], regions[39], regions[40], regions[41],
                                      regions[42], regions[43], regions[44], regions[45],
                                      regions[46], regions[47], regions[48], regions[49]},
                                     rinvid::AnimationMode::Looping};
    player_sprite_.get_animation().add_animation("standing_right", standing_right);
    player_sprite_.get_animation().add_animation("standing_left", standing_left);
    player_sprite_.get_animation().add_animation("treading_left", treading_left);
    player_sprite_.get_animation().add_animation("treading_right", treading_right);
    player_sprite_.get_animation().play("standing_right");

    platform_.reset(Vector2f{350.0F, 400.0F});
    platform_.resize(200.0F, 50.0F);
    platform_.set_movable(false);
    platform_gfx_.set_position(Vector2f{450.0F, 425.0F});
    platform_gfx_.set_color(0x88888888);

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
    for (auto button : buttons_)
    {
        button->update_state();
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
    if (Keyboard::is_key_pressed(Keyboard::Key::D))
    {
        camera_.move(Vector2f{480.0F * static_cast<float>(delta_time), 0.0F});
    }

    if (Keyboard::is_key_pressed(Keyboard::Key::A))
    {
        camera_.move(Vector2f{-480.0F * static_cast<float>(delta_time), 0.0F});
    }

    static bool last_moved_right = true;
    if (Keyboard::is_key_pressed(Keyboard::Key::Right))
    {
        player_sprite_.set_x_velocity(300.0F);
        player_sprite_.get_animation().play("treading_right");
        last_moved_right = true;
    }
    else if (Keyboard::is_key_pressed(Keyboard::Key::Left))
    {
        player_sprite_.set_x_velocity(-300.0F);
        player_sprite_.get_animation().play("treading_left");
        last_moved_right = false;
    }
    else
    {
        if (last_moved_right)
        {
            player_sprite_.get_animation().play("standing_right");
        }
        else
        {
            player_sprite_.get_animation().play("standing_left");
        }
    }
    if (Keyboard::is_key_pressed(system::Keyboard::Key::Up))
    {
        if (player_sprite_.is_touching(DOWN))
        {
            player_sprite_.set_y_velocity(-350.0F);
        }
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

    player_sprite_.update(delta_time);
    platform_.update(delta_time);
    box_sprite_.update(delta_time);

    World::collide(box_sprite_, platform_);
    World::collide(player_sprite_, platform_);
    World::collide(player_sprite_, box_sprite_);
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

    light_intensity_label_.draw();
    light_falloff_label_.draw();

    platform_gfx_.draw();
    box_sprite_.draw();
    player_sprite_.draw(delta_time);
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
