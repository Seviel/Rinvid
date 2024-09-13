/**********************************************************************
 * Copyright (c) 2024, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

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
     * @brief Deallocates FT_Library resources. Call when you know you won't be any more renderable
     * text.
     *
     *************************************************************************************************/
    static void destroy();

  private:
    static FT_Library* ft_lib_;
};

} // namespace rinvid
