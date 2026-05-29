/**********************************************************************
 * Copyright (c) 2025 - 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef INCLUDE_RINVID_GUI_LABEL_H
#define INCLUDE_RINVID_GUI_LABEL_H

#include <cstdint>
#include <string>

#include <rinvid/core/text.h>
#include <rinvid/gui/widget.h>

namespace rinvid
{

namespace gui
{

/**************************************************************************************************
 * @brief Horizontal alignment of label text inside its bounds.
 *
 *************************************************************************************************/
enum class LabelAlignment
{
    Left = 0U,
    Center,
    Right
};

/**************************************************************************************************
 * @brief A textual label.
 *
 *************************************************************************************************/
class Label : public Text, public Widget
{
  public:
    /**************************************************************************************************
     * @brief Label constructor.
     *
     * @param text The contents.
     * @param font_path Path to font on the filesystem.
     * @param position Position where to draw the text.
     * @param color Color of the text.
     * @param size Font size.
     * @param bounds_width Optional label bounds width used for wrapping and alignment.
     * @param alignment Horizontal alignment inside the bounds.
     *
     *************************************************************************************************/
    Label(std::string text, const std::string& font_path, Vector2f position, Color color,
          std::uint32_t size, float bounds_width = 0.0F,
          LabelAlignment alignment = LabelAlignment::Left);

    /**************************************************************************************************
     * @brief Updates label state.
     *
     * @param delta_time Time passed in seconds since last frame.
     *
     *************************************************************************************************/
    virtual void update(double delta_time) override;

    /**************************************************************************************************
     * @brief Moves the label.
     *
     * @param move_vector Vector to add to current position.
     *
     *************************************************************************************************/
    virtual void move(const Vector2f move_vector) override;

    /**************************************************************************************************
     * @brief Sets label position.
     *
     * @param position New position.
     *
     *************************************************************************************************/
    virtual void set_position(const Vector2f position) override;

    /**************************************************************************************************
     * @brief Sets horizontal alignment inside label bounds.
     *
     * @param alignment Horizontal alignment.
     *
     *************************************************************************************************/
    void set_alignment(LabelAlignment alignment);

    /**************************************************************************************************
     * @brief Returns horizontal alignment inside label bounds.
     *
     * @return Horizontal alignment.
     *
     *************************************************************************************************/
    LabelAlignment get_alignment() const;

    /**************************************************************************************************
     * @brief Sets label bounds width used for wrapping and alignment.
     *
     * @param bounds_width Label bounds width.
     *
     *************************************************************************************************/
    void set_bounds_width(float bounds_width);

    /**************************************************************************************************
     * @brief Returns label bounds width.
     *
     * @return Label bounds width.
     *
     *************************************************************************************************/
    float get_bounds_width() const;

    /**************************************************************************************************
     * @brief Sets whether the label should be drawn.
     *
     * @param visible True to show the label, false to hide it.
     *
     *************************************************************************************************/
    virtual void set_visible(bool visible) override;

    /**************************************************************************************************
     * @brief Checks whether the label is visible.
     *
     * @return True if the label is visible, false otherwise.
     *
     *************************************************************************************************/
    virtual bool is_visible() const override;

    /**************************************************************************************************
     * @brief Returns label bounds.
     *
     * @return Label bounds.
     *
     *************************************************************************************************/
    virtual Rect get_bounds() override;

    /**************************************************************************************************
     * @brief Draws the label if it is visible.
     *
     *************************************************************************************************/
    virtual void draw() override;

    /**************************************************************************************************
     * @brief Draws the label with shader if it is visible.
     *
     * @param shader Shader to use.
     *
     *************************************************************************************************/
    virtual void draw(const Shader shader) override;

  private:
    Vector2f get_aligned_position() const;

    float          bounds_width_;
    LabelAlignment alignment_;
    bool           visible_;
};

} // namespace gui

} // namespace rinvid

#endif // INCLUDE_RINVID_GUI_LABEL_H
