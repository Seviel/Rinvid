/**********************************************************************
 * Copyright (c) 2021, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef CORE_INCLUDE_SCREEN_H
#define CORE_INCLUDE_SCREEN_H

namespace rinvid
{

class Application;

class Screen
{
  public:
    /**************************************************************************************************
     * @brief Returns the application that currently renders this screen.
     *
     * @return Pointer to application
     *
     *************************************************************************************************/
    Application* get_application()
    {
        return application_;
    }

    /**************************************************************************************************
     * @brief Initializes screen. Should be called right after constructing the object.
     *
     *************************************************************************************************/
    virtual void create() = 0;

    /**************************************************************************************************
     * @brief Cleans up screen resources. Should be called after the screen is no longer needed.
     *
     *************************************************************************************************/
    virtual void destroy() = 0;

  private:
    friend class Application;
    void set_application(Application* application)
    {
        application_ = application;
    }

    virtual void update(double delta_time) = 0;

    Application* application_;
};

} // namespace rinvid

#endif // CORE_INCLUDE_SCREEN_H
