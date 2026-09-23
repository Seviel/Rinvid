/**********************************************************************
 * Copyright (c) 2023 - 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <SFML/Window.hpp>

#include <rinvid/system/application_context.h>
#include <rinvid/system/mouse.h>

namespace rinvid
{

namespace system
{

bool Mouse::is_button_pressed(MouseButton button)
{
    sf::Mouse::Button sf_button = sf::Mouse::Button::Left;
    if (button == Right)
    {
        sf_button = sf::Mouse::Button::Right;
    }

    return sf::Mouse::isButtonPressed(sf_button);
}

bool Mouse::is_button_just_pressed(MouseButton button)
{
    const auto* context{ApplicationContext::get_active_context()};
    if (context == nullptr || (button != Left && button != Right))
    {
        return false;
    }

    const auto sf_button{button == Left ? sf::Mouse::Button::Left : sf::Mouse::Button::Right};
    return context->get_input_state().is_button_just_pressed(sf_button);
}

bool Mouse::is_button_just_released(MouseButton button)
{
    const auto* context{ApplicationContext::get_active_context()};
    if (context == nullptr || (button != Left && button != Right))
    {
        return false;
    }

    const auto sf_button{button == Left ? sf::Mouse::Button::Left : sf::Mouse::Button::Right};
    return context->get_input_state().is_button_just_released(sf_button);
}

Vector2f Mouse::get_mouse_pos()
{
    ApplicationContext* context = ApplicationContext::get_active_context();
    if (context == nullptr || context->get_window() == nullptr)
    {
        return Vector2f{};
    }

    sf::Vector2i mouse_position = sf::Mouse::getPosition(*context->get_window());

    return Vector2f{static_cast<float>(mouse_position.x), static_cast<float>(mouse_position.y)};
}

} // namespace system

} // namespace rinvid
