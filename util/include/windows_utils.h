/**********************************************************************
 * Copyright (c) 2021, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef UTIL_WINDOWS_UTILS_H
#define UTIL_WINDOWS_UTILS_H

#ifdef _WIN32

namespace rinvid
{

/**************************************************************************************************
 * @brief A more accurate implementation of sleep for Windows OS
 *
 * @param microseconds time this thread should sleep
 *
 *************************************************************************************************/
void windows_sleep(std::uint64_t microseconds);

} // namespace rinvid

#endif // _WIN32

#endif // UTIL_WINDOWS_UTILS_H
