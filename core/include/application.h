/**********************************************************************
 * Copyright (c) 2021 - 2022, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef CORE_INCLUDE_APPLICATION_H
#define CORE_INCLUDE_APPLICATION_H

#include <string>

#include <SFML/Window.hpp>

#include "core/include/screen.h"
#include "util/include/vector2.h"

namespace rinvid
{

class Application
{
  public:
    /**************************************************************************************************
     * @brief Application constructor.
     *
     *************************************************************************************************/
    Application(std::uint32_t width, std::uint32_t height, const std::string& title);

    /**************************************************************************************************
     * @brief Runs the application at selected FPS
     *
     *************************************************************************************************/
    void run();

    /**************************************************************************************************
     * @brief Sets the current screen (state) of the application
     *
     * @param screen Screen to render
     *
     *************************************************************************************************/
    void set_screen(Screen* screen);

    Vector2<float> get_mouse_pos() const;

  private:
    void handle_events(sf::Window& window, sf::Event& event);

    sf::Window    window_;
    Screen*       current_screen_;
    std::uint16_t fps_;
    bool          running_;
};

} // namespace rinvid

#endif // CORE_INCLUDE_APPLICATION_H
