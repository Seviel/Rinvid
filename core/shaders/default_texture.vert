/**********************************************************************
 * Copyright (c) 2022, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texture_coord;
out vec2 tex_coord;
uniform mat4 model_view_projection;

void main()
{
   gl_Position = model_view_projection * vec4(position.x, position.y, position.z, 1.0);
   tex_coord = vec2(texture_coord.x, texture_coord.y);
}
