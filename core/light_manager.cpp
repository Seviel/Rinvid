/**********************************************************************
 * Copyright (c) 2023, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include "include/light_manager.h"
#include "include/rinvid_gfx.h"

namespace rinvid
{

bool LightManager::using_ambient_light_{false};

void LightManager::activate_ambient_light(float strength)
{
    (void)strength;
    const auto shape_shader = RinvidGfx::get_shape_default_shader();
    shape_shader->use();
    shape_shader->set_bool("use_ambient_light", true);
    shape_shader->set_float("ambient_strength", strength);

    const auto texture_shader = RinvidGfx::get_texture_default_shader();
    texture_shader->use();
    texture_shader->set_bool("use_ambient_light", true);
    texture_shader->set_float("ambient_strength", strength);
}

} // namespace rinvid
