/**********************************************************************
 * Copyright (c) 2025, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include "include/world.h"

namespace rinvid
{

float World::gravity{800.0F};

bool World::collide(Object& object_1, Object& object_2)
{
    (void)object_1;
    (void)object_2;
    return false;
}

bool World::separate(Object& object_1, Object& object_2)
{
    (void)object_1;
    (void)object_2;
    return false;
}

bool World::separate_x(Object& object_1, Object& object_2)
{
    (void)object_1;
    (void)object_2;
    return false;
}

bool World::separate_y(Object& object_1, Object& object_2)
{
    (void)object_1;
    (void)object_2;
    return false;
}

} // namespace rinvid
