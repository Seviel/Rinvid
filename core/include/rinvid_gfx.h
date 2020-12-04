#ifndef CORE_INCLUDE_RINFID_GFX_H
#define CORE_INCLUDE_RINFID_GFX_H

#define GL_GLEXT_PROTOTYPES

#include <cstdint>
#include <SFML/OpenGL.hpp>

namespace rinvid
{

class RinvidGfx
{
  public:
    RinvidGfx()
    {
    }

    static void init();

    static void set_viewport(std::int32_t x, std::int32_t y, std::int32_t width,
                             std::int32_t heigth);

    static void clear_screen(float r, float g, float b, float a);

    static std::uint32_t get_default_shader();

    static std::uint32_t get_width();

    static std::uint32_t get_height();

    static float get_opengl_x_coord(float absolute_coord)
    {
        return ((absolute_coord / width) * 2.0F) - 1.0F;
    }

    static float get_opengl_y_coord(float absolute_coord)
    {
        return -1.0F * (((absolute_coord / height) * 2.0F) - 1.0F);
    }

  private:
    static std::uint32_t default_shader;
    static std::uint32_t width;
    static std::uint32_t height;
};

} // namespace rinvid

#endif // CORE_INCLUDE_RINFID_GFX_H
