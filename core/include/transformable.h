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

#include <algorithm>
#include <vector>

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

    /**************************************************************************************************
     * @brief Checks whether any transformation (rotation or scaling) had been applied to the object
     *
     * @return true if object had been transformed, false otherwise
     *
     *************************************************************************************************/
    bool is_transformed() const;

  protected:
    /**************************************************************************************************
     * @brief Given a vector of vertices, it sets min and max x and y coordinates. Helper function
     * for finding bounding rect.
     *
     * @param vertices A vector of vertices which needs to contain at least a position in 2D space
     *(x and y coordinate)
     * @param min_x Will be set to value of minimal x coordinate out of all vertices
     * @param max_x Will be set to value of maximal x coordinate out of all vertices
     * @param min_y Will be set to value of maximum y coordinate out of all vertices
     * @param max_y Will be set to value of maximum y coordinate out of all vertices
     *
     *************************************************************************************************/
    template <typename T>
    void set_min_max_coords(const std::vector<T>& vertices, float& min_x, float& max_x,
                            float& min_y, float& max_y)
    {
        auto min_x_vector = std::min_element(vertices.begin(), vertices.end(),
                                             [](T first, T second) { return first.x < second.x; });
        auto max_x_vector = std::max_element(vertices.begin(), vertices.end(),
                                             [](T first, T second) { return first.x < second.x; });

        auto min_y_vector = std::min_element(vertices.begin(), vertices.end(),
                                             [](T first, T second) { return first.y < second.y; });
        auto max_y_vector = std::max_element(vertices.begin(), vertices.end(),
                                             [](T first, T second) { return first.y < second.y; });

        min_x = min_x_vector->x;
        max_x = max_x_vector->x;
        min_y = min_y_vector->y;
        max_y = max_y_vector->y;
    }

    Vector2<float> origin_;

  private:
    glm::mat4 transform_;
    float     angle_;
    float     scale_;
};

} // namespace rinvid

#endif // CORE_INCLUDE_TRANSFORMABLE_H
