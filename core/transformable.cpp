/**********************************************************************
 * Copyright (c) 2021 - 2022, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include "include/transformable.h"
#include "extern/glm/glm/gtx/transform.hpp"

namespace rinvid
{

Transformable::Transformable() : origin_{}, transform_{1.0F}, angle_{0.0F}, scale_{1.0F}
{
}

const glm::mat4& Transformable::get_transform()
{
    transform_ = glm::mat4{1.0F};
    transform_ = glm::translate(transform_, glm::vec3(origin_.x, origin_.y, 0.0f));
    transform_ = glm::scale(transform_, glm::vec3{scale_, scale_, 1.0F});
    transform_ = glm::rotate(transform_, glm::radians(angle_), glm::vec3{0.0F, 0.0F, 1.0F});
    transform_ =
        glm::translate(transform_, glm::vec3(origin_.x * (-1.0F), origin_.y * (-1.0F), 0.0f));

    return transform_;
}

Vector2<float> Transformable::get_origin()
{
    return origin_;
}

void Transformable::rotate(float degree_angle)
{
    angle_ += degree_angle;
}

void Transformable::set_rotation(float degree_angle)
{
    angle_ = degree_angle;
}

void Transformable::scale(float scale)
{
    scale_ *= scale;
}

void Transformable::set_scale(float scale)
{
    scale_ = scale;
}

bool Transformable::is_transformed() const
{
    return (angle_ != 0.0F) || (scale_ != 1.0F);
}

} // namespace rinvid
