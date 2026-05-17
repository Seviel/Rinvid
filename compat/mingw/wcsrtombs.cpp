/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include "wcsrtombs.h"

#include <cstdlib>

extern "C" std::size_t rinvid_mingw_wcsrtombs(char* destination, const wchar_t** source,
                                              std::size_t count, std::mbstate_t*)
{
    if (source == nullptr || *source == nullptr)
    {
        return 0;
    }

    const auto converted = std::wcstombs(destination, *source, count);

    if (converted != static_cast<std::size_t>(-1) && destination != nullptr)
    {
        *source += converted;
        if (converted < count)
        {
            *source = nullptr;
        }
    }

    return converted;
}
