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
 * @brief An interface for objects that can transform (rotate and scale).
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

  protected:
    Vector2<float> origin_;

  private:
    glm::mat4 transform_;
};

} // namespace rinvid

#endif // CORE_INCLUDE_TRANSFORMABLE_H
