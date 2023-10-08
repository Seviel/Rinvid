/**********************************************************************
 * Copyright (c) 2023, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <SFML/Window.hpp>

#include "include/mouse.h"

namespace rinvid
{

namespace system
{

bool Mouse::is_button_pressed(MouseButton button)
{
    sf::Mouse::Button sf_button = sf::Mouse::Button::Left;
    if (button == Right)
    {
        sf_button = sf::Mouse::Button::Right;
    }

    return sf::Mouse::isButtonPressed(sf_button);
}

} // namespace system

} // namespace rinvid
