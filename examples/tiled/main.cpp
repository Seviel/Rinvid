/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <memory>
#include <utility>

#include <rinvid/core/camera.h>
#include <rinvid/core/render_context.h>
#include <rinvid/core/tile_map.h>
#include <rinvid/system/application.h>
#include <rinvid/system/keyboard.h>
#include <rinvid/system/screen.h>

namespace
{

class TiledScreen : public rinvid::Screen
{
  public:
    explicit TiledScreen(std::filesystem::path map_file) : map_file_{std::move(map_file)}
    {
    }

    void create() override
    {
        map_ = std::make_unique<rinvid::TileMap>(map_file_);
        const auto& data{map_->get_data()};
        camera_.set_borders(
            {0.0F, 0.0F},
            {std::max(0.0F, static_cast<float>(data.width_ * data.tile_width_) - 960.0F),
             std::max(0.0F, static_cast<float>(data.height_ * data.tile_height_) - 640.0F)});
    }

    void destroy() override
    {
        map_.reset();
    }

  private:
    void update(double delta_time) override
    {
        using Keyboard = rinvid::system::Keyboard;
        const float      step{240.0F * static_cast<float>(delta_time)};
        rinvid::Vector2f movement{};
        if (Keyboard::is_key_pressed(Keyboard::Key::Left) ||
            Keyboard::is_key_pressed(Keyboard::Key::A))
        {
            movement.x -= step;
        }
        if (Keyboard::is_key_pressed(Keyboard::Key::Right) ||
            Keyboard::is_key_pressed(Keyboard::Key::D))
        {
            movement.x += step;
        }
        if (Keyboard::is_key_pressed(Keyboard::Key::Up) ||
            Keyboard::is_key_pressed(Keyboard::Key::W))
        {
            movement.y -= step;
        }
        if (Keyboard::is_key_pressed(Keyboard::Key::Down) ||
            Keyboard::is_key_pressed(Keyboard::Key::S))
        {
            movement.y += step;
        }
        if (Keyboard::is_key_pressed(Keyboard::Key::Escape))
        {
            get_application()->exit();
        }
        camera_.move(movement);
        camera_.update();
        get_render_context().clear_screen(0.08F, 0.13F, 0.17F, 1.0F);
        map_->draw();
    }

    std::filesystem::path            map_file_;
    std::unique_ptr<rinvid::TileMap> map_{};
    rinvid::Camera                   camera_{};
};

} // namespace

int main(int argc, char** argv)
{
    const auto          map_file{argc > 1 ? std::filesystem::absolute(argv[1])
                                          : std::filesystem::absolute(argv[0]).parent_path() /
                                       "resources/garden.tmx"};
    rinvid::Application application{960, 640, "Tiled | WASD / arrows: pan | Esc: exit"};
    application.set_screen(std::make_unique<TiledScreen>(map_file));
    application.set_fps(60);
    application.run();

    return 0;
}
