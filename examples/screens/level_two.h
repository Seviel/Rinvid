/**********************************************************************
 * Copyright (c) 2022 - 2025, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef EXAMPLES_SCREENS_LEVEL_TWO_H
#define EXAMPLES_SCREENS_LEVEL_TWO_H

#include <memory>

#include "core/include/screen.h"
#include "core/include/sprite.h"
#include "core/include/texture.h"

class LevelTwo : public rinvid::Screen
{
  public:
    LevelTwo(rinvid::Sprite* guardian_sprite);
    void create() override;
    void destroy() override;

  private:
    void update(double delta_time) override;

    rinvid::Texture bg_level_two_tex{"resources/level_2.png"};
    rinvid::Sprite  bg_level_two_sprite{&bg_level_two_tex, 640, 480, rinvid::Vector2f{0.0F, 0.0F},
                                       rinvid::Vector2f{0.0F, 0.0F}};

    rinvid::Sprite* guardian_sprite_;
};

#endif // EXAMPLES_SCREENS_LEVEL_TWO_H
