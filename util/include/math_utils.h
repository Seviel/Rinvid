#ifndef UTIL_MATH_UTILS_H
#define UTIL_MATH_UTILS_H

constexpr float DEGREES_IN_A_CIRCLE = 360.0F;

#ifndef DEGREES_TO_RADIANS
#define DEGREES_TO_RADIANS(angle) ((angle) * static_cast<float>(M_PI) / 180.0F)
#endif

#endif // UTIL_MATH_UTILS_H
