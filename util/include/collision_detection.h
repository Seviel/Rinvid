/**********************************************************************
 * Copyright (c) 2021, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef UTIL_COLLISION_DETECTION_H
#define UTIL_COLLISION_DETECTION_H

#include "util/include/rect.h"

namespace rinvid
{

/**************************************************************************************************
 * @brief Checks if two rects intersect
 *
 * @param rect1 first rect
 * @param rect2 second rect
 *
 * @return true rects intersect, false otherwise
 *
 *************************************************************************************************/
bool intersects(Rect rect1, Rect rect2);

} // namespace rinvid

#endif // UTIL_COLLISION_DETECTION_H
