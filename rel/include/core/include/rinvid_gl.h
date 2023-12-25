/**********************************************************************
 * Copyright (c) 2021, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef CORE_INCLUDE_RINVID_GL_H
#define CORE_INCLUDE_RINVID_GL_H

#ifdef __unix__
#define GL_GLEXT_PROTOTYPES
#include <SFML/OpenGL.hpp>
#else
#include "extern/glad/include/glad/glad.h"
#endif

#endif // CORE_INCLUDE_RINVID_GL_H
