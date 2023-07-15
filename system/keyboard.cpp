/**********************************************************************
 * Copyright (c) 2023, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include "include/keyboard.h"

namespace rinvid
{

namespace system
{

bool Keyboard::is_key_pressed(Key key)
{
    return sf::Keyboard::isKeyPressed(key);
}

} // namespace system

} // namespace rinvid
