#include <algorithm>
#include <cmath>

#include "core/include/rinvid_gfx.h"
#include "include/circle_shape.h"
#include "util/include/math_utils.h"
#include "util/include/vector3.h"

#define GL_GLEXT_PROTOTYPES

#include <SFML/OpenGL.hpp>

namespace rinvid
{

CircleShape::CircleShape(Vector2 center, float radius)
{
    float angle{};
    for (std::uint32_t i{0}; i < number_of_vertices_; ++i)
    {
        angle        = i * (DEGREES_IN_A_CIRCLE / number_of_vertices_);
        verts_.at(i) = Vector2{center.x + std::cos(DEGREES_TO_RADIANS(angle)) * radius,
                               center.y + std::sin(DEGREES_TO_RADIANS(angle)) * radius};
    }

    calculate_origin();

    init_vertex_buffer();
}

void CircleShape::draw()
{
    normalize_coordinates();

    std::int32_t color_location = glGetUniformLocation(RinvidGfx::get_default_shader(), "in_color");
    glUniform4f(color_location, color_.r, color_.g, color_.b, color_.a);

    glBindVertexArray(vao_);
    glDrawArrays(GL_POLYGON, 0, number_of_vertices_);
    glBindVertexArray(0);
}

} // namespace rinvid
