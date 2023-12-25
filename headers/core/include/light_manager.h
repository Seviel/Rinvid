/**********************************************************************
 * Copyright (c) 2023, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef CORE_INCLUDE_LIGHT_MANAGER_H
#define CORE_INCLUDE_LIGHT_MANAGER_H

#include <cstdint>

namespace rinvid
{

/**************************************************************************************************
 * @brief A class for controlling lighting.
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

#endif // CORE_INCLUDE_LIGHT_MANAGER_H
