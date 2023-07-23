/**********************************************************************
 * Copyright (c) 2022 - 2023, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef CORE_INCLUDE_CAMERA_H
#define CORE_INCLUDE_CAMERA_H

#include "extern/glm/glm/glm.hpp"
#include "extern/glm/glm/gtc/matrix_transform.hpp"
#include "extern/glm/glm/gtc/type_ptr.hpp"

#include "util/include/vector2.h"

namespace rinvid
{

class Camera
{
  public:
    /**************************************************************************************************
     * @brief Camera constructor.
     *
     *************************************************************************************************/
    Camera();

    /**************************************************************************************************
     * @brief Updates the camera.
     *
     * Should be called each frame.
     *
     *************************************************************************************************/
    void update();

    /**************************************************************************************************
     * @brief Moves the by adding move_vector to its position vector.
     *
     * @param move_vector Vector to be added to camera's position vector
     *
     *************************************************************************************************/
    void move(const Vector2<float> move_vector);

    /**************************************************************************************************
     * @brief Returns current camera position.
     *
     * @return camera position.
     *
     *************************************************************************************************/
    Vector2<float> get_pos() const;

  private:
    glm::vec3 camera_pos_;
    glm::vec3 camera_front_;
    glm::vec3 camera_up_;
};

} // namespace rinvid

#endif // CORE_INCLUDE_CAMERA_H
