/**********************************************************************
 * Copyright (c) 2025 - 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef PLATFORMERS_INCLUDE_WORLD_H
#define PLATFORMERS_INCLUDE_WORLD_H

#include <vector>

#include "core/include/object.h"

namespace rinvid
{

typedef bool (*CollisionResolver)(Object&, Object&);

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
     * @brief Sets world gravity which affects how fast objects accelerate downwards when falling.
     *
     * @param gravity New gravity level. Default is 800.0F which gives kinda slow floaty fall
     * effect.
     *
     *************************************************************************************************/
    static void set_gravity(float gravity);

    /**************************************************************************************************
     * @brief Checks whether objects collide and handles collision via callback function.
     *
     * @param object_1
     * @param object_2
     * @param resolve Pointer to function that decides how to resolve collision when it happens.
     * Default is to separate objects.
     *
     * @return True if objects collide.
     *
     *************************************************************************************************/
    static bool collide(Object& object_1, Object& object_2, CollisionResolver resolve = separate);

    /**************************************************************************************************
     * @brief Checks whether object collides with group of objects and handles collisions via
     * callback function.
     *
     * @param object
     * @param group
     * @param resolve Pointer to function that decides how to resolve collision when it happens.
     * Default is to separate objects.
     *
     * @return True if object collides with any of the objects from group.
     *
     *************************************************************************************************/
    static bool collide(Object& object, const std::vector<Object*>& group,
                        CollisionResolver resolve = separate);

    /**************************************************************************************************
     * @brief Checks whether object collides with group of objects and handles collisions via
     * callback function.
     *
     * @param group
     * @param object
     * @param resolve Pointer to function that decides how to resolve collision when it happens.
     * Default is to separate objects.
     *
     * @return True if object collides with any of the objects from group.
     *
     *************************************************************************************************/
    static bool collide(const std::vector<Object*>& group, Object& object,
                        CollisionResolver resolve = separate);

    /**************************************************************************************************
     * @brief Checks whether object collides with group of objects and handles collisions via
     * callback function.
     * WARNING: This function is probably not optimal for large number of objects.
     *
     * @param group_1
     * @param group_2
     * @param resolve Pointer to function that decides how to resolve collision when it happens.
     * Default is to separate objects.
     *
     * @return True if aby objects from group collide.
     *
     *************************************************************************************************/
    static bool collide(const std::vector<Object*>& group_1, const std::vector<Object*>& group_2,
                        CollisionResolver resolve = separate);

    static float gravity;

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

  private:
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
