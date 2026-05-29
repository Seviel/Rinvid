/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef INCLUDE_RINVID_GUI_WIDGET_H
#define INCLUDE_RINVID_GUI_WIDGET_H

#include <rinvid/util/rect.h>
#include <rinvid/util/vector2.h>

namespace rinvid
{

namespace gui
{

/**************************************************************************************************
 * @brief Common interface for GUI components that can be placed in containers.
 *
 *************************************************************************************************/
class Widget
{
  public:
    /**************************************************************************************************
     * @brief Widget virtual destructor.
     *
     *************************************************************************************************/
    virtual ~Widget() = default;

    /**************************************************************************************************
     * @brief Updates widget state.
     *
     * @param delta_time Time passed in seconds since last frame.
     *
     *************************************************************************************************/
    virtual void update(double delta_time);

    /**************************************************************************************************
     * @brief Draws the widget.
     *
     *************************************************************************************************/
    virtual void draw() = 0;

    /**************************************************************************************************
     * @brief Draws the widget.
     *
     * @param delta_time Time passed in seconds since last frame.
     *
     *************************************************************************************************/
    virtual void draw(double delta_time);

    /**************************************************************************************************
     * @brief Moves the widget.
     *
     * @param move_vector Vector to add to current position.
     *
     *************************************************************************************************/
    virtual void move(const Vector2f move_vector) = 0;

    /**************************************************************************************************
     * @brief Sets widget position.
     *
     * @param position New position.
     *
     *************************************************************************************************/
    virtual void set_position(const Vector2f position) = 0;

    /**************************************************************************************************
     * @brief Returns widget bounds.
     *
     * @return Widget bounds.
     *
     *************************************************************************************************/
    virtual Rect get_bounds() = 0;

    /**************************************************************************************************
     * @brief Sets whether the widget should be drawn and updated.
     *
     * @param visible True to show the widget, false to hide it.
     *
     *************************************************************************************************/
    virtual void set_visible(bool visible) = 0;

    /**************************************************************************************************
     * @brief Checks whether the widget is visible.
     *
     * @return True if the widget is visible, false otherwise.
     *
     *************************************************************************************************/
    virtual bool is_visible() const = 0;

    /**************************************************************************************************
     * @brief Sets whether the widget should react to input.
     *
     * @param enabled True to enable the widget, false to disable it.
     *
     *************************************************************************************************/
    virtual void set_enabled(bool enabled);

    /**************************************************************************************************
     * @brief Checks whether the widget reacts to input.
     *
     * @return True if the widget is enabled, false otherwise.
     *
     *************************************************************************************************/
    virtual bool is_enabled() const;
};

} // namespace gui

} // namespace rinvid

#endif // INCLUDE_RINVID_GUI_WIDGET_H
