/**********************************************************************
 * Copyright (c) 2021 - 2025, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef CORE_INCLUDE_APPLICATION_H
#define CORE_INCLUDE_APPLICATION_H

#include <cstdint>
#include <string>

#include <SFML/Window.hpp>

#include "core/include/screen.h"
#include "util/include/vector2.h"

namespace rinvid
{

namespace system
{
class Mouse;
}

class Application
{
  public:
    /**************************************************************************************************
     * @brief Application constructor.
     *
     * @param width The width of the application window
     * @param height The height of the application window
     * @param title Title of the application window
     * @param fullscreen Should the window be fullscreen
     * @param fps Frames per second, pass 0 to have uncapped framerate (default is 60)
     *
     *************************************************************************************************/
    Application(std::uint32_t width, std::uint32_t height, const std::string& title,
                bool fullscreen = false, std::uint16_t fps = 60U);

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

    /**************************************************************************************************
     * @brief Sets the framerate of the application
     *
     * @param fps The framerate to set expressed in frames per second. Pass 0 to have uncapped
     * framerate.
     *
     *************************************************************************************************/
    void set_fps(std::uint16_t fps);

    /**************************************************************************************************
     * @brief Exits the application.
     *
     *************************************************************************************************/
    void exit();

  private:
    friend class rinvid::system::Mouse;
    void handle_events(sf::Window& window, sf::Event& event);

    sf::Window    window_;
    Screen*       current_screen_;
    Screen*       new_screen_;
    std::uint16_t fps_;
    bool          running_;
};

} // namespace rinvid

#endif // CORE_INCLUDE_APPLICATION_H
