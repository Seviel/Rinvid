/**********************************************************************
 * Copyright (c) 2021, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef CORE_INCLUDE_DRAWABLE_H
#define CORE_INCLUDE_DRAWABLE_H

namespace rinvid
{

/**************************************************************************************************
 * @brief An interface for drawable objects.
 *
 *************************************************************************************************/
class Drawable
{
  public:
    /**************************************************************************************************
     * @brief Drawable constructor.
     *
     *************************************************************************************************/
    Drawable(){};

    /**************************************************************************************************
     * @brief Draws the object. Use this function for objects that are not animated.
     *
     *************************************************************************************************/
    virtual void draw() = 0;

    /**************************************************************************************************
     * @brief Draws the object. Use this function for drawing animated objects.
     *
     *************************************************************************************************/
    virtual void draw(double delta_time) = 0;
};

} // namespace rinvid

#endif // CORE_INCLUDE_DRAWABLE_H
