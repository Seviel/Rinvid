/**********************************************************************
 * Copyright (c) 2021 - 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef INCLUDE_RINVID_CORE_RINVID_GL_H
#define INCLUDE_RINVID_CORE_RINVID_GL_H

#ifdef __unix__
#define GL_GLEXT_PROTOTYPES
#include <SFML/OpenGL.hpp>
#else
#include <glad/glad.h>
#endif

#endif // INCLUDE_RINVID_CORE_RINVID_GL_H
