/**********************************************************************
 * Copyright (c) 2022 - 2023, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#version 330 core
out vec4 out_color; 
uniform vec4 in_color;
uniform bool use_ambient_light = false;
uniform float ambient_strength = 0.1;

void main()
{
   out_color = in_color;

   if (use_ambient_light)
      out_color *= ambient_strength;
}
