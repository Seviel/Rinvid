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
uniform float opacity;
uniform sampler2D the_texture;
uniform bool use_ambient_light = false;
uniform float ambient_strength = 0.1;
in vec2 tex_coord;

#define NUMBER_OF_LIGHTS 100 

uniform bool light_active[NUMBER_OF_LIGHTS];
uniform vec2 light_pos[NUMBER_OF_LIGHTS];
uniform float light_constant[NUMBER_OF_LIGHTS];
uniform float light_linear[NUMBER_OF_LIGHTS];
uniform float light_quadratic[NUMBER_OF_LIGHTS];

vec4 apply_light(vec3 object_color, vec3 ambient, int light_number)
{
   vec4 color = vec4(0.0, 0.0, 0.0, 0.0);

   if (light_active[light_number])
   {
      vec2 pixel = gl_FragCoord.xy;		
      vec2 aux = light_pos[light_number] - pixel;
      float dist = length(aux);
      vec3 attenuation = vec3(light_constant[light_number], light_linear[light_number], light_quadratic[light_number]);
      /// @todo Revisit this formula
      float light_attenuation = 1.0 / (attenuation.x + attenuation.y * dist + attenuation.z * dist * dist);
      color = vec4(light_attenuation, light_attenuation, light_attenuation, 1.0) * vec4(ambient * object_color, 1.0);
   }
   else 
   {
      color = vec4(ambient * object_color, 1.0);
   }

   return color;
}

void main()
{
   float used_ambient_strength = ambient_strength;
   if (!use_ambient_light)
      used_ambient_strength = 1.0;

   vec4 alpha = texture(the_texture, tex_coord).aaaa;
   vec3 ambient = vec3(used_ambient_strength, used_ambient_strength, used_ambient_strength);
   vec3 object_color = texture(the_texture, tex_coord).rgb;

   vec4 color = vec4(0.0, 0.0, 0.0, 0.0);
   for (int i = 0; i < NUMBER_OF_LIGHTS; ++i)
   {
      if (light_active[i])
         color += apply_light(object_color, ambient, i);
   }
           
   out_color = color;
   out_color.a = alpha.a * opacity;
}
