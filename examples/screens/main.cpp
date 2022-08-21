/**********************************************************************
 * Copyright (c) 2022, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <SFML/Window.hpp>

#include "core/include/animation.h"
#include "core/include/application.h"
#include "core/include/screen.h"
#include "core/include/sprite.h"
#include "core/include/texture.h"
#include "level_one.h"
#include "level_two.h"
#include "util/include/vector2.h"

int main()
{
    rinvid::Application screens_demo_app{640, 480, "Rinvid screens demo"};

    rinvid::Texture guardian_tex{"examples/screens/resources/guardian.png"};
    rinvid::Sprite  guardian_sprite{&guardian_tex, 30, 45, rinvid::Vector2<float>{50.0F, 425.0F},
                                   rinvid::Vector2<float>{0.0F, 0.0F}};

    auto              regions = guardian_sprite.split_animation_frames(30, 45, 52, 1);
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
    guardian_sprite.add_animation("standing_right", standing_right);
    guardian_sprite.add_animation("standing_left", standing_left);
    guardian_sprite.add_animation("treading_left", treading_left);
    guardian_sprite.add_animation("treading_right", treading_right);
    guardian_sprite.play("standing_right");

    LevelTwo level_two{&guardian_sprite};
    LevelOne level_one{&guardian_sprite, &level_two};
    screens_demo_app.set_screen(&level_one);
    screens_demo_app.run();

    return 0;
}
