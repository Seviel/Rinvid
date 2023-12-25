/**********************************************************************
 * Copyright (c) 2020, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef UTIL_MATH_UTILS_H
#define UTIL_MATH_UTILS_H

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

constexpr float DEGREES_IN_A_CIRCLE = 360.0F;

#ifndef DEGREES_TO_RADIANS
#define DEGREES_TO_RADIANS(angle) ((angle) * static_cast<float>(M_PI) / 180.0F)
#endif

#endif // UTIL_MATH_UTILS_H
