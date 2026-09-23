/**********************************************************************
 * Copyright (c) 2023 - 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef INCLUDE_RINVID_SYSTEM_KEYBOARD_H
#define INCLUDE_RINVID_SYSTEM_KEYBOARD_H

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

    /**************************************************************************************************
     * @brief Checks whether a key was newly pressed during the current application frame.
     * Remains true for every query in that frame, even if the key was also released. Holding a key
     * does not generate additional press transitions. Focus loss clears pending transitions.
     *
     * @param key Key to check.
     *
     * @return True for a press transition, or false for an invalid key or no active application.
     *
     *************************************************************************************************/
    static bool is_key_just_pressed(Key key);

    /**************************************************************************************************
     * @brief Checks whether a key was released during the current application frame.
     * Remains true for every query in that frame. Focus loss clears state without generating
     * releases.
     *
     * @param key Key to check.
     *
     * @return True for a release transition, or false for an invalid key or no active application.
     *
     *************************************************************************************************/
    static bool is_key_just_released(Key key);
};

} // namespace system

} // namespace rinvid

#endif // INCLUDE_RINVID_SYSTEM_KEYBOARD_H
