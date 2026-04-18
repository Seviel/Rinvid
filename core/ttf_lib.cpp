/**********************************************************************
 * Copyright (c) 2024 - 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include "core/include/ttf_lib.h"

namespace rinvid
{

FT_Library*   TTFLib::ft_lib_ = nullptr;
std::uint32_t TTFLib::reference_count_{0U};

const FT_Library* TTFLib::get_instance()
{
    if (!ft_lib_)
    {
        ft_lib_ = new FT_Library{};
        if (FT_Init_FreeType(ft_lib_))
        {
            delete ft_lib_;
            ft_lib_ = nullptr;
            throw "Freetype: Could not init FreeType Library";
        }
    }

    ++reference_count_;

    return ft_lib_;
}

void TTFLib::release()
{
    if (reference_count_ == 0U)
    {
        return;
    }

    --reference_count_;

    if ((reference_count_ == 0U) && ft_lib_)
    {
        FT_Done_FreeType(*ft_lib_);
        delete ft_lib_;
        ft_lib_ = nullptr;
    }
}

void TTFLib::destroy()
{
    if ((reference_count_ == 0U) && ft_lib_)
    {
        FT_Done_FreeType(*ft_lib_);
        delete ft_lib_;
        ft_lib_ = nullptr;
    }
}

} // namespace rinvid
