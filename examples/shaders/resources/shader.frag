/**********************************************************************
 * Copyright (c) 2024, Filip Vasiljevic
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
   // Makes the texture grayscale
   vec4 alpha = texture(the_texture, tex_coord).aaaa;
   vec3 object_color = texture(the_texture, tex_coord).rgb;
   float grayscale_color = (object_color.r + object_color.g + object_color.b) / 3.0;
   vec4 color = vec4(grayscale_color, grayscale_color, grayscale_color, 1.0);
   out_color = color;
   out_color.a = alpha.a * opacity;
};
