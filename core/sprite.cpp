/**********************************************************************
 * Copyright (c) 2021 - 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <rinvid/core/rinvid_gfx.h>
#include <rinvid/core/rinvid_gl.h>
#include <rinvid/core/sprite.h>

namespace rinvid
{

Sprite::Sprite()
    : sprite_animation_{}, texture_{nullptr}, texture_offset_{0.0F, 0.0F}, opacity_{1.0F}
{
    position_ = Vector2f{0.0F, 0.0F};
    width_    = 0;
    height_   = 0;
}

Sprite::Sprite(Texture* texture, std::int32_t width, std::int32_t height, Vector2f top_left,
               Vector2f texture_offset)
    : sprite_animation_{}, texture_{texture}, texture_offset_{texture_offset}, opacity_{1.0F}
{
    setup(texture, width, height, top_left, texture_offset);
}

Sprite::~Sprite()
{
    release_gl_resources();
}

void Sprite::draw()
{
    draw(0.0);
}

void Sprite::draw(const Shader shader)
{
    draw(0.0, shader);
}

void Sprite::draw(double delta_time)
{
    const auto shader = RinvidGfx::get_texture_default_shader();
    draw(delta_time, shader);
}

void Sprite::draw(double delta_time, const Shader shader)
{
    if (texture_ == nullptr)
    {
        return;
    }

    origin_.x = position_.x + width_ / 2;
    origin_.y = position_.y + height_ / 2;

    if (sprite_animation_.is_active_)
    {
        sprite_animation_.current_animation_->advance(delta_time);
        Rect          texture_region = sprite_animation_.current_animation_->frame();
        Vector2f      offset{texture_offset_};
        std::uint32_t width  = texture_region.width;
        std::uint32_t height = texture_region.height;

        offset.x += texture_region.position.x;
        offset.y += texture_region.position.y;

        update_vertices(offset, width, height);
    }

    ensure_vertex_buffer_initialized();

    shader.use();
    RinvidGfx::update_mvp_matrix(get_transform(), shader.get_id());
    shader.set_float("opacity", opacity_);

    texture_->bind();
    GL_CALL(glBindVertexArray(vertex_array_object_));
    GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
}

void Sprite::move(const Vector2f move_vector)
{
    position_.move(move_vector);
}

void Sprite::set_position(const Vector2f vector)
{
    position_.set(vector);
}

Rect Sprite::bounding_rect()
{
    if (is_transformed() == false)
    {
        return Rect{position_, width_, height_};
    }

    Rect                   rect{};
    std::vector<glm::vec4> glm_vertices{};
    glm_vertices.reserve(4U);

    const auto& transform = get_transform();

    glm_vertices.emplace_back(transform * glm::vec4{position_.x, position_.y, 1.0F, 1.0F});
    glm_vertices.emplace_back(transform * glm::vec4{position_.x + width_, position_.y, 1.0F, 1.0F});
    glm_vertices.emplace_back(transform *
                              glm::vec4{position_.x + width_, position_.y + height_, 1.0F, 1.0F});
    glm_vertices.emplace_back(transform *
                              glm::vec4{position_.x, position_.y + height_, 1.0F, 1.0F});

    float min_x{};
    float max_x{};
    float min_y{};
    float max_y{};

    set_min_max_coords(glm_vertices, min_x, max_x, min_y, max_y);

    rect.position.x = min_x;
    rect.position.y = min_y;
    rect.width      = max_x - min_x;
    rect.height     = max_y - min_y;

    return rect;
}

void Sprite::setup(Texture* texture, std::int32_t width, std::int32_t height, Vector2f top_left,
                   Vector2f texture_offset)
{
    texture_        = texture;
    width_          = width;
    height_         = height;
    position_       = top_left;
    texture_offset_ = texture_offset;

    update_vertices(texture_offset_, width_, height_);
}

void Sprite::set_opacity(float transparency)
{
    opacity_ = transparency;
}

SpriteAnimation& Sprite::get_animation()
{
    return sprite_animation_;
}

void Sprite::ensure_vertex_buffer_initialized()
{
    if (vertex_array_object_ == 0U)
    {
        init_vertex_buffer();
    }
}

void Sprite::init_vertex_buffer()
{
    GL_CALL(glGenVertexArrays(1, &vertex_array_object_));
    GL_CALL(glGenBuffers(1, &vertex_buffer_object_));
    GL_CALL(glGenBuffers(1, &element_buffer_object_));

    GL_CALL(glBindVertexArray(vertex_array_object_));

    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(gl_vertices_), gl_vertices_, GL_DYNAMIC_DRAW));

    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object_));
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_), indices_, GL_STATIC_DRAW));

    // Position attribute
    GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0));
    GL_CALL(glEnableVertexAttribArray(0));

    // Texture coordinate attribute
    GL_CALL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                                  (void*)(3 * sizeof(float))));
    GL_CALL(glEnableVertexAttribArray(1));
}

void Sprite::release_gl_resources()
{
    if (element_buffer_object_ != 0U)
    {
        GL_CALL(glDeleteBuffers(1, &element_buffer_object_));
        element_buffer_object_ = 0U;
    }

    if (vertex_buffer_object_ != 0U)
    {
        GL_CALL(glDeleteBuffers(1, &vertex_buffer_object_));
        vertex_buffer_object_ = 0U;
    }

    if (vertex_array_object_ != 0U)
    {
        GL_CALL(glDeleteVertexArrays(1, &vertex_array_object_));
        vertex_array_object_ = 0U;
    }
}

void Sprite::update_vertices(Vector2f offset, std::uint32_t width, std::uint32_t height)
{
    if (texture_ == nullptr)
    {
        return;
    }

    ensure_vertex_buffer_initialized();

    // Make center of texture (0, 0) for simplicity.
    Vector2f top_left{};
    top_left.x = 0.0F - (width / 2.0F);
    top_left.y = 0.0F - (height / 2.0F);

    // Top left
    gl_vertices_[0] = top_left.x;
    gl_vertices_[1] = top_left.y;
    gl_vertices_[2] = 0.0F;
    gl_vertices_[3] = offset.x / static_cast<float>(texture_->width_);
    gl_vertices_[4] = offset.y / static_cast<float>(texture_->height_);

    // Top right
    gl_vertices_[5] = top_left.x + width;
    gl_vertices_[6] = top_left.y;
    gl_vertices_[7] = 0.0F;
    gl_vertices_[8] = static_cast<float>(width) / static_cast<float>(texture_->width_) +
                      offset.x / static_cast<float>(texture_->width_);
    gl_vertices_[9] = offset.y / static_cast<float>(texture_->height_);

    // Bottom right
    gl_vertices_[10] = top_left.x + width;
    gl_vertices_[11] = top_left.y + height;
    gl_vertices_[12] = 0.0F;
    gl_vertices_[13] = static_cast<float>(width) / static_cast<float>(texture_->width_) +
                       offset.x / static_cast<float>(texture_->width_);
    gl_vertices_[14] = static_cast<float>(height) / static_cast<float>(texture_->height_) +
                       offset.y / static_cast<float>(texture_->height_);

    // Bottom left
    gl_vertices_[15] = top_left.x;
    gl_vertices_[16] = top_left.y + height;
    gl_vertices_[17] = 0.0F;
    gl_vertices_[18] = offset.x / static_cast<float>(texture_->width_);
    gl_vertices_[19] = static_cast<float>(height) / static_cast<float>(texture_->height_) +
                       offset.y / static_cast<float>(texture_->height_);

    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(gl_vertices_), gl_vertices_, GL_DYNAMIC_DRAW));
}

} // namespace rinvid
