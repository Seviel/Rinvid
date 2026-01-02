/**********************************************************************
 * Copyright (c) 2022 - 2026, Filip Vasiljevic
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
     * @brief Sets camera position.
     *
     * @param position New position.
     *
     *************************************************************************************************/
    void set_position(const Vector2f position);

    /**************************************************************************************************
     * @brief Moves the by adding move_vector to its position vector.
     *
     * @param move_vector Vector to be added to camera's position vector
     *
     *************************************************************************************************/
    void move(const Vector2f move_vector);

    /**************************************************************************************************
     * @brief Returns current camera position.
     *
     * @return camera position.
     *
     *************************************************************************************************/
    Vector2f get_pos() const;

    /**************************************************************************************************
     * @brief Sets limits to camera movement.
     *
     * @param upper_left x component of upper_left vector limits camera left movement, y component
     * limits camera up movement.
     * @param lower_right x component of lower_right vector limits camera right movement, y
     * component limits camera down movement.
     *
     *************************************************************************************************/
    void set_borders(Vector2f upper_left, Vector2f lower_right);

    /**************************************************************************************************
     * @brief Sets limits to camera movement on X axis.
     *
     * @param left Left limit of camera movement.
     * @param right Right limit of camera movement.
     *
     *************************************************************************************************/
    void set_x_borders(float left, float right);

    /**************************************************************************************************
     * @brief Sets limits to camera movement on Y axis.
     *
     * @param up Upper limit of camera movement.
     * @param down Lower limit of camera movement.
     *
     *************************************************************************************************/
    void set_y_borders(float up, float down);

  private:
    glm::vec3 camera_pos_;
    glm::vec3 camera_front_;
    glm::vec3 camera_up_;

    Vector2f upper_left_border_;
    Vector2f lower_right_border_;
};

} // namespace rinvid

#endif // CORE_INCLUDE_CAMERA_H
