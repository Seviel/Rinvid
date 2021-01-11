/**********************************************************************
 * Copyright (c) 2020, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef UTIL_COLOR_H
#define UTIL_COLOR_H

namespace rinvid
{

/**************************************************************************************************
 * @brief Represents color in RGBA mode.
 *
 *************************************************************************************************/
struct Color
{
    /**************************************************************************************************
     * @brief Default constructor.
     *
     *************************************************************************************************/
    Color() : r{}, g{}, b{}, a{}
    {
    }

    /**************************************************************************************************
     * @brief Constructor taking floats. Since OpenGl works with floats with each color element
     * ranging [0.0, 1.0]
     *
     * @param red red element [0.0, 1.0]
     * @param green green element [0.0, 1.0]
     * @param blue blue element [0.0, 1.0]
     * @param alpha alpha element [0.0, 1.0]
     *
     *************************************************************************************************/
    Color(float red, float green, float blue, float alpha) : r{red}, g{green}, b{blue}, a{alpha}
    {
    }

    /**************************************************************************************************
     * @brief Constructor taking 8 bit unsigned ints [0, 255]. This feels like a more natural way of
     * working with color compared to each element being represented with float.
     *
     * @param red red element [0, 255]
     * @param green green element [0, 255]
     * @param blue blue element [0, 255]
     * @param alpha alpha element [0, 255]
     *
     *************************************************************************************************/
    Color(std::uint8_t red, std::uint8_t green, std::uint8_t blue, std::uint8_t alpha)
    {
        r = static_cast<float>(red) / static_cast<float>(UINT8_MAX);
        g = static_cast<float>(green) / static_cast<float>(UINT8_MAX);
        b = static_cast<float>(blue) / static_cast<float>(UINT8_MAX);
        a = static_cast<float>(alpha) / static_cast<float>(UINT8_MAX);
    }

    /**************************************************************************************************
     * @brief Constructor taking 32 bit unsigned ints. Added as a convinience to be able to create
     * Color object without having to cast arguments to uint8. With this constructor one can use
     * unsigned int literal U. Values still must be in [0, 255] range.
     * Example:
     * Color color{100U, 15U, 250U, 255U};
     *
     * @param red red element [0, 255]
     * @param green green element [0, 255]
     * @param blue blue element [0, 255]
     * @param alpha alpha element [0, 255]
     *
     *************************************************************************************************/
    Color(std::uint32_t red, std::uint32_t green, std::uint32_t blue, std::uint32_t alpha)
        : Color(static_cast<std::uint8_t>(red), static_cast<std::uint8_t>(green),
                static_cast<std::uint8_t>(blue), static_cast<std::uint8_t>(alpha))
    {
    }

    /**************************************************************************************************
     * @brief Constructor taking 32 bit ints. Added as a convinience to be able to create Color
     * object without having to cast arguments to uint8. With this constructor one can pass ints
     * without any casting or using any literals. Values still must be in [0, 255] range.
     * Example:
     * Color color{100, 15, 250, 255};
     *
     * @param red red element [0, 255]
     * @param green green element [0, 255]
     * @param blue blue element [0, 255]
     * @param alpha alpha element [0, 255]
     *
     *************************************************************************************************/
    Color(std::int32_t red, std::int32_t green, std::int32_t blue, std::int32_t alpha)
        : Color(static_cast<std::uint8_t>(red), static_cast<std::uint8_t>(green),
                static_cast<std::uint8_t>(blue), static_cast<std::uint8_t>(alpha))
    {
    }

    /**************************************************************************************************
     * @brief Constructor taking one 32 bit int. Added as a convinience as this is another common
     * way people are used to when working with colors.
     * Example:
     * Color color{0xF5DD42FF};
     *
     * @param color a hexadecimal value of RGBA color code
     *
     *************************************************************************************************/
    Color(std::uint32_t color)
    {
        std::uint8_t red   = (0xFF & (color >> 24));
        std::uint8_t green = (0xFF & (color >> 16));
        std::uint8_t blue  = (0xFF & (color >> 8));
        std::uint8_t alpha = (0xFF & color);

        r = static_cast<float>(red) / static_cast<float>(UINT8_MAX);
        g = static_cast<float>(green) / static_cast<float>(UINT8_MAX);
        b = static_cast<float>(blue) / static_cast<float>(UINT8_MAX);
        a = static_cast<float>(alpha) / static_cast<float>(UINT8_MAX);
    }

    float r;
    float g;
    float b;
    float a;
};

} // namespace rinvid

#endif // UTIL_COLOR_H
