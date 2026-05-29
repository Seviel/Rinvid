/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef INCLUDE_RINVID_GUI_SLIDER_H
#define INCLUDE_RINVID_GUI_SLIDER_H

#include <memory>

#include <rinvid/core/rectangle_shape.h>
#include <rinvid/gui/widget.h>
#include <rinvid/util/color.h>

namespace rinvid
{

namespace gui
{

/**************************************************************************************************
 * @brief A horizontal value slider.
 *
 *************************************************************************************************/
class Slider : public Widget
{
  public:
    /**************************************************************************************************
     * @brief Slider constructor.
     *
     * @param position Top-left slider position.
     * @param width Slider width.
     * @param min_value Minimum value.
     * @param max_value Maximum value.
     * @param value Initial value.
     *
     *************************************************************************************************/
    Slider(Vector2f position, float width, float min_value = 0.0F, float max_value = 1.0F,
           float value = 0.0F);

    /**************************************************************************************************
     * @brief Sets slider size.
     *
     * @param width Slider width.
     * @param height Slider height.
     *
     *************************************************************************************************/
    void set_size(float width, float height);

    /**************************************************************************************************
     * @brief Returns slider width.
     *
     * @return Slider width.
     *
     *************************************************************************************************/
    float get_width() const;

    /**************************************************************************************************
     * @brief Returns slider height.
     *
     * @return Slider height.
     *
     *************************************************************************************************/
    float get_height() const;

    /**************************************************************************************************
     * @brief Sets slider range.
     *
     * @param min_value Minimum value.
     * @param max_value Maximum value.
     *
     *************************************************************************************************/
    void set_range(float min_value, float max_value);

    /**************************************************************************************************
     * @brief Returns minimum value.
     *
     * @return Minimum value.
     *
     *************************************************************************************************/
    float get_min_value() const;

    /**************************************************************************************************
     * @brief Returns maximum value.
     *
     * @return Maximum value.
     *
     *************************************************************************************************/
    float get_max_value() const;

    /**************************************************************************************************
     * @brief Sets slider value.
     *
     * @param value New value.
     *
     *************************************************************************************************/
    void set_value(float value);

    /**************************************************************************************************
     * @brief Returns slider value.
     *
     * @return Slider value.
     *
     *************************************************************************************************/
    float get_value() const;

    /**************************************************************************************************
     * @brief Sets slider step size.
     *
     * Set to 0.0F for continuous values.
     *
     * @param step Step size.
     *
     *************************************************************************************************/
    void set_step(float step);

    /**************************************************************************************************
     * @brief Returns slider step size.
     *
     * @return Step size.
     *
     *************************************************************************************************/
    float get_step() const;

    /**************************************************************************************************
     * @brief Sets track height.
     *
     * @param height Track height in pixels.
     *
     *************************************************************************************************/
    void set_track_height(float height);

    /**************************************************************************************************
     * @brief Returns track height.
     *
     * @return Track height in pixels.
     *
     *************************************************************************************************/
    float get_track_height() const;

    /**************************************************************************************************
     * @brief Sets thumb size.
     *
     * @param width Thumb width.
     * @param height Thumb height.
     *
     *************************************************************************************************/
    void set_thumb_size(float width, float height);

    /**************************************************************************************************
     * @brief Sets track color.
     *
     * @param color Track color.
     *
     *************************************************************************************************/
    void set_track_color(Color color);

    /**************************************************************************************************
     * @brief Sets fill color.
     *
     * @param color Fill color.
     *
     *************************************************************************************************/
    void set_fill_color(Color color);

    /**************************************************************************************************
     * @brief Sets thumb color.
     *
     * @param color Thumb color.
     *
     *************************************************************************************************/
    void set_thumb_color(Color color);

    /**************************************************************************************************
     * @brief Checks whether the slider is currently being dragged.
     *
     * @return True if the slider is being dragged, false otherwise.
     *
     *************************************************************************************************/
    bool is_dragging() const;

    /**************************************************************************************************
     * @brief Checks whether value changed during the most recent update or setter call.
     *
     * @return True if value changed, false otherwise.
     *
     *************************************************************************************************/
    bool was_value_changed() const;

    /**************************************************************************************************
     * @brief Updates slider input.
     *
     * @param delta_time Time passed in seconds since last frame.
     *
     *************************************************************************************************/
    virtual void update(double delta_time) override;

    /**************************************************************************************************
     * @brief Draws the slider if visible.
     *
     *************************************************************************************************/
    virtual void draw() override;

    /**************************************************************************************************
     * @brief Draws the slider if visible.
     *
     * @param delta_time Time passed in seconds since last frame.
     *
     *************************************************************************************************/
    virtual void draw(double delta_time) override;

    /**************************************************************************************************
     * @brief Moves the slider.
     *
     * @param move_vector Vector to add to current position.
     *
     *************************************************************************************************/
    virtual void move(const Vector2f move_vector) override;

    /**************************************************************************************************
     * @brief Sets slider position.
     *
     * @param position New top-left slider position.
     *
     *************************************************************************************************/
    virtual void set_position(const Vector2f position) override;

    /**************************************************************************************************
     * @brief Returns slider bounds.
     *
     * @return Slider bounds.
     *
     *************************************************************************************************/
    virtual Rect get_bounds() override;

    /**************************************************************************************************
     * @brief Sets whether the slider should be drawn and updated.
     *
     * @param visible True to show the slider, false to hide it.
     *
     *************************************************************************************************/
    virtual void set_visible(bool visible) override;

    /**************************************************************************************************
     * @brief Checks whether the slider is visible.
     *
     * @return True if the slider is visible, false otherwise.
     *
     *************************************************************************************************/
    virtual bool is_visible() const override;

    /**************************************************************************************************
     * @brief Sets whether the slider should react to input.
     *
     * @param enabled True to enable the slider, false to disable it.
     *
     *************************************************************************************************/
    virtual void set_enabled(bool enabled) override;

    /**************************************************************************************************
     * @brief Checks whether the slider reacts to input.
     *
     * @return True if the slider is enabled, false otherwise.
     *
     *************************************************************************************************/
    virtual bool is_enabled() const override;

  private:
    float    clamp_value(float value) const;
    float    get_normalized_value() const;
    float    get_track_width() const;
    float    get_track_left() const;
    float    get_track_center_y() const;
    Vector2f get_mouse_position() const;
    bool     is_mouse_over(Vector2f mouse_position);
    void     set_value_from_position(float position_x);
    void     update_shapes();

    Vector2f                        position_;
    float                           width_;
    float                           height_;
    float                           min_value_;
    float                           max_value_;
    float                           value_;
    float                           step_;
    float                           track_height_;
    float                           thumb_width_;
    float                           thumb_height_;
    Color                           track_color_;
    Color                           fill_color_;
    Color                           thumb_color_;
    std::unique_ptr<RectangleShape> track_;
    std::unique_ptr<RectangleShape> fill_;
    std::unique_ptr<RectangleShape> thumb_;
    bool                            visible_;
    bool                            enabled_;
    bool                            dragging_;
    bool                            mouse_was_down_;
    bool                            was_value_changed_;
};

} // namespace gui

} // namespace rinvid

#endif // INCLUDE_RINVID_GUI_SLIDER_H
