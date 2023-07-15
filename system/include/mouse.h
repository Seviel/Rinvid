/**********************************************************************
 * Copyright (c) 2023, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef SYSTEM_INCLUDE_MOUSE_H
#define SYSTEM_INCLUDE_MOUSE_H

#include <SFML/Window.hpp>

namespace rinvid
{

namespace system
{

/**************************************************************************************************
 * @brief Gives access to mouse. Basically a wrapper around sf::Mouse
 *
 *************************************************************************************************/
class Mouse
{
  public:
    enum MouseButton
    {
        Left,
        Right
    };

    /**************************************************************************************************
     * @brief Check whether button is pressed.
     *
     * @param mouse_button which mouse button.
     *
     *************************************************************************************************/
    static bool is_button_pressed(MouseButton mouse_button);
};

} // namespace system

} // namespace rinvid

#endif // SYSTEM_INCLUDE_MOUSE_H
