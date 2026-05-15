/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef RINVID_COMPAT_MINGW_WCSRTOMBS_H
#define RINVID_COMPAT_MINGW_WCSRTOMBS_H

#pragma push_macro("wcsrtombs")
#undef wcsrtombs

#include <cstddef>
#include <cwchar>

#pragma pop_macro("wcsrtombs")

extern "C" std::size_t rinvid_mingw_wcsrtombs(char* destination, const wchar_t** source,
                                              std::size_t count, std::mbstate_t* state);

#endif // RINVID_COMPAT_MINGW_WCSRTOMBS_H
