/**********************************************************************
 * Copyright (c) 2022, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#version 330 core
out vec4 out_color; 
uniform float opacity;
uniform sampler2D the_texture;
in vec2 tex_coord;

void main()
{
   vec4 alpha = texture(the_texture, tex_coord).aaaa;
   out_color = texture(the_texture, tex_coord);
   out_color.a = alpha.a * opacity;
}
