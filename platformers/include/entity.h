/**********************************************************************
 * Copyright (c) 2025, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef PLATFORMERS_INCLUDE_ENTITY_H
#define PLATFORMERS_INCLUDE_ENTITY_H

#include "core/include/sprite.h"
#include "core/include/object.h"

namespace rinvid
{

class Entity : public Sprite, public Object
{
  public:
    /**************************************************************************************************
     * @brief Default constructor.
     *
     *************************************************************************************************/
    Entity();
};

} // namespace rinvid

#endif // PLATFORMERS_INCLUDE_ENTITY_H
