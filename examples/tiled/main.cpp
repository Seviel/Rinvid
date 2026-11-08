/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <algorithm>
#include <cmath>
#include <filesystem>
#include <iostream>
#include <memory>
#include <optional>
#include <utility>
#include <vector>

#include <rinvid/core/camera.h>
#include <rinvid/core/rectangle_shape.h>
#include <rinvid/core/render_context.h>
#include <rinvid/core/tile_map.h>
#include <rinvid/system/application.h>
#include <rinvid/system/keyboard.h>
#include <rinvid/system/screen.h>
#include <rinvid/util/image_cache.h>
#include <rinvid/util/tiled_map_loader.h>

namespace
{

class TiledScreen : public rinvid::Screen
{
  public:
    TiledScreen(rinvid::TileMapData data, rinvid::ImageCache& images)
        : prepared_data_{std::move(data)}, images_{images}
    {
    }

    void create() override
    {
        map_ = std::make_unique<rinvid::TileMap>(std::move(prepared_data_), images_);
        const auto& data{map_->get_data()};
        for (const auto& layer : data.object_layers_)
        {
            for (const auto& object : layer.objects_)
            {
                if (object.shape_ == rinvid::MapObjectShape::Point &&
                    object.name_ == "player_start")
                {
                    player_position_ = object.position_;
                }
                if (object.shape_ == rinvid::MapObjectShape::Rectangle &&
                    object.class_ == "collision" && object.size_.x > 0.0F && object.size_.y > 0.0F)
                {
                    collisions_.push_back(object);
                    obstacles_.emplace_back(
                        rinvid::Vector2f{object.position_.x + object.size_.x / 2.0F,
                                         object.position_.y + object.size_.y / 2.0F},
                        object.size_.x, object.size_.y);
                    obstacles_.back().set_color({0.8F, 0.2F, 0.2F, 0.65F});
                }
            }
        }
        player_.emplace(player_position_, 16.0F, 16.0F);
        player_->set_color({1.0F, 0.85F, 0.2F, 1.0F});
        camera_.set_borders(
            {0.0F, 0.0F},
            {std::max(0.0F, static_cast<float>(data.width_ * data.tile_width_) - 960.0F),
             std::max(0.0F, static_cast<float>(data.height_ * data.tile_height_) - 640.0F)});
    }

    void destroy() override
    {
        player_.reset();
        obstacles_.clear();
        collisions_.clear();
        map_.reset();
    }

  private:
    bool blocked(rinvid::Vector2f position) const
    {
        for (const auto& object : collisions_)
        {
            if (position.x + 8.0F > object.position_.x &&
                position.x - 8.0F < object.position_.x + object.size_.x &&
                position.y + 8.0F > object.position_.y &&
                position.y - 8.0F < object.position_.y + object.size_.y)
            {
                return true;
            }
        }
        return false;
    }

    void update(double delta_time) override
    {
        using Keyboard = rinvid::system::Keyboard;
        const float      step{240.0F * static_cast<float>(std::min(delta_time, 0.1))};
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
        // Small axis-separated steps keep this demo from jumping through thin rectangles.
        const auto steps{static_cast<std::int32_t>(
            std::ceil(std::max(std::abs(movement.x), std::abs(movement.y)) / 4.0F))};
        for (std::int32_t index{0}; index < steps; ++index)
        {
            const rinvid::Vector2f horizontal{player_position_.x + movement.x / steps,
                                              player_position_.y};
            if (!blocked(horizontal))
            {
                player_position_ = horizontal;
            }
            const rinvid::Vector2f vertical{player_position_.x,
                                            player_position_.y + movement.y / steps};
            if (!blocked(vertical))
            {
                player_position_ = vertical;
            }
        }
        player_->set_position(player_position_);
        camera_.set_position({player_position_.x - 480.0F, player_position_.y - 320.0F});
        camera_.update();
        get_render_context().clear_screen(0.08F, 0.13F, 0.17F, 1.0F);
        map_->draw();
        for (auto& obstacle : obstacles_)
        {
            obstacle.draw();
        }
        player_->draw();
    }

    rinvid::TileMapData                   prepared_data_{};
    rinvid::ImageCache&                   images_;
    std::unique_ptr<rinvid::TileMap>      map_{};
    rinvid::Camera                        camera_{};
    rinvid::Vector2f                      player_position_{160.0F, 160.0F};
    std::optional<rinvid::RectangleShape> player_{};
    std::vector<rinvid::MapObject>        collisions_{};
    std::vector<rinvid::RectangleShape>   obstacles_{};
};

} // namespace

int main(int argc, char** argv)
{
    const auto         map_file{argc > 1 ? std::filesystem::absolute(argv[1])
                                         : std::filesystem::absolute(argv[0]).parent_path() /
                                               "resources/garden.tmx"};
    rinvid::ImageCache images{};
    auto               data{rinvid::load_tiled_map(map_file)};
    std::vector<std::filesystem::path> paths{};
    for (const auto& tileset : data.tilesets_)
    {
        paths.push_back(tileset.image_);
    }
    // Decode on the cache worker while the window and rendering context are initialized.
    images.preload(paths);
    rinvid::Application application{960, 640, "Tiled | WASD / arrows: move | Esc: exit"};
    application.set_screen(std::make_unique<TiledScreen>(std::move(data), images));
    application.set_fps(60);
    application.run();

    return 0;
}
