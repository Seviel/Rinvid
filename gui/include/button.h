/**********************************************************************
 * Copyright (c) 2022 - 2025, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef GUI_INCLUDE_BUTTON_H
#define GUI_INCLUDE_BUTTON_H

#include "core/include/sprite.h"

namespace rinvid
{

namespace gui
{

/**************************************************************************************************
 * @brief A clickable button.
 *
 *************************************************************************************************/
class Button : public Sprite
{
  public:
    /**************************************************************************************************
     * @brief Button constructor.
     *
     *************************************************************************************************/
    Button();

    /**************************************************************************************************
     * @brief Updates the button status (clicked status and animation) based on mouse position.
     *
     *************************************************************************************************/
    void update_state();

    /**************************************************************************************************
     * @brief Sets animation regions for idle status (mouse is not hovering over button).
     *
     * @param regions Vector of rects indicating frames of texture that make up the animation.
     *
     *************************************************************************************************/
    void set_idle(const std::vector<Rect>& regions);

    /**************************************************************************************************
     * @brief Sets animation regions for when mouse is hovering over the button.
     *
     * @param regions Vector of rects indicating frames of texture that make up the animation.
     *
     *************************************************************************************************/
    void set_mouse_hovering(const std::vector<Rect>& regions);

    /**************************************************************************************************
     * @brief Sets animation regions for when the button is clicked.
     *
     * @param regions Vector of rects indicating frames of texture that make up the animation.
     *
     *************************************************************************************************/
    void set_clicked(const std::vector<Rect>& regions);

    /**************************************************************************************************
     * @brief Checks whether button is clicked on.
     *
     * @return True if button is clicked, false otherwise.
     *
     *************************************************************************************************/
    bool is_clicked() const;

    /**************************************************************************************************
     * @brief Checks whether button is "just clicked". Will return true only once per click.
     *
     * @return True if button is just clicked, false otherwise.
     *
     *************************************************************************************************/
    bool just_clicked();

  private:
    Animation idle_;
    Animation mouse_hovering_;
    Animation clicked_;
    bool      is_clicked_;
    bool      just_clicked_;
};

} // namespace gui

} // namespace rinvid

#endif // GUI_INCLUDE_BUTTON_H
