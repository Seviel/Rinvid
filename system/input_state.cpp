/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <cstddef>

#include <rinvid/system/input_state.h>

namespace rinvid
{

namespace system
{

void InputState::begin_frame()
{
    for (auto& key : keys_)
    {
        key.pressed_  = false;
        key.released_ = false;
    }
    for (auto& button : buttons_)
    {
        button.pressed_  = false;
        button.released_ = false;
    }
}

void InputState::handle_event(const sf::Event& event)
{
    if (event.is<sf::Event::FocusLost>())
    {
        reset(false);
    }
    else if (event.is<sf::Event::FocusGained>())
    {
        focused_ = true;
    }
    else if (focused_)
    {
        if (const auto* pressed = event.getIf<sf::Event::KeyPressed>())
        {
            const auto index{static_cast<std::size_t>(pressed->code)};
            if (index < keys_.size())
            {
                set_down(keys_[index], true);
            }
        }
        else if (const auto* released = event.getIf<sf::Event::KeyReleased>())
        {
            const auto index{static_cast<std::size_t>(released->code)};
            if (index < keys_.size())
            {
                set_down(keys_[index], false);
            }
        }
        else if (const auto* pressed = event.getIf<sf::Event::MouseButtonPressed>())
        {
            const auto index{static_cast<std::size_t>(pressed->button)};
            if (index < buttons_.size())
            {
                set_down(buttons_[index], true);
            }
        }
        else if (const auto* released = event.getIf<sf::Event::MouseButtonReleased>())
        {
            const auto index{static_cast<std::size_t>(released->button)};
            if (index < buttons_.size())
            {
                set_down(buttons_[index], false);
            }
        }
    }
}

void InputState::reset(bool focused)
{
    keys_    = {};
    buttons_ = {};
    focused_ = focused;
}

bool InputState::is_key_just_pressed(sf::Keyboard::Key key) const
{
    const auto index{static_cast<std::size_t>(key)};
    return index < keys_.size() && keys_[index].pressed_;
}

bool InputState::is_key_just_released(sf::Keyboard::Key key) const
{
    const auto index{static_cast<std::size_t>(key)};
    return index < keys_.size() && keys_[index].released_;
}

bool InputState::is_button_just_pressed(sf::Mouse::Button button) const
{
    const auto index{static_cast<std::size_t>(button)};
    return index < buttons_.size() && buttons_[index].pressed_;
}

bool InputState::is_button_just_released(sf::Mouse::Button button) const
{
    const auto index{static_cast<std::size_t>(button)};
    return index < buttons_.size() && buttons_[index].released_;
}

void InputState::set_down(ButtonState& state, bool down)
{
    if (state.down_ != down)
    {
        state.down_ = down;
        if (down)
        {
            state.pressed_ = true;
        }
        else
        {
            state.released_ = true;
        }
    }
}

} // namespace system

} // namespace rinvid
