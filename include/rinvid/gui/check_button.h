/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef INCLUDE_RINVID_GUI_CHECK_BUTTON_H
#define INCLUDE_RINVID_GUI_CHECK_BUTTON_H

#include <cstdint>
#include <string>
#include <vector>

#include <rinvid/gui/text_button.h>

namespace rinvid
{

namespace gui
{

/**************************************************************************************************
 * @brief A toggleable text button with checked and unchecked states.
 *
 *************************************************************************************************/
class CheckButton : public TextButton
{
  public:
    /**************************************************************************************************
     * @brief CheckButton constructor.
     *
     * @param text Button label contents.
     * @param font_path Path to font on the filesystem.
     * @param text_color Color of the button label.
     * @param text_size Font size of the button label.
     *
     *************************************************************************************************/
    CheckButton(std::string text, const std::string& font_path, Color text_color,
                std::uint32_t text_size);

    /**************************************************************************************************
     * @brief Sets texture, position, size and checked state regions from a horizontal atlas.
     *
     * Regions are expected to be ordered as unchecked states followed by checked states. Each state
     * group is ordered as idle, hovered, pressed and optionally disabled.
     *
     * @param texture Button texture.
     * @param width Button width.
     * @param height Button height.
     * @param top_left Top-left button position.
     * @param state_count Number of states in each checked/unchecked atlas group.
     * @param atlas_offset Top-left offset of the first region in the texture.
     *
     *************************************************************************************************/
    void setup_from_atlas(Texture* texture, std::int32_t width, std::int32_t height,
                          Vector2f top_left, std::uint32_t state_count = 3U,
                          Vector2f atlas_offset = {0.0F, 0.0F});

    /**************************************************************************************************
     * @brief Updates the button and toggles checked state when activated.
     *
     *************************************************************************************************/
    void update();

    /**************************************************************************************************
     * @brief Updates the button and toggles checked state when activated.
     *
     * @param delta_time Time passed in seconds since last frame.
     *
     *************************************************************************************************/
    virtual void update(double delta_time) override;

    /**************************************************************************************************
     * @brief Sets animation regions for the unchecked state.
     *
     * @param regions Ordered list of button state texture regions.
     *
     *************************************************************************************************/
    void set_unchecked_regions(const std::vector<Rect>& regions);

    /**************************************************************************************************
     * @brief Sets animation regions for the checked state.
     *
     * @param regions Ordered list of button state texture regions.
     *
     *************************************************************************************************/
    void set_checked_regions(const std::vector<Rect>& regions);

    /**************************************************************************************************
     * @brief Sets animation regions for both unchecked and checked states.
     *
     * @param unchecked_regions Ordered list of unchecked texture regions.
     * @param checked_regions Ordered list of checked texture regions.
     *
     *************************************************************************************************/
    void set_check_regions(const std::vector<Rect>& unchecked_regions,
                           const std::vector<Rect>& checked_regions);

    /**************************************************************************************************
     * @brief Sets whether the button is checked.
     *
     * @param checked True to check the button, false to uncheck it.
     *
     *************************************************************************************************/
    void set_checked(bool checked);

    /**************************************************************************************************
     * @brief Checks whether the button is checked.
     *
     * @return True if the button is checked, false otherwise.
     *
     *************************************************************************************************/
    bool is_checked() const;

    /**************************************************************************************************
     * @brief Checks whether the button toggled during the most recent update.
     *
     * @return True if checked state changed during the most recent update, false otherwise.
     *
     *************************************************************************************************/
    bool was_toggled() const;

  private:
    void apply_check_regions();

    std::vector<Rect> unchecked_regions_;
    std::vector<Rect> checked_regions_;
    bool              checked_;
    bool              was_toggled_;
};

} // namespace gui

} // namespace rinvid

#endif // INCLUDE_RINVID_GUI_CHECK_BUTTON_H
