/**********************************************************************
 * Copyright (c) 2023 - 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <algorithm>

#include <rinvid/core/light_manager.h>
#include <rinvid/core/render_context.h>

namespace rinvid
{

void LightManager::activate_ambient_light(float strength)
{
    set_ambient_light(true, strength);
}

void LightManager::deactivate_ambient_light()
{
    set_ambient_light(false, 0.0F);
}

void LightManager::set_ambient_light(bool enabled, float strength)
{
    strength = std::clamp(strength, 0.0F, 1.0F);

    RenderContext* render_context = RenderContext::get_active_context();
    if (render_context == nullptr)
    {
        return;
    }

    render_context->set_default_ambient_light(enabled, strength);
}

} // namespace rinvid
