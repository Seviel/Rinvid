/**********************************************************************
 * Copyright (c) 2022, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef GUI_INCLUDE_BUTTON_H
#define GUI_INCLUDE_BUTTON_H

#include <SFML/Window.hpp>

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

    void update(Vector2<float> mouse_pos);

    void set_idle(const std::vector<Rect>& regions);

    void set_mouse_hovering(const std::vector<Rect>& regions);

    void set_clicked(const std::vector<Rect>& regions);

    bool is_clicked() const;

  private:
    Animation idle_;
    Animation mouse_hovering_;
    Animation clicked_;
    bool      is_clicked_;
};

} // namespace gui

} // namespace rinvid

#endif // GUI_INCLUDE_BUTTON_H
