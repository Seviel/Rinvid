/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef INCLUDE_RINVID_GUI_PROGRESS_BAR_H
#define INCLUDE_RINVID_GUI_PROGRESS_BAR_H

#include <memory>

#include <rinvid/core/rectangle_shape.h>
#include <rinvid/gui/widget.h>
#include <rinvid/util/color.h>

namespace rinvid
{

namespace gui
{

/**************************************************************************************************
 * @brief A horizontal progress bar.
 *
 *************************************************************************************************/
class ProgressBar : public Widget
{
  public:
    /**************************************************************************************************
     * @brief ProgressBar constructor.
     *
     * @param position Top-left progress bar position.
     * @param width Progress bar width.
     * @param height Progress bar height.
     * @param min_value Minimum value.
     * @param max_value Maximum value.
     * @param value Initial value.
     *
     *************************************************************************************************/
    ProgressBar(Vector2f position, float width, float height = 16.0F, float min_value = 0.0F,
                float max_value = 1.0F, float value = 0.0F);

    /**************************************************************************************************
     * @brief Sets progress bar size.
     *
     * @param width Progress bar width.
     * @param height Progress bar height.
     *
     *************************************************************************************************/
    void set_size(float width, float height);

    /**************************************************************************************************
     * @brief Returns progress bar width.
     *
     * @return Progress bar width.
     *
     *************************************************************************************************/
    float get_width() const;

    /**************************************************************************************************
     * @brief Returns progress bar height.
     *
     * @return Progress bar height.
     *
     *************************************************************************************************/
    float get_height() const;

    /**************************************************************************************************
     * @brief Sets progress bar range.
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
     * @brief Sets progress bar value.
     *
     * @param value New value.
     *
     *************************************************************************************************/
    void set_value(float value);

    /**************************************************************************************************
     * @brief Returns progress bar value.
     *
     * @return Progress bar value.
     *
     *************************************************************************************************/
    float get_value() const;

    /**************************************************************************************************
     * @brief Sets normalized progress in [0.0, 1.0].
     *
     * @param progress New normalized progress.
     *
     *************************************************************************************************/
    void set_progress(float progress);

    /**************************************************************************************************
     * @brief Returns normalized progress in [0.0, 1.0].
     *
     * @return Normalized progress.
     *
     *************************************************************************************************/
    float get_progress() const;

    /**************************************************************************************************
     * @brief Sets background color.
     *
     * @param color Background color.
     *
     *************************************************************************************************/
    void set_background_color(Color color);

    /**************************************************************************************************
     * @brief Sets fill color.
     *
     * @param color Fill color.
     *
     *************************************************************************************************/
    void set_fill_color(Color color);

    /**************************************************************************************************
     * @brief Sets border color.
     *
     * @param color Border color.
     *
     *************************************************************************************************/
    void set_border_color(Color color);

    /**************************************************************************************************
     * @brief Sets border thickness.
     *
     * @param thickness Border thickness in pixels.
     *
     *************************************************************************************************/
    void set_border_thickness(float thickness);

    /**************************************************************************************************
     * @brief Returns border thickness.
     *
     * @return Border thickness in pixels.
     *
     *************************************************************************************************/
    float get_border_thickness() const;

    /**************************************************************************************************
     * @brief Draws the progress bar if visible.
     *
     *************************************************************************************************/
    virtual void draw() override;

    /**************************************************************************************************
     * @brief Draws the progress bar if visible.
     *
     * @param delta_time Time passed in seconds since last frame.
     *
     *************************************************************************************************/
    virtual void draw(double delta_time) override;

    /**************************************************************************************************
     * @brief Moves the progress bar.
     *
     * @param move_vector Vector to add to current position.
     *
     *************************************************************************************************/
    virtual void move(const Vector2f move_vector) override;

    /**************************************************************************************************
     * @brief Sets progress bar position.
     *
     * @param position New top-left progress bar position.
     *
     *************************************************************************************************/
    virtual void set_position(const Vector2f position) override;

    /**************************************************************************************************
     * @brief Returns progress bar bounds.
     *
     * @return Progress bar bounds.
     *
     *************************************************************************************************/
    virtual Rect get_bounds() override;

    /**************************************************************************************************
     * @brief Sets whether the progress bar should be drawn.
     *
     * @param visible True to show the progress bar, false to hide it.
     *
     *************************************************************************************************/
    virtual void set_visible(bool visible) override;

    /**************************************************************************************************
     * @brief Checks whether the progress bar is visible.
     *
     * @return True if the progress bar is visible, false otherwise.
     *
     *************************************************************************************************/
    virtual bool is_visible() const override;

    /**************************************************************************************************
     * @brief Sets whether the progress bar is enabled.
     *
     * @param enabled True to enable the progress bar, false to disable it.
     *
     *************************************************************************************************/
    virtual void set_enabled(bool enabled) override;

    /**************************************************************************************************
     * @brief Checks whether the progress bar is enabled.
     *
     * @return True if the progress bar is enabled, false otherwise.
     *
     *************************************************************************************************/
    virtual bool is_enabled() const override;

  private:
    float clamp_value(float value) const;
    void  update_shapes();

    Vector2f                        position_;
    float                           width_;
    float                           height_;
    float                           min_value_;
    float                           max_value_;
    float                           value_;
    float                           border_thickness_;
    Color                           background_color_;
    Color                           fill_color_;
    Color                           border_color_;
    std::unique_ptr<RectangleShape> background_;
    std::unique_ptr<RectangleShape> fill_;
    std::unique_ptr<RectangleShape> border_;
    bool                            visible_;
    bool                            enabled_;
};

} // namespace gui

} // namespace rinvid

#endif // INCLUDE_RINVID_GUI_PROGRESS_BAR_H
