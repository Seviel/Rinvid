/**********************************************************************
 * Copyright (c) 2022 - 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef EXAMPLES_SCREENS_LEVEL_ONE_H
#define EXAMPLES_SCREENS_LEVEL_ONE_H

#include <memory>

#include <rinvid/core/rectangle_shape.h>
#include <rinvid/core/sprite.h>
#include <rinvid/core/texture.h>
#include <rinvid/system/application.h>
#include <rinvid/system/screen.h>
#include <rinvid/util/image_cache.h>

class LevelOne : public rinvid::Screen
{
  public:
    LevelOne(rinvid::Sprite* guardian_sprite, rinvid::ImageCache& images);
    void create() override;
    void destroy() override;

  private:
    void update(double delta_time) override;

    rinvid::ImageCache&              images_;
    std::unique_ptr<rinvid::Texture> bg_level_one_tex;
    std::unique_ptr<rinvid::Texture> next_texture_;
    rinvid::Sprite                   bg_level_one_sprite;
    rinvid::RectangleShape           loading_indicator_{{320.0F, 20.0F}, 160.0F, 12.0F};

    rinvid::Sprite* guardian_sprite_;
};

#endif // EXAMPLES_SCREENS_LEVEL_ONE_H
