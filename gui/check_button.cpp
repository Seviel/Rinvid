/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <utility>

#include <rinvid/gui/check_button.h>

namespace rinvid
{

namespace gui
{

CheckButton::CheckButton(std::string text, const std::string& font_path, Color text_color,
                         std::uint32_t text_size)
    : TextButton{std::move(text), font_path, text_color, text_size}, unchecked_regions_{},
      checked_regions_{}, checked_{false}, was_toggled_{false}
{
}

void CheckButton::setup_from_atlas(Texture* texture, std::int32_t width, std::int32_t height,
                                   Vector2f top_left, std::uint32_t state_count,
                                   Vector2f atlas_offset)
{
    TextButton::setup(texture, width, height, top_left);

    std::vector<Rect> unchecked_regions{};
    std::vector<Rect> checked_regions{};
    unchecked_regions.reserve(state_count);
    checked_regions.reserve(state_count);

    for (std::uint32_t index{0U}; index < state_count; ++index)
    {
        Rect region{};
        region.position.x =
            atlas_offset.x + (static_cast<float>(index) * static_cast<float>(width));
        region.position.y = atlas_offset.y;
        region.width      = width;
        region.height     = height;
        unchecked_regions.push_back(region);
    }

    for (std::uint32_t index{0U}; index < state_count; ++index)
    {
        Rect region{};
        region.position.x =
            atlas_offset.x + (static_cast<float>(index + state_count) * static_cast<float>(width));
        region.position.y = atlas_offset.y;
        region.width      = width;
        region.height     = height;
        checked_regions.push_back(region);
    }

    set_check_regions(unchecked_regions, checked_regions);
}

void CheckButton::update()
{
    was_toggled_ = false;
    Button::update();

    if (was_activated())
    {
        set_checked(!checked_);
        was_toggled_ = true;
    }
}

void CheckButton::update(double delta_time)
{
    (void)delta_time;
    update();
}

void CheckButton::set_unchecked_regions(const std::vector<Rect>& regions)
{
    unchecked_regions_ = regions;
    apply_check_regions();
}

void CheckButton::set_checked_regions(const std::vector<Rect>& regions)
{
    checked_regions_ = regions;
    apply_check_regions();
}

void CheckButton::set_check_regions(const std::vector<Rect>& unchecked_regions,
                                    const std::vector<Rect>& checked_regions)
{
    unchecked_regions_ = unchecked_regions;
    checked_regions_   = checked_regions;
    apply_check_regions();
}

void CheckButton::set_checked(bool checked)
{
    if (checked_ == checked)
    {
        return;
    }

    checked_ = checked;
    apply_check_regions();
}

bool CheckButton::is_checked() const
{
    return checked_;
}

bool CheckButton::was_toggled() const
{
    return was_toggled_;
}

void CheckButton::apply_check_regions()
{
    if (checked_ && !checked_regions_.empty())
    {
        set_state_regions(checked_regions_);
        return;
    }

    if (!unchecked_regions_.empty())
    {
        set_state_regions(unchecked_regions_);
    }
}

} // namespace gui

} // namespace rinvid
