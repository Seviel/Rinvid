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

    static void set_viewport(std::int32_t x, std::int32_t y, std::int32_t width,
                             std::int32_t heigth);
};

} // namespace rinvid

#endif // CORE_INCLUDE_RINFID_GFX_H