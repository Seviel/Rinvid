/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef INCLUDE_RINVID_SYSTEM_INPUT_STATE_H
#define INCLUDE_RINVID_SYSTEM_INPUT_STATE_H

#include <array>

#include <SFML/Window/Event.hpp>

namespace rinvid
{

namespace system
{

/**************************************************************************************************
 * @brief Tracks keyboard and mouse transitions for one application's frame.
 * Application advances this state before updating its screen. Queries do not consume transitions.
 * A press and release in the same frame make both queries true. Focus loss clears all state without
 * generating release transitions, and input events are ignored until focus returns.
 *
 *************************************************************************************************/
class InputState
{
  public:
    /**************************************************************************************************
     * @brief Clears frame transitions while preserving which inputs are down.
     * Call once before processing a new frame's events.
     *
     *************************************************************************************************/
    void begin_frame();

    /**************************************************************************************************
     * @brief Records an event. Repeated presses of an input already down do not create new edges.
     *
     * @param event Window event to process.
     *
     *************************************************************************************************/
    void handle_event(const sf::Event& event);

    /**************************************************************************************************
     * @brief Clears held inputs and transitions without generating release transitions.
     *
     * @param focused Whether input events should be accepted.
     *
     *************************************************************************************************/
    void reset(bool focused = true);

    /**************************************************************************************************
     * @brief Checks whether a key transitioned to pressed during this frame.
     *
     * @param key Key to check.
     *
     * @return True for a press transition, or false for an invalid key.
     *
     *************************************************************************************************/
    bool is_key_just_pressed(sf::Keyboard::Key key) const;

    /**************************************************************************************************
     * @brief Checks whether a key transitioned to released during this frame.
     *
     * @param key Key to check.
     *
     * @return True for a release transition, or false for an invalid key.
     *
     *************************************************************************************************/
    bool is_key_just_released(sf::Keyboard::Key key) const;

    /**************************************************************************************************
     * @brief Checks whether a mouse button transitioned to pressed during this frame.
     *
     * @param button Mouse button to check.
     *
     * @return True for a press transition, or false for an invalid button.
     *
     *************************************************************************************************/
    bool is_button_just_pressed(sf::Mouse::Button button) const;

    /**************************************************************************************************
     * @brief Checks whether a mouse button transitioned to released during this frame.
     *
     * @param button Mouse button to check.
     *
     * @return True for a release transition, or false for an invalid button.
     *
     *************************************************************************************************/
    bool is_button_just_released(sf::Mouse::Button button) const;

  private:
    struct ButtonState
    {
        bool down_{false};
        bool pressed_{false};
        bool released_{false};
    };

    static void set_down(ButtonState& state, bool down);

    std::array<ButtonState, sf::Keyboard::KeyCount> keys_{};
    std::array<ButtonState, sf::Mouse::ButtonCount> buttons_{};
    bool                                            focused_{true};
};

} // namespace system

} // namespace rinvid

#endif // INCLUDE_RINVID_SYSTEM_INPUT_STATE_H
