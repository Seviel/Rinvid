/**********************************************************************
 * Copyright (c) 2020, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef CORE_INCLUDE_RINFID_GFX_H
#define CORE_INCLUDE_RINFID_GFX_H

#define GL_GLEXT_PROTOTYPES

#include <cstdint>
#include <SFML/OpenGL.hpp>

namespace rinvid
{

/**************************************************************************************************
 * @brief A class that holds global objects and functions.
 *
 * All functions and members are static.
 *
 *************************************************************************************************/
class RinvidGfx
{
  public:
    /**************************************************************************************************
     * @brief Default constructor
     *
     *************************************************************************************************/
    RinvidGfx()
    {
    }

    /**************************************************************************************************
     * @brief Initializes Rinvid internal state.
     *
     * Should be called once, before any other Rinvid graphics related functions.
     *
     *************************************************************************************************/
    static void init();

    /**************************************************************************************************
     * @brief Sets viewport position and size.
     *
     * Basically a wrapper around glViewport.
     *
     *************************************************************************************************/
    static void set_viewport(std::int32_t x, std::int32_t y, std::int32_t width,
                             std::int32_t heigth);

    /**************************************************************************************************
     * @brief Clears screen
     *
     * @param r red component of the color with which to clear screen
     * @param g green component of the color with which to clear screen
     * @param b blue component of the color with which to clear screen
     * @param a alpha component of the color with which to clear screen
     *
     *************************************************************************************************/
    static void clear_screen(float r, float g, float b, float a);

    /**************************************************************************************************
     * @brief Returns default shader handle. Should only used internally in Rinvid.
     *
     * @return A handle to default shader
     *
     *************************************************************************************************/
    static std::uint32_t get_default_shader();

    /**************************************************************************************************
     * @brief Returns screen width.
     *
     * @return Screen width
     *
     *************************************************************************************************/
    static std::uint32_t get_width();

    /**************************************************************************************************
     * @brief Returns screen height.
     *
     * @return Screen height
     *
     *************************************************************************************************/
    static std::uint32_t get_height();

    /**************************************************************************************************
     * @brief Returns normalized x coordinate. Intended for internal Rinvid use.
     *
     * @param absolute_coord an absolute x axis coordinate in pixels (origin assumed to be in top
     * left corner)
     *
     * @return Normalized x coordinate ([-1, 1] range)
     *
     *************************************************************************************************/
    static float get_opengl_x_coord(float absolute_coord)
    {
        return ((absolute_coord / width) * 2.0F) - 1.0F;
    }

    /**************************************************************************************************
     * @brief Returns normalized y coordinate. Intended for internal Rinvid use.
     *
     * @param absolute_coord an absolute y axis coordinate in pixels (origin assumed to be in top
     * left corner)
     *
     * @return Normalized y coordinate ([-1, 1] range)
     *
     *************************************************************************************************/
    static float get_opengl_y_coord(float absolute_coord)
    {
        return -1.0F * (((absolute_coord / height) * 2.0F) - 1.0F);
    }

  private:
    static std::uint32_t default_shader;
    static std::uint32_t width;
    static std::uint32_t height;
};

} // namespace rinvid

#endif // CORE_INCLUDE_RINFID_GFX_H
