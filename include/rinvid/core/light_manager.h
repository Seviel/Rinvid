/**********************************************************************
 * Copyright (c) 2023 - 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef INCLUDE_RINVID_CORE_LIGHT_MANAGER_H
#define INCLUDE_RINVID_CORE_LIGHT_MANAGER_H

#include <cstdint>

namespace rinvid
{

/**************************************************************************************************
 * @brief A class for controlling lighting. Only works if you're using default shaders.
 *
 * All functions and members are static.
 *
 *************************************************************************************************/
class LightManager
{
  public:
    /**************************************************************************************************
     * @brief Activates ambient lighting
     *
     * @param strength Intensity in 0.0 - 1.0 range
     *
     *************************************************************************************************/
    static void activate_ambient_light(float strength = 0.1F);
};

} // namespace rinvid

#endif // INCLUDE_RINVID_CORE_LIGHT_MANAGER_H
