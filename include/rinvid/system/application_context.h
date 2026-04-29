/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef INCLUDE_RINVID_SYSTEM_APPLICATION_CONTEXT_H
#define INCLUDE_RINVID_SYSTEM_APPLICATION_CONTEXT_H

#include <rinvid/core/render_context.h>

namespace sf
{

class Window;

} // namespace sf

namespace rinvid
{

class Application;

/**************************************************************************************************
 * @brief Owns runtime services for one application instance.
 *
 *************************************************************************************************/
class ApplicationContext
{
  public:
    /**************************************************************************************************
     * @brief Default constructor.
     *
     *************************************************************************************************/
    ApplicationContext() = default;

    /**************************************************************************************************
     * @brief Copy constructor deleted.
     *
     *************************************************************************************************/
    ApplicationContext(const ApplicationContext& other) = delete;

    /**************************************************************************************************
     * @brief Copy assignment operator deleted.
     *
     *************************************************************************************************/
    ApplicationContext& operator=(const ApplicationContext& other) = delete;

    /**************************************************************************************************
     * @brief Move constructor deleted.
     *
     *************************************************************************************************/
    ApplicationContext(ApplicationContext&& other) = delete;

    /**************************************************************************************************
     * @brief Move assignment operator deleted.
     *
     *************************************************************************************************/
    ApplicationContext& operator=(ApplicationContext&& other) = delete;

    /**************************************************************************************************
     * @brief Destructor.
     *
     *************************************************************************************************/
    ~ApplicationContext() = default;

    /**************************************************************************************************
     * @brief Initializes application services.
     *
     * @param application Application that owns this context.
     * @param window Window owned by application.
     *
     *************************************************************************************************/
    void init(const Application* application, sf::Window& window);

    /**************************************************************************************************
     * @brief Releases application services.
     *
     *************************************************************************************************/
    void shutdown();

    /**************************************************************************************************
     * @brief Returns render context.
     *
     * @return Render context.
     *
     *************************************************************************************************/
    RenderContext& get_render_context();

    /**************************************************************************************************
     * @brief Returns render context.
     *
     * @return Render context.
     *
     *************************************************************************************************/
    const RenderContext& get_render_context() const;

    /**************************************************************************************************
     * @brief Returns application window.
     *
     * @return Application window.
     *
     *************************************************************************************************/
    sf::Window* get_window();

    /**************************************************************************************************
     * @brief Returns application window.
     *
     * @return Application window.
     *
     *************************************************************************************************/
    const sf::Window* get_window() const;

    /**************************************************************************************************
     * @brief Returns currently active application context.
     *
     * @return Active application context.
     *
     *************************************************************************************************/
    static ApplicationContext* get_active_context();

  private:
    static ApplicationContext* active_context_;

    RenderContext render_context_{};
    sf::Window*   window_{nullptr};
};

} // namespace rinvid

#endif // INCLUDE_RINVID_SYSTEM_APPLICATION_CONTEXT_H
