/**********************************************************************
 * Copyright (c) 2025, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef PLATFORMERS_INCLUDE_WORLD_H
#define PLATFORMERS_INCLUDE_WORLD_H

#include "core/include/object.h"

namespace rinvid
{

class World
{
  public:
    /**************************************************************************************************
     * @brief Default constructor.
     *
     *************************************************************************************************/
    World()
    {
    }

    /**************************************************************************************************
     * @brief Checks whether objects collide and handles collisions by separating them.
     *
     * @param object_1
     * @param object_2
     *
     * @return True if objects collide.
     *
     *************************************************************************************************/
    static bool collide(Object& object_1, Object& object_2);

    static float gravity;

  private:
    /**************************************************************************************************
     * @brief Called by 'collide' if objects collided to separate them.
     *
     * @param object_1
     * @param object_2
     *
     * @return True if objects were separated.
     *
     *************************************************************************************************/
    static bool separate(Object& object_1, Object& object_2);

    /**************************************************************************************************
     * @brief Called by 'separate' to separate objects on x axis.
     *
     * @param object_1
     * @param object_2
     *
     * @return True if objects were separated.
     *
     *************************************************************************************************/
    static bool separate_x(Object& object_1, Object& object_2);

    /**************************************************************************************************
     * @brief Called by 'separate' to separate objects on y axis.
     *
     * @param object_1
     * @param object_2
     *
     * @return True if objects were separated.
     *
     *************************************************************************************************/
    static bool separate_y(Object& object_1, Object& object_2);
};

} // namespace rinvid

#endif // PLATFORMERS_INCLUDE_WORLD_H
