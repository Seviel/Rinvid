/**********************************************************************
 * Copyright (c) 2023 - 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <rinvid/system/application_context.h>
#include <rinvid/system/keyboard.h>

namespace rinvid
{

namespace system
{

bool Keyboard::is_key_pressed(Key key)
{
    return sf::Keyboard::isKeyPressed(key);
}

bool Keyboard::is_key_just_pressed(Key key)
{
    const auto* context{ApplicationContext::get_active_context()};
    return context != nullptr && context->get_input_state().is_key_just_pressed(key);
}

bool Keyboard::is_key_just_released(Key key)
{
    const auto* context{ApplicationContext::get_active_context()};
    return context != nullptr && context->get_input_state().is_key_just_released(key);
}

} // namespace system

} // namespace rinvid
