/**********************************************************************
 * Copyright (c) 2021 - 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef INCLUDE_RINVID_UTIL_ERROR_HANDLER_H
#define INCLUDE_RINVID_UTIL_ERROR_HANDLER_H

#include <cstdint>
#include <string>

/// @brief RINVID_DEBUG_MODE and RINVID_DEBUG_MODE_OUTPUT_TO_FILE are configured through CMake.
/// Diagnostic messages are always recorded for inspection through this API. RINVID_DEBUG_MODE also
/// prints unique messages to stderr and enables GL_CALL OpenGL error checks. If
/// RINVID_DEBUG_MODE_OUTPUT_TO_FILE is also enabled, messages are appended to rinvid_error.log.

#ifdef RINVID_DEBUG_MODE
/// @brief This is a common way to deal with OpenGL error handling. Instead of calling glGetError()
/// being called after every GL call and then proccessed, every GL call is instead wrapped in a
/// macro. This reduces code size and makes it more readable. A potentially better way to handle
/// errors in OpenGL is by registering a callback via MessageCallback(), but that feature is
/// available only from OpenGL 4.3+, so bellow way offer better compatibility.
/// Additional notice: do{..}while(false) pattern used here ensures that the semicolon is required
/// at the end, and always means the same thing.
#define GL_CALL(gl_function_call)                                                                  \
    do                                                                                             \
    {                                                                                              \
        gl_function_call;                                                                          \
        rinvid::errors::handle_gl_errors(__FILE__, __LINE__);                                      \
    } while (false)
#else
#define GL_CALL(gl_function_call) (gl_function_call)
#endif

namespace rinvid
{
namespace errors
{

/**************************************************************************************************
 * @brief Records a diagnostic message. If RINVID_DEBUG_MODE is defined, unique messages are also
 * printed to stderr. If RINVID_DEBUG_MODE_OUTPUT_TO_FILE is defined as well, unique messages are
 * also written to rinvid_error.log.
 *
 * @param error_description Message to be written to console/file
 *
 *************************************************************************************************/
void put_error_to_log(const std::string& error_description);

/**************************************************************************************************
 * @brief Records a diagnostic message. If RINVID_DEBUG_MODE is defined, unique messages are also
 * printed to stderr. If RINVID_DEBUG_MODE_OUTPUT_TO_FILE is defined as well, unique messages are
 * also written to rinvid_error.log.
 *
 * @param error_description Message to be written to console/file
 *
 *************************************************************************************************/
void put_error_to_log(const char* error_description);

/**************************************************************************************************
 * @brief Handles all OpenGL errors that happened until last call of this function. All discovered
 * errors will be recorded and printed on console/file if debug mode is activated.
 *
 * @param file File in which potential error happened
 * @param line Line number on which potential error happened
 *
 *************************************************************************************************/
void handle_gl_errors(const char* file, std::uint32_t line);

/**************************************************************************************************
 * @brief Returns number of unique recorded errors.
 *
 * @return Number of errors that occured up to this point in time.
 *
 *************************************************************************************************/
std::uint32_t get_error_count();

/**************************************************************************************************
 * @brief Clears all recorded diagnostic errors.
 *
 *************************************************************************************************/
void clear_errors();

/**************************************************************************************************
 * @brief Check whether specific error occurred.
 *
 * @param description Description of the error.
 *
 * @return True if error with this description has occurred.
 *
 *************************************************************************************************/
bool has_error_occurred(const std::string& description);

} // namespace errors
} // namespace rinvid

#endif // INCLUDE_RINVID_UTIL_ERROR_HANDLER_H
