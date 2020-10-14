#include "include/rinvid_gfx.h"

namespace rinvid
{

void RinvidGfx::set_viewport(std::int32_t x, std::int32_t y, std::int32_t width,
                             std::int32_t heigth)
{
    glViewport(x, y, width, heigth);
}

} // namespace rinvid
