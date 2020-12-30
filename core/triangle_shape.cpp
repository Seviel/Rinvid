#include <algorithm>

#include "core/include/rinvid_gfx.h"
#include "include/triangle_shape.h"
#include "util/include/vector3.h"

#define GL_GLEXT_PROTOTYPES

#include <SFML/OpenGL.hpp>

namespace rinvid
{

TriangleShape::TriangleShape(Vector2 vert1, Vector2 vert2, Vector2 vert3)
{
    verts_.at(0) = vert1;
    verts_.at(1) = vert2;
    verts_.at(2) = vert3;
}

void TriangleShape::draw()
{
    FixedPolygonShape::draw();
    draw_arrays(GL_TRIANGLES);
}

} // namespace rinvid
