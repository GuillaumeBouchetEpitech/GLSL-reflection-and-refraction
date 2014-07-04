

#include "GL_Light.hpp"



GLfloat	GL_Base_Light::m_v4[4];


// void	GL_Light::Set_Material_Ambient(float x, float y, float z, float w)
// {
//   m_v4[0] = x;
//   m_v4[1] = y;
//   m_v4[2] = z;
//   m_v4[3] = w;

//   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_v4);
// }

// void	GL_Light::Set_Material_Diffuse(float x, float y, float z, float w)
// {
//   m_v4[0] = x;
//   m_v4[1] = y;
//   m_v4[2] = z;
//   m_v4[3] = w;

//   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_v4);
// }

// void	GL_Light::Set_Material_Specular(float x, float y, float z, float w)
// {
//   m_v4[0] = x;
//   m_v4[1] = y;
//   m_v4[2] = z;
//   m_v4[3] = w;

//   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_v4);
// }

// void	GL_Light::Set_Material_Shininess(float value)
// {
//   glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, value);
// }


// void	GL_Light::Set_Diffuse(unsigned int light, float x, float y, float z, float w)
// {
//   if (light >= 8)
//     return;

//   m_v4[0] = x;
//   m_v4[1] = y;
//   m_v4[2] = z;
//   m_v4[3] = w;

//   glLightfv(GL_LIGHT0 + light, GL_DIFFUSE, m_v4);
// }

// void	GL_Light::Set_Linear_Attenuation(unsigned int light, float value)
// {
//   if (light >= 8)
//     return;

//   glLightf(GL_LIGHT0 + light, GL_LINEAR_ATTENUATION, value);
// }

// void	GL_Light::Set_Quadratic_Attenuation(unsigned int light, float value)
// {
//   if (light >= 8)
//     return;

//   glLightf(GL_LIGHT0 + light, GL_QUADRATIC_ATTENUATION, value);
// }

// void	GL_Light::Set_Constant_Attenuation(unsigned int light, float value)
// {
//   if (light >= 8)
//     return;

//   glLightf(GL_LIGHT0 + light, GL_CONSTANT_ATTENUATION, value);
// }

// void	GL_Light::Set_Position(unsigned int light, float x, float y, float z)
// {
//   if (light >= 8)
//     return;

//   m_v4[0] = x;
//   m_v4[1] = y;
//   m_v4[2] = z;
//   m_v4[3] = 1;

//   glLightfv(GL_LIGHT0 + light, GL_POSITION, m_v4);
// }

// //////////////////////////////////////////

// void	GL_Light::Enable_Material()
// {
//   glEnable(GL_COLOR_MATERIAL);
//   glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
// }

// void	GL_Light::Disable_Material()
// {
//   glDisable(GL_COLOR_MATERIAL);
// }

// //////////////////////////////////////////

// void	GL_Light::Enable_Light(unsigned int light)
// {
//   if (light >= 8)
//     return;

//   glEnable(GL_LIGHT0 + light);
// }

// void	GL_Light::Disable_Light(unsigned int light)
// {
//   if (light >= 8)
//     return;

//   glDisable(GL_LIGHT0 + light);
// }

// //////////////////////////////////////////

// void	GL_Light::Enable_Lighting()
// {
//   glEnable(GL_LIGHTING);
// }

// void	GL_Light::Disable_Lighting()
// {
//   glDisable(GL_LIGHTING);
// }


