/**********************************************************************
 * Copyright (c) 2021 - 2024, Lazar Lukic
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

using namespace rinvid::system;

class SpritesScreen : public rinvid::Screen
{
  public:
    void create() override;
    void destroy() override;

  private:
    void update(double delta_time) override;
    void handle_inputs(float& horizontal_delta);
    bool handle_animation_state(std::string& action, std::string& direction, float horizontal_delta,
                                bool is_animation_finished);

    std::string     action{"idle"};
    std::string     direction{"right"};
    std::string     current_animation_state{action + "_" + direction};
    rinvid::Color   base_color{0.1078431F, 0.6215686F, 0.5745098F, 1.0F};
    rinvid::Color   active_color{1.0000000F, 0.6705882F, 0.2862745F, 1.0F};
    rinvid::Texture robot_texture{"examples/sprites/resources/trashbot.png"};
    rinvid::Sprite  robot_sprite{&robot_texture, 455, 455, rinvid::Vector2<float>{0.0F, 100.0F},
                                rinvid::Vector2<float>{0.0F, 0.0F}};
};

void SpritesScreen::create()
{
    robot_sprite.get_animation().split_animation_frames(455, 455, 9, 8);

    auto idle_right    = robot_sprite.get_animation().get_regions({0});
    auto running_right = robot_sprite.get_animation().get_regions(
        {1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15,
         16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30});
    auto attack_right = robot_sprite.get_animation().get_regions({31, 32, 33, 34, 34, 34});

    rinvid::Animation idle_right_animation{0.0, idle_right, rinvid::AnimationMode::Normal};
    rinvid::Animation running_right_animation{20.0, running_right, rinvid::AnimationMode::Looping};
    rinvid::Animation attack_right_animation{30.0, attack_right, rinvid::AnimationMode::Normal};

    robot_sprite.get_animation().add_animation("idle_right", idle_right_animation);
    robot_sprite.get_animation().add_animation("running_right", running_right_animation);
    robot_sprite.get_animation().add_animation("attack_right", attack_right_animation);

    auto idle_left    = robot_sprite.get_animation().get_regions({35});
    auto attack_left  = robot_sprite.get_animation().get_regions({66, 67, 68, 69, 69, 69});
    auto running_left = robot_sprite.get_animation().get_regions(
        {36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
         51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65});

    rinvid::Animation idle_left_animation{0.0, idle_left, rinvid::AnimationMode::Normal};
    rinvid::Animation running_left_animation{20.0, running_left, rinvid::AnimationMode::Looping};
    rinvid::Animation attack_left_animation{30.0, attack_left, rinvid::AnimationMode::Normal};

    robot_sprite.get_animation().add_animation("idle_left", idle_left_animation);
    robot_sprite.get_animation().add_animation("running_left", running_left_animation);
    robot_sprite.get_animation().add_animation("attack_left", attack_left_animation);

    robot_sprite.get_animation().play(current_animation_state);
}

void SpritesScreen::update(double delta_time)
{
    rinvid::RinvidGfx::clear_screen(0.2F, 0.4F, 0.4F, 1.0F);

    float horizontal_delta = 0.0F;

    robot_sprite.draw(delta_time);

    handle_inputs(horizontal_delta);

    bool state_changed = handle_animation_state(
        action, direction, horizontal_delta, robot_sprite.get_animation().is_animation_finished());

    if (state_changed == true)
    {
        current_animation_state = action + "_" + direction;
        robot_sprite.get_animation().play(current_animation_state);
    }

    robot_sprite.move(rinvid::Vector2<float>{horizontal_delta * static_cast<float>(delta_time), 0});
}

void SpritesScreen::destroy()
{
}

void SpritesScreen::handle_inputs(float& horizontal_delta)
{
    if (Keyboard::is_key_pressed(Keyboard::Key::D) ||
        Keyboard::is_key_pressed(Keyboard::Key::Right))
    {
        horizontal_delta += 480.0F;
    }

    if (Keyboard::is_key_pressed(Keyboard::Key::A) || Keyboard::is_key_pressed(Keyboard::Key::Left))
    {
        horizontal_delta -= 480.0F;
    }
}

bool SpritesScreen::handle_animation_state(std::string& action, std::string& direction,
                                           float horizontal_delta, bool is_animation_finished)
{
    std::string current_animation_state{action + "_" + direction};
    std::string new_animation_state{action + "_" + direction};

    if (Keyboard::is_key_pressed(Keyboard::Key::F) ||
        (is_animation_finished == false && action == "attack"))
    {
        action = "attack";
    }
    else
    {
        if (horizontal_delta != 0)
        {
            action = "running";
        }
        else
        {
            action = "idle";
        }
    }

    if (horizontal_delta > 0)
    {
        direction = "right";
    }

    if (horizontal_delta < 0)
    {
        direction = "left";
    }

    new_animation_state = action + "_" + direction;

    if (current_animation_state != new_animation_state)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int main()
{
    rinvid::Application sprites_app{800, 600, "Sprites example"};
    SpritesScreen       sprites_screen{};
    sprites_app.set_screen(&sprites_screen);
    sprites_app.set_fps(120);
    sprites_app.run();

    return 0;
}
