/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef INCLUDE_RINVID_GUI_TEXT_BUTTON_H
#define INCLUDE_RINVID_GUI_TEXT_BUTTON_H

#include <cstdint>
#include <string>

#include <rinvid/gui/button.h>
#include <rinvid/gui/label.h>

namespace rinvid
{

namespace gui
{

/**************************************************************************************************
 * @brief A clickable button with a centered text label.
 *
 *************************************************************************************************/
class TextButton : public Button
{
  public:
    /**************************************************************************************************
     * @brief TextButton constructor.
     *
     * @param text Button label contents.
     * @param font_path Path to font on the filesystem.
     * @param text_color Color of the button label.
     * @param text_size Font size of the button label.
     *
     *************************************************************************************************/
    TextButton(std::string text, const std::string& font_path, Color text_color,
               std::uint32_t text_size);

    /**************************************************************************************************
     * @brief Sets texture, position and size of the button.
     *
     * @param texture Button texture.
     * @param width Button width.
     * @param height Button height.
     * @param top_left Top-left button position.
     * @param texture_offset Top-left texture region offset.
     *
     *************************************************************************************************/
    void setup(Texture* texture, std::int32_t width, std::int32_t height, Vector2f top_left,
               Vector2f texture_offset = {0.0F, 0.0F});

    /**************************************************************************************************
     * @brief Sets texture, position, size and state regions from a horizontal sprite atlas.
     *
     * Regions are expected to be ordered as idle, hovered, pressed and optionally disabled.
     *
     * @param texture Button texture.
     * @param width Button width.
     * @param height Button height.
     * @param top_left Top-left button position.
     * @param state_count Number of state regions in the atlas.
     * @param atlas_offset Top-left offset of the first region in the texture.
     *
     *************************************************************************************************/
    void setup_from_atlas(Texture* texture, std::int32_t width, std::int32_t height,
                          Vector2f top_left, std::uint32_t state_count = 3U,
                          Vector2f atlas_offset = {0.0F, 0.0F});

    /**************************************************************************************************
     * @brief Moves the button and label.
     *
     * @param move_vector Vector to add to the current position.
     *
     *************************************************************************************************/
    virtual void move(const Vector2f move_vector) override;

    /**************************************************************************************************
     * @brief Sets button and label position.
     *
     * @param position New top-left button position.
     *
     *************************************************************************************************/
    virtual void set_position(const Vector2f position) override;

    /**************************************************************************************************
     * @brief Sets whether the button and label should be drawn and updated.
     *
     * @param visible True to show the button, false to hide it.
     *
     *************************************************************************************************/
    void set_visible(bool visible);

    /**************************************************************************************************
     * @brief Sets the button label text.
     *
     * @param text New label text.
     *
     *************************************************************************************************/
    void set_text(const std::string& text);

    /**************************************************************************************************
     * @brief Sets the button label color.
     *
     * @param color New label color.
     *
     *************************************************************************************************/
    void set_text_color(Color color);

    /**************************************************************************************************
     * @brief Sets the button label font size.
     *
     * @param size New label font size.
     *
     *************************************************************************************************/
    void set_text_size(std::uint32_t size);

    /**************************************************************************************************
     * @brief Sets horizontal label padding.
     *
     * @param padding Horizontal padding in pixels.
     *
     *************************************************************************************************/
    void set_label_padding(float padding);

    /**************************************************************************************************
     * @brief Returns the button label.
     *
     * @return Button label.
     *
     *************************************************************************************************/
    Label& get_label();

    /**************************************************************************************************
     * @brief Returns the button label.
     *
     * @return Button label.
     *
     *************************************************************************************************/
    const Label& get_label() const;

    /**************************************************************************************************
     * @brief Draws the button and label if visible.
     *
     *************************************************************************************************/
    virtual void draw() override;

    /**************************************************************************************************
     * @brief Draws the button and label with shader if visible.
     *
     * @param shader Shader to use for the button texture.
     *
     *************************************************************************************************/
    virtual void draw(const Shader shader) override;

    /**************************************************************************************************
     * @brief Draws the animated button and label if visible.
     *
     * @param delta_time Time passed in seconds since last frame.
     *
     *************************************************************************************************/
    virtual void draw(double delta_time) override;

    /**************************************************************************************************
     * @brief Draws the animated button and label with shader if visible.
     *
     * @param delta_time Time passed in seconds since last frame.
     * @param shader Shader to use for the button texture.
     *
     *************************************************************************************************/
    virtual void draw(double delta_time, const Shader shader) override;

  private:
    void update_label_layout();

    Label label_;
    float label_padding_;
};

} // namespace gui

} // namespace rinvid

#endif // INCLUDE_RINVID_GUI_TEXT_BUTTON_H
