/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef INCLUDE_RINVID_UTIL_ERROR_H
#define INCLUDE_RINVID_UTIL_ERROR_H

#include <stdexcept>

namespace rinvid
{

/**************************************************************************************************
 * @brief Base class for runtime errors reported by Rinvid APIs.
 * Required resource acquisition failures are reported by throwing an Error-derived exception.
 * Recoverable operations use explicit return values, and rinvid::errors logging is only diagnostic.
 *
 *************************************************************************************************/
class Error : public std::runtime_error
{
  public:
    using std::runtime_error::runtime_error;
};

/**************************************************************************************************
 * @brief Error thrown when Rinvid cannot load or create a required resource.
 *
 *************************************************************************************************/
class ResourceLoadError : public Error
{
  public:
    using Error::Error;
};

/**************************************************************************************************
 * @brief Error thrown by graphics and rendering APIs.
 *
 *************************************************************************************************/
class GraphicsError : public Error
{
  public:
    using Error::Error;
};

/**************************************************************************************************
 * @brief Error thrown by audio APIs.
 *
 *************************************************************************************************/
class AudioError : public Error
{
  public:
    using Error::Error;
};

} // namespace rinvid

#endif // INCLUDE_RINVID_UTIL_ERROR_H
