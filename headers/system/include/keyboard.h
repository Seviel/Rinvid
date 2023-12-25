/**********************************************************************
 * Copyright (c) 2023, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef SYSTEM_INCLUDE_KEYBOARD_H
#define SYSTEM_INCLUDE_KEYBOARD_H

#include <SFML/Window.hpp>

namespace rinvid
{

namespace system
{

/**************************************************************************************************
 * @brief Gives access to keyboard. Basically a wrapper around sf::Keyboard
 *
 *************************************************************************************************/
class Keyboard
{
  public:
    using Key = sf::Keyboard::Key;

    /**************************************************************************************************
     * @brief Check whether key is pressed.
     *
     * @param Key the key to check.
     *
     *************************************************************************************************/
    static bool is_key_pressed(Key key);
};

} // namespace system

} // namespace rinvid

#endif // SYSTEM_INCLUDE_KEYBOARD_H
