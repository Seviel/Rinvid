#include <algorithm>

#include "core/include/rinvid_gfx.h"
#include "include/quad_shape.h"
#include "util/include/vector3.h"

#define GL_GLEXT_PROTOTYPES

#include <SFML/OpenGL.hpp>

namespace rinvid
{

QuadShape::QuadShape(Vector2 top_left, Vector2 top_right, Vector2 bottom_right, Vector2 bottom_left)
{
    verts_.at(0) = top_left;
    verts_.at(1) = top_right;
    verts_.at(2) = bottom_right;
    verts_.at(3) = bottom_left;
}

void QuadShape::draw()
{
    FixedPolygonShape::draw();
    draw_arrays(GL_QUADS);
}

} // namespace rinvid
