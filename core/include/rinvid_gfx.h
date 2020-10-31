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

  private:
    static std::uint32_t default_shader;
};

} // namespace rinvid

#endif // CORE_INCLUDE_RINFID_GFX_H