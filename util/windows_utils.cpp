/**********************************************************************
 * Copyright (c) 2021, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifdef _WIN32

#include <windows.h>

#include "util/include/windows_utils.h"

namespace rinvid
{

void windows_sleep(std::int64_t microseconds)
{
    double       frequenzy     = 0.0;
    std::int64_t counter_start = 0;

    LARGE_INTEGER freq;
    LARGE_INTEGER counter;
    QueryPerformanceFrequency(&freq);
    frequenzy = 1000000.0 / static_cast<double>(freq.QuadPart);
    QueryPerformanceCounter(&counter);
    counter_start = counter.QuadPart;
    do
    {
        QueryPerformanceCounter(&counter);
    } while ((counter.QuadPart * frequenzy) < ((counter_start * frequenzy) + microseconds));
}

} // namespace rinvid

#endif // _WIN32
