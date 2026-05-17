/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef INCLUDE_RINVID_GUI_STACK_LAYOUT_H
#define INCLUDE_RINVID_GUI_STACK_LAYOUT_H

#include <cstddef>
#include <vector>

#include <rinvid/gui/widget.h>

namespace rinvid
{

namespace gui
{

/**************************************************************************************************
 * @brief Direction in which a stack layout places children.
 *
 *************************************************************************************************/
enum class StackLayoutOrientation
{
    Vertical = 0U,
    Horizontal
};

/**************************************************************************************************
 * @brief A simple non-owning layout container that stacks child widgets.
 *
 *************************************************************************************************/
class StackLayout : public Widget
{
  public:
    /**************************************************************************************************
     * @brief StackLayout constructor.
     *
     * @param position Top-left layout position.
     * @param orientation Direction in which child widgets are placed.
     *
     *************************************************************************************************/
    StackLayout(Vector2f               position    = {0.0F, 0.0F},
                StackLayoutOrientation orientation = StackLayoutOrientation::Vertical);

    /**************************************************************************************************
     * @brief Adds a child widget to the layout.
     *
     * The layout does not take ownership of the widget.
     *
     * @param widget Child widget.
     *
     *************************************************************************************************/
    void add(Widget* widget);

    /**************************************************************************************************
     * @brief Removes a child widget from the layout.
     *
     * @param widget Child widget.
     *
     *************************************************************************************************/
    void remove(Widget* widget);

    /**************************************************************************************************
     * @brief Removes all child widgets from the layout.
     *
     *************************************************************************************************/
    void clear();

    /**************************************************************************************************
     * @brief Repositions all child widgets according to current layout settings.
     *
     *************************************************************************************************/
    void layout();

    /**************************************************************************************************
     * @brief Sets stack orientation.
     *
     * @param orientation New stack orientation.
     *
     *************************************************************************************************/
    void set_orientation(StackLayoutOrientation orientation);

    /**************************************************************************************************
     * @brief Returns stack orientation.
     *
     * @return Stack orientation.
     *
     *************************************************************************************************/
    StackLayoutOrientation get_orientation() const;

    /**************************************************************************************************
     * @brief Sets spacing between child widgets.
     *
     * @param spacing Spacing in pixels.
     *
     *************************************************************************************************/
    void set_spacing(float spacing);

    /**************************************************************************************************
     * @brief Returns spacing between child widgets.
     *
     * @return Spacing in pixels.
     *
     *************************************************************************************************/
    float get_spacing() const;

    /**************************************************************************************************
     * @brief Sets padding around child widgets.
     *
     * @param padding Padding in pixels.
     *
     *************************************************************************************************/
    void set_padding(float padding);

    /**************************************************************************************************
     * @brief Returns padding around child widgets.
     *
     * @return Padding in pixels.
     *
     *************************************************************************************************/
    float get_padding() const;

    /**************************************************************************************************
     * @brief Returns number of child widgets.
     *
     * @return Number of child widgets.
     *
     *************************************************************************************************/
    std::size_t get_child_count() const;

    /**************************************************************************************************
     * @brief Updates child widgets if the layout is visible and enabled.
     *
     * @param delta_time Time passed in seconds since last frame.
     *
     *************************************************************************************************/
    virtual void update(double delta_time) override;

    /**************************************************************************************************
     * @brief Draws child widgets if the layout is visible.
     *
     *************************************************************************************************/
    virtual void draw() override;

    /**************************************************************************************************
     * @brief Draws child widgets if the layout is visible.
     *
     * @param delta_time Time passed in seconds since last frame.
     *
     *************************************************************************************************/
    virtual void draw(double delta_time) override;

    /**************************************************************************************************
     * @brief Moves the layout and child widgets.
     *
     * @param move_vector Vector to add to current position.
     *
     *************************************************************************************************/
    virtual void move(const Vector2f move_vector) override;

    /**************************************************************************************************
     * @brief Sets layout position and repositions child widgets.
     *
     * @param position New top-left layout position.
     *
     *************************************************************************************************/
    virtual void set_position(const Vector2f position) override;

    /**************************************************************************************************
     * @brief Returns layout bounds.
     *
     * @return Layout bounds.
     *
     *************************************************************************************************/
    virtual Rect get_bounds() override;

    /**************************************************************************************************
     * @brief Sets whether the layout should be drawn and updated.
     *
     * @param visible True to show the layout, false to hide it.
     *
     *************************************************************************************************/
    virtual void set_visible(bool visible) override;

    /**************************************************************************************************
     * @brief Checks whether the layout is visible.
     *
     * @return True if the layout is visible, false otherwise.
     *
     *************************************************************************************************/
    virtual bool is_visible() const override;

    /**************************************************************************************************
     * @brief Sets whether child widgets should be updated.
     *
     * @param enabled True to update child widgets, false to disable updates.
     *
     *************************************************************************************************/
    virtual void set_enabled(bool enabled) override;

    /**************************************************************************************************
     * @brief Checks whether child widgets are updated.
     *
     * @return True if the layout updates child widgets, false otherwise.
     *
     *************************************************************************************************/
    virtual bool is_enabled() const override;

  private:
    std::vector<Widget*>   children_;
    Vector2f               position_;
    StackLayoutOrientation orientation_;
    float                  spacing_;
    float                  padding_;
    bool                   visible_;
    bool                   enabled_;
};

} // namespace gui

} // namespace rinvid

#endif // INCLUDE_RINVID_GUI_STACK_LAYOUT_H
