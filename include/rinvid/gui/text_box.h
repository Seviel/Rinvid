/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef INCLUDE_RINVID_GUI_TEXT_BOX_H
#define INCLUDE_RINVID_GUI_TEXT_BOX_H

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>

#include <SFML/Window/Keyboard.hpp>

#include <rinvid/core/rectangle_shape.h>
#include <rinvid/core/text.h>
#include <rinvid/gui/panel.h>
#include <rinvid/gui/widget.h>

namespace rinvid
{

namespace gui
{

/**************************************************************************************************
 * @brief A single-line editable textbox drawn with Rinvid rectangle shapes.
 * Supports printable ASCII, matching Text's glyph set. Click to focus and position the caret;
 * Left/Right, Home/End, Backspace and Delete edit the contents. Enter submits and Escape unfocuses.
 * Text scrolls horizontally and is clipped to the interior. Update once per application frame.
 * Like other GUI controls, use an axis-aligned view for layout and drawing.
 *
 *************************************************************************************************/
class TextBox : public Widget
{
  public:
    /**************************************************************************************************
     * @brief Constructs a textbox. Requires an active render context.
     *
     * @param text Initial contents; characters outside printable ASCII are discarded.
     * @param font_path Path to the font on the filesystem.
     * @param position Top-left position.
     * @param width Outer width, clamped to zero or greater.
     * @param height Outer height, clamped to zero or greater.
     * @param text_color Text and caret color.
     * @param text_size Font size.
     *
     * @throws ResourceLoadError if the font cannot be loaded.
     *
     *************************************************************************************************/
    TextBox(std::string text, const std::string& font_path, Vector2f position, float width,
            float height, Color text_color, std::uint32_t text_size);

    /**************************************************************************************************
     * @brief Sets contents, discarding characters outside printable ASCII, and moves caret to end.
     * Sets was_text_changed() if the contents change.
     *
     * @param text New contents.
     *
     *************************************************************************************************/
    void set_text(const std::string& text);

    /**************************************************************************************************
     * @brief Returns the complete contents, including text outside the visible area.
     *
     * @return Text contents.
     *
     *************************************************************************************************/
    const std::string& get_text() const;

    /**************************************************************************************************
     * @brief Sets the text and caret color.
     *
     * @param color New color.
     *
     *************************************************************************************************/
    void set_text_color(Color color);

    /**************************************************************************************************
     * @brief Sets the font size and updates the caret and scrolling.
     *
     * @param size New font size.
     *
     *************************************************************************************************/
    void set_text_size(std::uint32_t size);

    /**************************************************************************************************
     * @brief Sets the outer dimensions, clamping negative dimensions to zero.
     *
     * @param width Outer width.
     * @param height Outer height.
     *
     *************************************************************************************************/
    void set_size(float width, float height);

    /**************************************************************************************************
     * @brief Sets the textbox background color.
     *
     * @param color Background color.
     *
     *************************************************************************************************/
    void set_background_color(Color color);

    /**************************************************************************************************
     * @brief Sets the unfocused border color.
     *
     * @param color Border color.
     *
     *************************************************************************************************/
    void set_border_color(Color color);

    /**************************************************************************************************
     * @brief Sets the focused border color.
     *
     * @param color Focused border color.
     *
     *************************************************************************************************/
    void set_focused_border_color(Color color);

    /**************************************************************************************************
     * @brief Sets border thickness, clamped to zero or greater and drawn inside the bounds.
     *
     * @param thickness Border thickness.
     *
     *************************************************************************************************/
    void set_border_thickness(float thickness);

    /**************************************************************************************************
     * @brief Sets spacing between the border and contents, clamped to zero or greater.
     *
     * @param padding Interior padding.
     *
     *************************************************************************************************/
    void set_padding(float padding);

    /**************************************************************************************************
     * @brief Sets keyboard focus. Hidden or disabled textboxes cannot receive focus.
     * Callers assigning focus programmatically must unfocus any other textbox first.
     *
     * @param focused Whether to focus this textbox.
     *
     *************************************************************************************************/
    void set_focused(bool focused);

    /**************************************************************************************************
     * @brief Checks whether the textbox has keyboard focus.
     *
     * @return True if focused.
     *
     *************************************************************************************************/
    bool is_focused() const;

    /**************************************************************************************************
     * @brief Checks whether text changed since the start of the latest update.
     *
     * @return True if text changed. Cleared at the start of each update.
     *
     *************************************************************************************************/
    bool was_text_changed() const;

    /**************************************************************************************************
     * @brief Checks whether Enter was pressed while focused during the latest update.
     *
     * @return True if submitted. Cleared at the start of each update.
     *
     *************************************************************************************************/
    bool was_submitted() const;

    /**************************************************************************************************
     * @brief Processes application input and advances the blinking caret.
     *
     * @param delta_time Time passed in seconds since last frame.
     *
     *************************************************************************************************/
    virtual void update(double delta_time) override;

    /**************************************************************************************************
     * @brief Draws the textbox if visible.
     *
     *************************************************************************************************/
    virtual void draw() override;

    /**************************************************************************************************
     * @brief Draws the textbox if visible. Does not process input or advance the caret.
     *
     * @param delta_time Time passed in seconds since last frame.
     *
     *************************************************************************************************/
    virtual void draw(double delta_time) override;

    /**************************************************************************************************
     * @brief Moves the textbox.
     *
     * @param move_vector Offset to add to the current position.
     *
     *************************************************************************************************/
    virtual void move(const Vector2f move_vector) override;

    /**************************************************************************************************
     * @brief Sets the textbox position.
     *
     * @param position New top-left position.
     *
     *************************************************************************************************/
    virtual void set_position(const Vector2f position) override;

    /**************************************************************************************************
     * @brief Returns the outer textbox bounds, including the border.
     *
     * @return Textbox bounds.
     *
     *************************************************************************************************/
    virtual Rect get_bounds() override;

    /**************************************************************************************************
     * @brief Sets visibility. Hiding the textbox removes focus.
     *
     * @param visible Whether to draw and update the textbox.
     *
     *************************************************************************************************/
    virtual void set_visible(bool visible) override;

    /**************************************************************************************************
     * @brief Checks visibility.
     *
     * @return True if visible.
     *
     *************************************************************************************************/
    virtual bool is_visible() const override;

    /**************************************************************************************************
     * @brief Enables or disables input. Disabling the textbox removes focus.
     *
     * @param enabled Whether to accept input.
     *
     *************************************************************************************************/
    virtual void set_enabled(bool enabled) override;

    /**************************************************************************************************
     * @brief Checks whether input is enabled.
     *
     * @return True if enabled.
     *
     *************************************************************************************************/
    virtual bool is_enabled() const override;

  private:
    float get_inset() const;
    float measure_prefix(std::size_t length);
    void  update_layout();
    void  place_caret(float position_x);
    void  handle_key(sf::Keyboard::Key key);

    Panel                           background_;
    Text                            text_graphic_;
    std::unique_ptr<RectangleShape> caret_;
    std::string                     text_;
    Vector2f                        position_;
    Color                           border_color_{0x5D7390FF};
    Color                           focused_border_color_{0x4AA3FFFF};
    float                           padding_{6.0F};
    float                           scroll_offset_{0.0F};
    std::size_t                     caret_position_{0U};
    double                          blink_time_{0.0};
    bool                            focused_{false};
    bool                            was_text_changed_{false};
    bool                            was_submitted_{false};
};

} // namespace gui

} // namespace rinvid

#endif // INCLUDE_RINVID_GUI_TEXT_BOX_H
