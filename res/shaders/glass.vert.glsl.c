
#version 130



// Indices of refraction
const float Air = 1.0;
const float Glass = 1.51714;

// Air to glass ratio of the indices of refraction (Eta)
const float Eta = Air / Glass;

// see http://en.wikipedia.org/wiki/Refractive_index Reflectivity
const float R0 = ((Air - Glass) * (Air - Glass)) / ((Air + Glass) * (Air + Glass));



uniform mat4 u_viewProjectionMatrix;
uniform mat4 u_modelMatrix;
/* uniform mat4 u_modelMatrix_vertex; */
uniform mat3 u_normalMatrix;
uniform vec3 u_camera;

in vec3 a_vertex;
in vec3 a_normal;

out vec3 v_reflection;
out vec3 v_refraction;
out float v_fresnel;


void main(void)
{
  // We calculate in world space.

  vec4 vertex = u_modelMatrix * vec4( a_vertex, 1.0 );
  /* vec4 camera = u_modelMatrix * vec4( u_camera, 1.0 ); */
  vec4 camera = vec4( u_camera, 1.0 );

  /* vec4 camera = u_camera; */

  /* vec3 incident = normalize( vec3( vertex - u_camera ) ); */
  /* vec3 incident = normalize( vec3( vertex - camera ) ); */
  vec3 incident = normalize( vec3( vertex - camera ) );

  // Assume incoming normal is normalized.

  vec3 normal = u_normalMatrix * a_normal;
  /* vec3 normal = a_normal; */

  /* vec3 normal = normalize( vec4( u_normalMatrix * vec4( a_normal, 1.0) ).xyz ); */

  /* vec3 normal = normalize( vec4( u_normalMatrix * vec4( a_normal, 1.0) ).xyz ); */
  /* vec3 normal = normalize( a_normal ); */

  /* vec3 normal = u_normalMatrix * normalize( a_normal ); */

  /* vec3 normal = vec4( u_normalMatrix * vec4( a_normal ) ).xyz; */

  /* vec4 normal = u_normalMatrix * vec4( a_normal, 1 ); */

  v_refraction = refract( incident, normal, Eta );
  v_reflection = reflect( incident, normal );

  /* v_refraction = refract( incident, normal.xyz, Eta ); */
  /* v_reflection = reflect( incident, normal.xyz ); */

  // see http://en.wikipedia.org/wiki/Schlick%27s_approximation
  v_fresnel = R0 + (1.0 - R0) * pow( (1.0 - dot( -incident, normal ) ), 5.0);
  /* v_fresnel = R0 + (1.0 - R0) * pow( (1.0 - dot( -incident, normal ) ), 0.2); */
  /* v_fresnel = 0.0; */

  gl_Position = u_viewProjectionMatrix * vertex;

  /* vec4 vertex2 = u_modelMatrix_vertex * vec4( a_vertex, 1.0 ); */
  /* gl_Position = u_viewProjectionMatrix * vertex2; */
}
