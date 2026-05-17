/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef INCLUDE_RINVID_GUI_PANEL_H
#define INCLUDE_RINVID_GUI_PANEL_H

#include <cstddef>
#include <memory>
#include <vector>

#include <rinvid/core/rectangle_shape.h>
#include <rinvid/gui/widget.h>
#include <rinvid/util/color.h>

namespace rinvid
{

namespace gui
{

/**************************************************************************************************
 * @brief A simple rectangular container for grouping GUI widgets.
 *
 *************************************************************************************************/
class Panel : public Widget
{
  public:
    /**************************************************************************************************
     * @brief Panel constructor.
     *
     * @param position Top-left panel position.
     * @param width Panel width.
     * @param height Panel height.
     *
     *************************************************************************************************/
    Panel(Vector2f position, float width, float height);

    /**************************************************************************************************
     * @brief Adds a child widget to the panel.
     *
     * The panel does not take ownership of the widget. The local position is relative to the
     * padded content area of the panel.
     *
     * @param widget Child widget.
     * @param local_position Position relative to the padded content area.
     *
     *************************************************************************************************/
    void add(Widget* widget, Vector2f local_position);

    /**************************************************************************************************
     * @brief Removes a child widget from the panel.
     *
     * @param widget Child widget.
     *
     *************************************************************************************************/
    void remove(Widget* widget);

    /**************************************************************************************************
     * @brief Removes all child widgets from the panel.
     *
     *************************************************************************************************/
    void clear();

    /**************************************************************************************************
     * @brief Sets panel size.
     *
     * @param width Panel width.
     * @param height Panel height.
     *
     *************************************************************************************************/
    void set_size(float width, float height);

    /**************************************************************************************************
     * @brief Returns panel width.
     *
     * @return Panel width.
     *
     *************************************************************************************************/
    float get_width() const;

    /**************************************************************************************************
     * @brief Returns panel height.
     *
     * @return Panel height.
     *
     *************************************************************************************************/
    float get_height() const;

    /**************************************************************************************************
     * @brief Sets panel background color.
     *
     * @param color Background color.
     *
     *************************************************************************************************/
    void set_background_color(Color color);

    /**************************************************************************************************
     * @brief Sets panel border color.
     *
     * @param color Border color.
     *
     *************************************************************************************************/
    void set_border_color(Color color);

    /**************************************************************************************************
     * @brief Sets panel border thickness.
     *
     * @param thickness Border thickness in pixels.
     *
     *************************************************************************************************/
    void set_border_thickness(float thickness);

    /**************************************************************************************************
     * @brief Returns panel border thickness.
     *
     * @return Border thickness in pixels.
     *
     *************************************************************************************************/
    float get_border_thickness() const;

    /**************************************************************************************************
     * @brief Sets panel content padding.
     *
     * @param padding Padding in pixels.
     *
     *************************************************************************************************/
    void set_padding(float padding);

    /**************************************************************************************************
     * @brief Returns panel content padding.
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
     * @brief Updates child widgets if the panel is visible and enabled.
     *
     * @param delta_time Time passed in seconds since last frame.
     *
     *************************************************************************************************/
    virtual void update(double delta_time) override;

    /**************************************************************************************************
     * @brief Draws the panel and child widgets if visible.
     *
     *************************************************************************************************/
    virtual void draw() override;

    /**************************************************************************************************
     * @brief Draws the panel and child widgets if visible.
     *
     * @param delta_time Time passed in seconds since last frame.
     *
     *************************************************************************************************/
    virtual void draw(double delta_time) override;

    /**************************************************************************************************
     * @brief Moves the panel and child widgets.
     *
     * @param move_vector Vector to add to current position.
     *
     *************************************************************************************************/
    virtual void move(const Vector2f move_vector) override;

    /**************************************************************************************************
     * @brief Sets panel position and repositions child widgets.
     *
     * @param position New top-left panel position.
     *
     *************************************************************************************************/
    virtual void set_position(const Vector2f position) override;

    /**************************************************************************************************
     * @brief Returns panel bounds.
     *
     * @return Panel bounds.
     *
     *************************************************************************************************/
    virtual Rect get_bounds() override;

    /**************************************************************************************************
     * @brief Sets whether the panel should be drawn and updated.
     *
     * @param visible True to show the panel, false to hide it.
     *
     *************************************************************************************************/
    virtual void set_visible(bool visible) override;

    /**************************************************************************************************
     * @brief Checks whether the panel is visible.
     *
     * @return True if the panel is visible, false otherwise.
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
     * @return True if the panel updates child widgets, false otherwise.
     *
     *************************************************************************************************/
    virtual bool is_enabled() const override;

  private:
    struct Child
    {
        Widget*  widget;
        Vector2f local_position;
    };

    void update_shapes();
    void layout_child(Child& child);
    void layout_children();

    std::vector<Child>              children_;
    Vector2f                        position_;
    float                           width_;
    float                           height_;
    float                           border_thickness_;
    float                           padding_;
    Color                           background_color_;
    Color                           border_color_;
    std::unique_ptr<RectangleShape> background_;
    std::unique_ptr<RectangleShape> border_;
    bool                            visible_;
    bool                            enabled_;
};

} // namespace gui

} // namespace rinvid

#endif // INCLUDE_RINVID_GUI_PANEL_H
