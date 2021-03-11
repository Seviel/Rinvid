/**********************************************************************
 * Copyright (c) 2021, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef CORE_INCLUDE_TRANSFORMABLE_H
#define CORE_INCLUDE_TRANSFORMABLE_H

#include "extern/glm/mat4x4.hpp"
#include "util/include/vector2.h"

namespace rinvid
{

/**************************************************************************************************
 * @brief A drawable triangle shape.
 *
 *************************************************************************************************/
class Transformable
{
  public:
    /**************************************************************************************************
     * @brief Transformable constructor.
     *
     *************************************************************************************************/
    Transformable();

    /**************************************************************************************************
     * @brief Returns transform of the object.
     *
     * @return Matrix composition of position/rotation/scale
     *
     *************************************************************************************************/
    const glm::mat4& get_transform();

    /**************************************************************************************************
     * @brief Returns center point of object.
     *
     * @return Center point of object
     *
     *************************************************************************************************/
    Vector2<float> get_origin();

    /**************************************************************************************************
     * @brief Rotates object
     *
     * @param degree_angle angle in degrees
     *
     *************************************************************************************************/
    void rotate(float degree_angle);

    /**************************************************************************************************
     * @brief Sets object's rotation
     *
     * @param degree_angle angle in degrees
     *
     *************************************************************************************************/
    void set_rotation(float degree_angle);

    /**************************************************************************************************
     * @brief Scales object
     *
     * @param scale how much to scale object (e.g. 0.5 means object will be half of its current
     * size)
     *
     *************************************************************************************************/
    void scale(float scale);

    /**************************************************************************************************
     * @brief Sets obejct's scale
     *
     * @param scale new scale of the object
     *
     *************************************************************************************************/
    void set_scale(float scale);

  protected:
    Vector2<float> origin_;

  private:
    glm::mat4 transform_;
    float     angle_;
    float     scale_;
};

} // namespace rinvid

#endif // CORE_INCLUDE_TRANSFORMABLE_H
