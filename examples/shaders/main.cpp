/**********************************************************************
 * Copyright (c) 2024 - 2025, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "core/include/application.h"
#include "core/include/rinvid_gfx.h"
#include "core/include/screen.h"
#include "core/include/sprite.h"
#include "core/include/texture.h"
#include "util/include/vector2.h"

using namespace rinvid;

static std::string load_shader_source(const char* shader_path)
{
    std::string   shader_code;
    std::ifstream shader_file;
    shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        shader_file.open(shader_path);
        std::stringstream shader_stream;
        shader_stream << shader_file.rdbuf();
        shader_file.close();
        shader_code = shader_stream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cerr << "Error: file not successfully read: " << e.what() << std::endl;
    }

    return shader_code;
}

class ShadersScreen : public rinvid::Screen
{
  public:
    void create() override;
    void destroy() override;

  private:
    void update(double delta_time) override;

    Texture texture_{"resources/logo.png"};
    Sprite  sprite1_{&texture_, 100, 100, Vector2f{170.0F, 190.0F}, Vector2f{0.0F, 0.0F}};
    Sprite  sprite2_{&texture_, 100, 100, Vector2f{370.0F, 190.0F}, Vector2f{0.0F, 0.0F}};
    Shader  shader_;
};

void ShadersScreen::create()
{
    auto vertex_shader_code   = load_shader_source("resources/shader.vert");
    auto fragment_shader_code = load_shader_source("resources/shader.frag");
    shader_                   = Shader(vertex_shader_code.c_str(), fragment_shader_code.c_str());
}

void ShadersScreen::update(double delta_time)
{
    (void)delta_time;
    RinvidGfx::clear_screen(0.0F, 0.0F, 0.0F, 1.0F);
    sprite1_.draw(shader_);
    sprite2_.draw();
}

void ShadersScreen::destroy()
{
}

int main()
{
    Application   shaders_app{640, 480, "Rinvid shaders demo"};
    ShadersScreen shaders_screen{};
    shaders_app.set_screen(&shaders_screen);
    shaders_app.run();

    return 0;
}
