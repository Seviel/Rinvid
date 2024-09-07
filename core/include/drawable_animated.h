/**********************************************************************
 * Copyright (c) 2023 - 2024, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef CORE_INCLUDE_DRAWABLE_ANIMATED_H
#define CORE_INCLUDE_DRAWABLE_ANIMATED_H

#include "core/include/drawable.h"

namespace rinvid
{

/**************************************************************************************************
 * @brief An interface for animated drawable objects.
 *
 *************************************************************************************************/
class DrawableAnimated : public Drawable
{
  public:
    /**************************************************************************************************
     * @brief DrawableAnimated constructor.
     *
     *************************************************************************************************/
    DrawableAnimated(){};

    /**************************************************************************************************
     * @brief DrawableAnimated virtual destructor.
     *
     *************************************************************************************************/
    virtual ~DrawableAnimated(){};

    /**************************************************************************************************
     * @brief Draws the animated object.
     *
     *************************************************************************************************/
    virtual void draw(double delta_time) = 0;

    /**************************************************************************************************
     * @brief Draws the animated object with shader applied.
     *
     * @param shader Shader to use.
     *
     *************************************************************************************************/
    virtual void draw(double delta_time, const Shader shader) = 0;
};

} // namespace rinvid

#endif // CORE_INCLUDE_DRAWABLE_ANIMATED_H
