/**********************************************************************
 * Copyright (c) 2021 - 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <fstream>
#include <iostream>
#include <mutex>
#include <set>
#include <string>

#ifdef __unix__
#define GL_GLEXT_PROTOTYPES
#include <SFML/OpenGL.hpp>
#else
#include <glad/glad.h>
#endif

#include <rinvid/util/error_handler.h>

namespace rinvid
{
namespace errors
{

static std::set<std::string> errors;
static std::mutex            errors_mutex;

void put_error_to_log(const std::string& error_description)
{
    std::lock_guard<std::mutex> lock{errors_mutex};
    const auto                  result = errors.insert(error_description);
    if (result.second == false)
    {
        return;
    }

#ifdef RINVID_DEBUG_MODE
    std::cerr << error_description << '\n';
#endif

#if defined(RINVID_DEBUG_MODE) && defined(RINVID_DEBUG_MODE_OUTPUT_TO_FILE)
    std::ofstream error_log;
    error_log.open("rinvid_error.log", std::ios_base::app);
    error_log << error_description << '\n';
    error_log.close();
#endif
}

void put_error_to_log(const char* error_description)
{
    if (error_description == nullptr)
    {
        put_error_to_log(std::string{"Rinvid error: null error description"});
        return;
    }

    put_error_to_log(std::string{error_description});
}

void handle_gl_errors(const char* file, std::uint32_t line)
{
#ifndef RINVID_DEBUG_MODE
    return;
#endif

    GLenum      error_code;
    std::string error;

    while ((error_code = glGetError()))
    {
        switch (error_code)
        {
            case GL_INVALID_ENUM:
                error = "GL_INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                error = "GL_INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION:
                error = "GL_INVALID_OPERATION";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                error = "GL_INVALID_FRAMEBUFFER_OPERATION";
                break;
            case GL_OUT_OF_MEMORY:
                error = "GL_OUT_OF_MEMORY";
                break;
            case GL_STACK_UNDERFLOW:
                error = "GL_STACK_UNDERFLOW";
                break;
            case GL_STACK_OVERFLOW:
                error = "GL_STACK_OVERFLOW";
                break;
            default:
                break;
        }

        std::string error_description = "An OpenGL error: " + error + " occured in file: " + file +
                                        ", on line: " + std::to_string(line);

        put_error_to_log(error_description);
    }
}

std::uint32_t get_error_count()
{
    std::lock_guard<std::mutex> lock{errors_mutex};
    return static_cast<std::uint32_t>(errors.size());
}

void clear_errors()
{
    std::lock_guard<std::mutex> lock{errors_mutex};
    errors.clear();
}

bool has_error_occurred(const std::string& description)
{
    std::lock_guard<std::mutex> lock{errors_mutex};
    return errors.find(description) != errors.end();
}

} // namespace errors
} // namespace rinvid
