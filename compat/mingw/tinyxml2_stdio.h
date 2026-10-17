/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef COMPAT_MINGW_TINYXML2_STDIO_H
#define COMPAT_MINGW_TINYXML2_STDIO_H

#include <cstdio>

// MSVCRT does not export the imported _fseeki64/_ftelli64 symbols used by TinyXML2.
#if defined(__MINGW32__) && !defined(_UCRT)
#define _fseeki64 fseek
#define _ftelli64 ftell
#endif

#endif // COMPAT_MINGW_TINYXML2_STDIO_H
