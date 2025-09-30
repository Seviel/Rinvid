/**********************************************************************
 * Copyright (c) 2022 - 2025, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include "core/include/camera.h"
#include "core/include/rinvid_gfx.h"

namespace rinvid
{

Camera::Camera()
    : camera_pos_{0.0F, 0.0F, 1.0F}, camera_front_{0.0F, 0.0F, -1.0F}, camera_up_{0.0F, 1.0F, 0.0F}
{
    camera_pos_ += 1.5F * camera_front_;
}

void Camera::update()
{
    auto view = glm::lookAt(camera_pos_, camera_pos_ + camera_front_, camera_up_);
    RinvidGfx::update_view(view);
}

void Camera::set_position(const Vector2f position)
{
    Vector2f current_pos = get_pos();
    Vector2f move_vector{};
    move_vector.x = position.x - current_pos.x;
    move_vector.y = position.y - current_pos.y;
    move(move_vector);
}

void Camera::move(const Vector2f move_vector)
{
    camera_pos_ += move_vector.y * camera_up_;
    camera_pos_ += glm::normalize(glm::cross(camera_front_, camera_up_)) * move_vector.x;
}

Vector2f Camera::get_pos() const
{
    return Vector2f{camera_pos_.x, camera_pos_.y};
}

} // namespace rinvid
