#include "include/triangle_shape.h"
#include "util/include/vector3.h"

#define GL_GLEXT_PROTOTYPES

#include <SFML/OpenGL.hpp>

namespace rinvid
{

TriangleShape::TriangleShape(Vector3 vert1, Vector3 vert2, Vector3 vert3)
    : vert1_{vert1}, vert2_{vert2}, vert3_{vert3}
{
    float vertices[] = {vert1_.x, vert1_.y, vert1_.z, vert2_.x, vert2_.y,
                        vert2_.z, vert3_.x, vert3_.y, vert3_.z};

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void TriangleShape::draw()
{
    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

} // namespace rinvid