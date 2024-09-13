/**********************************************************************
 * Copyright (c) 2024, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include "core/include/ttf_lib.h"

namespace rinvid
{

FT_Library* TTFLib::ft_lib_ = nullptr;

const FT_Library* TTFLib::get_instance()
{
    if (!ft_lib_)
    {
        ft_lib_ = new FT_Library{};
        if (FT_Init_FreeType(ft_lib_))
        {
            throw "Freetype: Could not init FreeType Library";
        }
    }

    return ft_lib_;
}

void TTFLib::destroy()
{
    if (ft_lib_)
    {
        FT_Done_FreeType(*ft_lib_);
        delete ft_lib_;
        ft_lib_ = nullptr;
    }
}

} // namespace rinvid
