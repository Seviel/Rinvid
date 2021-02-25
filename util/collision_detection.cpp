/**********************************************************************
 * Copyright (c) 2021, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include "util/include/collision_detection.h"
#include "util/include/rect.h"

namespace rinvid
{

bool intersects(Rect rect1, Rect rect2)
{
    if (rect1.position.x > (rect2.position.x + rect2.width))
    {
        return false;
    }
    else if ((rect1.position.x + rect1.width) < rect2.position.x)
    {
        return false;
    }
    else if (rect1.position.y > (rect2.position.y + rect2.height))
    {
        return false;
    }
    else if ((rect1.position.y + rect1.height) < rect2.position.y)
    {
        return false;
    }

    return true;
}

} // namespace rinvid
