/**********************************************************************
 * Copyright (c) 2024 - 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef CORE_INCLUDE_TTF_LIB_H
#define CORE_INCLUDE_TTF_LIB_H

#include <cstdint>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace rinvid
{

/**************************************************************************************************
 * @brief A singleton wrapper around FT_Library.
 *
 *************************************************************************************************/
class TTFLib
{
  public:
    /**************************************************************************************************
     * @brief Returns an instance of FT_Library.
     *
     * @return FT_Library instance.
     *
     *************************************************************************************************/
    static const FT_Library* get_instance();

    /**************************************************************************************************
     * @brief Releases a previously acquired FT_Library reference.
     *
     *************************************************************************************************/
    static void release();

    /**************************************************************************************************
     * @brief Deallocates FT_Library resources. Call when you know you won't be any more renderable
     * text. If there are still active references, the library will be destroyed automatically once
     * the last reference is released.
     *
     *************************************************************************************************/
    static void destroy();

  private:
    static FT_Library*   ft_lib_;
    static std::uint32_t reference_count_;
};

} // namespace rinvid

#endif // CORE_INCLUDE_TTF_LIB_H
