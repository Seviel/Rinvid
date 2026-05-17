/**********************************************************************
 * Copyright (c) 2022 - 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef INCLUDE_RINVID_GUI_BUTTON_H
#define INCLUDE_RINVID_GUI_BUTTON_H

#include <cstdint>
#include <vector>

#include <rinvid/core/sprite.h>
#include <rinvid/gui/widget.h>

namespace rinvid
{

namespace gui
{

/**************************************************************************************************
 * @brief Visual and input state of a button.
 *
 *************************************************************************************************/
enum class ButtonState
{
    Idle = 0U,
    Hovered,
    Pressed,
    Disabled
};

/**************************************************************************************************
 * @brief A clickable button.
 *
 *************************************************************************************************/
class Button : public Sprite, public Widget
{
  public:
    /**************************************************************************************************
     * @brief Button constructor.
     *
     *************************************************************************************************/
    Button();

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
     * @brief Updates the button state and input events based on mouse position.
     *
     *************************************************************************************************/
    void update();

    /**************************************************************************************************
     * @brief Updates the button state and input events based on mouse position.
     *
     * @param delta_time Time passed in seconds since last frame.
     *
     *************************************************************************************************/
    virtual void update(double delta_time) override;

    /**************************************************************************************************
     * @brief Moves the button.
     *
     * @param move_vector Vector to add to current position.
     *
     *************************************************************************************************/
    virtual void move(const Vector2f move_vector) override;

    /**************************************************************************************************
     * @brief Sets button position.
     *
     * @param position New top-left button position.
     *
     *************************************************************************************************/
    virtual void set_position(const Vector2f position) override;

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
    void set_hovered(const std::vector<Rect>& regions);

    /**************************************************************************************************
     * @brief Sets animation regions for when the button is pressed.
     *
     * @param regions Vector of rects indicating frames of texture that make up the animation.
     *
     *************************************************************************************************/
    void set_pressed(const std::vector<Rect>& regions);

    /**************************************************************************************************
     * @brief Sets animation regions for when the button is disabled.
     *
     * @param regions Vector of rects indicating frames of texture that make up the animation.
     *
     *************************************************************************************************/
    void set_disabled(const std::vector<Rect>& regions);

    /**************************************************************************************************
     * @brief Sets state regions from an ordered region list.
     *
     * Regions are expected to be ordered as idle, hovered, pressed and optionally disabled.
     *
     * @param regions Ordered list of state texture regions.
     *
     *************************************************************************************************/
    void set_state_regions(const std::vector<Rect>& regions);

    /**************************************************************************************************
     * @brief Sets whether the button should react to input.
     *
     * @param enabled True to enable the button, false to disable it.
     *
     *************************************************************************************************/
    virtual void set_enabled(bool enabled) override;

    /**************************************************************************************************
     * @brief Checks whether the button reacts to input.
     *
     * @return True if the button is enabled, false otherwise.
     *
     *************************************************************************************************/
    virtual bool is_enabled() const override;

    /**************************************************************************************************
     * @brief Sets whether the button should be drawn and updated.
     *
     * @param visible True to show the button, false to hide it.
     *
     *************************************************************************************************/
    virtual void set_visible(bool visible) override;

    /**************************************************************************************************
     * @brief Checks whether the button is visible.
     *
     * @return True if the button is visible, false otherwise.
     *
     *************************************************************************************************/
    virtual bool is_visible() const override;

    /**************************************************************************************************
     * @brief Returns the current button state.
     *
     * @return Current button state.
     *
     *************************************************************************************************/
    ButtonState get_state() const;

    /**************************************************************************************************
     * @brief Returns button bounds.
     *
     * @return Button bounds.
     *
     *************************************************************************************************/
    virtual Rect get_bounds() override;

    /**************************************************************************************************
     * @brief Checks whether the mouse is hovering over the button.
     *
     * @return True if the button is hovered, false otherwise.
     *
     *************************************************************************************************/
    bool is_hovered() const;

    /**************************************************************************************************
     * @brief Checks whether the button is currently pressed.
     *
     * @return True if the button is pressed, false otherwise.
     *
     *************************************************************************************************/
    bool is_pressed() const;

    /**************************************************************************************************
     * @brief Checks whether the button was pressed during the most recent update.
     *
     * @return True if the button was pressed, false otherwise.
     *
     *************************************************************************************************/
    bool was_pressed() const;

    /**************************************************************************************************
     * @brief Checks whether the button was released during the most recent update.
     *
     * @return True if the button was released, false otherwise.
     *
     *************************************************************************************************/
    bool was_released() const;

    /**************************************************************************************************
     * @brief Checks whether the button was pressed and released while hovered.
     *
     * @return True if the button was activated, false otherwise.
     *
     *************************************************************************************************/
    bool was_activated() const;

    /**************************************************************************************************
     * @brief Draws the button if it is visible.
     *
     *************************************************************************************************/
    virtual void draw() override;

    /**************************************************************************************************
     * @brief Draws the button with shader if it is visible.
     *
     * @param shader Shader to use.
     *
     *************************************************************************************************/
    virtual void draw(const Shader shader) override;

    /**************************************************************************************************
     * @brief Draws the animated button if it is visible.
     *
     * @param delta_time Time passed in seconds since last frame.
     *
     *************************************************************************************************/
    virtual void draw(double delta_time) override;

    /**************************************************************************************************
     * @brief Draws the animated button with shader if it is visible.
     *
     * @param delta_time Time passed in seconds since last frame.
     * @param shader Shader to use.
     *
     *************************************************************************************************/
    virtual void draw(double delta_time, const Shader shader) override;

  private:
    void clear_events();
    bool is_mouse_over();
    void set_state(ButtonState state);
    void apply_state_animation();

    Animation   idle_;
    Animation   hovered_;
    Animation   pressed_;
    Animation   disabled_;
    ButtonState state_;
    bool        visible_;
    bool        enabled_;
    bool        has_disabled_animation_;
    bool        mouse_was_down_;
    bool        pressed_inside_;
    bool        was_pressed_;
    bool        was_released_;
    bool        was_activated_;
};

} // namespace gui

} // namespace rinvid

#endif // INCLUDE_RINVID_GUI_BUTTON_H
