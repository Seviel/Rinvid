/**********************************************************************
 * Copyright (c) 2023 - 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef INCLUDE_RINVID_SYSTEM_MOUSE_H
#define INCLUDE_RINVID_SYSTEM_MOUSE_H

#include <rinvid/util/vector2.h>

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

    /**************************************************************************************************
     * @brief Checks whether a mouse button was newly pressed during the current application frame.
     * Remains true for every query in that frame, even if the button was also released. Holding a
     * button does not generate additional press transitions. Focus loss clears pending transitions.
     *
     * @param mouse_button Mouse button to check.
     *
     * @return True for a press transition, or false for an invalid button or no active application.
     *
     *************************************************************************************************/
    static bool is_button_just_pressed(MouseButton mouse_button);

    /**************************************************************************************************
     * @brief Checks whether a mouse button was released during the current application frame.
     * Remains true for every query in that frame. Focus loss clears state without generating
     * releases.
     *
     * @param mouse_button Mouse button to check.
     *
     * @return True for a release transition, or false for an invalid button or no active
     * application.
     *
     *************************************************************************************************/
    static bool is_button_just_released(MouseButton mouse_button);

    /**************************************************************************************************
     * @brief Returns mouse position
     *
     * @return Vector2 representing mouse position in 2D space
     *
     *************************************************************************************************/
    static Vector2f get_mouse_pos();
};

} // namespace system

} // namespace rinvid

#endif // INCLUDE_RINVID_SYSTEM_MOUSE_H
