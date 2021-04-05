/**********************************************************************
 * Copyright (c) 2021, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef CORE_INCLUDE_ERROR_HANDLER_H
#define CORE_INCLUDE_ERROR_HANDLER_H

#include <string>

/// @brief Uncomment bellow macro if you want to activate Rinvid debug mode. Debug mode allows
/// error messages to be printed in console (and to file if you choose to), but also it causes some
/// performance overhead. Activate debug mode only during debugging and never on "release" builds or
/// when measuring performance, etc.
// #define RINVID_DEBUG_MODE

/// @brief Uncomment bellow macro if you want error messages to be written to file. This will only
/// work if RINVID_DEBUG_MODE is also defined. File rinvid_error.log will be created if you do so.
// #define RINVID_DEBUG_MODE_OUTPUT_TO_FILE

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
 * @brief Prints message to console if RINVID_DEBUG_MODE is defined, also writes it to
 * rinvid_error.log file if RINVID_DEBUG_MODE_OUTPUT_TO_FILE is defined
 *
 * @param error_description Message to be written to console/file
 *
 *************************************************************************************************/
void put_error_to_log(const std::string& error_description);

/**************************************************************************************************
 * @brief Prints message to console if RINVID_DEBUG_MODE is defined, also writes it to
 * rinvid_error.log file if RINVID_DEBUG_MODE_OUTPUT_TO_FILE is defined
 *
 * @param error_description Message to be written to console/file
 *
 *************************************************************************************************/
void put_error_to_log(const char* error_description);

/**************************************************************************************************
 * @brief Handles all OpenGL errors that happened until last call of this function. All discovered
 * errors will be printed on console/file if debug mode is activated.
 *
 * @param file File in which potential error happened
 * @param line Line number on which potential error happened
 *
 *************************************************************************************************/
void handle_gl_errors(const char* file, std::uint32_t line);

} // namespace errors
} // namespace rinvid

#endif // CORE_INCLUDE_ERROR_HANDLER_H
