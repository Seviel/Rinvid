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

void windows_sleep(std::int64_t microseconds);
{
    double       frequenzy     = 0.0;
    std::int64_t counter_start = 0;

    LARGE_INTEGER counter;
    QueryPerformanceFrequency(&counter) frequenzy = double(counter.QuadPart) / 1000.0;

    QueryPerformanceCounter(&counter);
    counter_start = counter.QuadPart;

    do
    {
        QueryPerformanceCounter(&counter);
    } while (counter.QuadPart < (counter_start + microseconds));
}

#endif // _WIN32

} // namespace rinvid
