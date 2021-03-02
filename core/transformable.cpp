/**********************************************************************
 * Copyright (c) 2021, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include "include/transformable.h"

namespace rinvid
{

Transformable::Transformable() : transform_{1.0F}
{
}

const glm::mat4& Transformable::get_transform()
{
    return transform_;
}

} // namespace rinvid
