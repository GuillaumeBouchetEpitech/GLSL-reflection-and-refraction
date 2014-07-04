
#version 130

vec4 red = vec4( 1,0,0, 1 );

uniform mat4 u_modelViewProjectionMatrix;
uniform vec3 u_lightDir;

in vec3 a_vertex;
in vec3 a_normal;

out vec4 v_color;

void main(void)
{
  vec3 lightDir = normalize( u_lightDir );

  v_color = red * 0.3 + red * max( dot( a_normal, lightDir ), 0.0 );
  v_color.w = 1;

  gl_Position = u_modelViewProjectionMatrix * vec4( a_vertex, 1.0 );
}
