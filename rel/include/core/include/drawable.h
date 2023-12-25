/**********************************************************************
 * Copyright (c) 2021 - 2023, Filip Vasiljevic
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
     * @brief Drawable virtual destructor.
     *
     *************************************************************************************************/
    virtual ~Drawable(){};

    /**************************************************************************************************
     * @brief Draws the object.
     *
     *************************************************************************************************/
    virtual void draw() = 0;
};

} // namespace rinvid

#endif // CORE_INCLUDE_DRAWABLE_H
