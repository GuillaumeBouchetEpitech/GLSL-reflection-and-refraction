

#ifndef	GL_LIGHT_HPP
#define	GL_LIGHT_HPP


#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <GL/gl.h>
#include <GL/glu.h>



class	GL_Base_Light
{
protected :

  static GLfloat	m_v4[4];

};

template<int l>
class	GL_Light : protected GL_Base_Light
{
public :

  static void	Set_Global_Ambient(float x, float y, float z, float w)
  {
    m_v4[0] = x;
    m_v4[1] = y;
    m_v4[2] = z;
    m_v4[3] = w;

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, m_v4);
  }

  static void	Enable_Lighting()
  {
    glEnable(GL_LIGHTING);
  }

  static void	Disable_Lighting()
  {
    glDisable(GL_LIGHTING);
  }

public :

  static void	Set_Diffuse(float x, float y, float z, float w)
  {
    m_v4[0] = x;
    m_v4[1] = y;
    m_v4[2] = z;
    m_v4[3] = w;

    glLightfv(GL_LIGHT0 + l, GL_DIFFUSE, m_v4);
  }

  static void	Set_Ambiant(float x, float y, float z, float w)
  {
    m_v4[0] = x;
    m_v4[1] = y;
    m_v4[2] = z;
    m_v4[3] = w;

    glLightfv(GL_LIGHT0 + l, GL_AMBIENT, m_v4);
  }

  static void	Set_Specular(float x, float y, float z, float w)
  {
    m_v4[0] = x;
    m_v4[1] = y;
    m_v4[2] = z;
    m_v4[3] = w;

    glLightfv(GL_LIGHT0 + l, GL_SPECULAR, m_v4);
  }

  static void	Set_Position(float x, float y, float z)
  {
    m_v4[0] = x;
    m_v4[1] = y;
    m_v4[2] = z;
    m_v4[3] = 1;

    glLightfv(GL_LIGHT0 + l, GL_POSITION, m_v4);
  }

public :

  static void	Set_Linear_Attenuation(float value)
  {
    glLightf(GL_LIGHT0 + l, GL_LINEAR_ATTENUATION, value);
  }

  static void	Set_Quadratic_Attenuation(float value)
  {
    glLightf(GL_LIGHT0 + l, GL_QUADRATIC_ATTENUATION, value);
  }

  static void	Set_Constant_Attenuation(float value)
  {
    glLightf(GL_LIGHT0 + l, GL_CONSTANT_ATTENUATION, value);
  }

public :

  static void	Enable()
  {
    glEnable(GL_LIGHT0 + l);
  }

  static void	Disable()
  {
    glDisable(GL_LIGHT0 + l);
  }

};

typedef GL_Light<0>	GL_Light0;
typedef GL_Light<1>	GL_Light1;
typedef GL_Light<2>	GL_Light2;
typedef GL_Light<3>	GL_Light3;
typedef GL_Light<4>	GL_Light4;
typedef GL_Light<5>	GL_Light5;
typedef GL_Light<6>	GL_Light6;
typedef GL_Light<7>	GL_Light7;


class	GL_Material : protected GL_Base_Light
{
public :

  static void	Set_Ambient(float x, float y, float z, float w)
  {
    m_v4[0] = x;
    m_v4[1] = y;
    m_v4[2] = z;
    m_v4[3] = w;

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_v4);
  }

  static void	Set_Diffuse(float x, float y, float z, float w)
  {
    m_v4[0] = x;
    m_v4[1] = y;
    m_v4[2] = z;
    m_v4[3] = w;

    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_v4);
  }

  static void	Set_Specular(float x, float y, float z, float w)
  {
    m_v4[0] = x;
    m_v4[1] = y;
    m_v4[2] = z;
    m_v4[3] = w;

    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_v4);
  }

  static void	Set_Shininess(float value)
  {
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, value);
  }

public :

  static void	Enable()
  {
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  }

  static void	Disable()
  {
    glDisable(GL_COLOR_MATERIAL);
  }

};


// class	GL_Light
// {
// private :

//   static GLfloat	m_v4[4];

// public :

//   static void	Set_Global_Ambient(float x, float y, float z, float w)
//   {
//     m_v4[0] = x;
//     m_v4[1] = y;
//     m_v4[2] = z;
//     m_v4[3] = w;

//     glLightModelfv(GL_LIGHT_MODEL_AMBIENT, m_v4);
//   }

// public :

//   static void	Set_Material_Ambient(float x, float y, float z, float w);
//   static void	Set_Material_Diffuse(float x, float y, float z, float w);
//   static void	Set_Material_Specular(float x, float y, float z, float w);
//   static void	Set_Material_Shininess(float value);

// public :

//   static void	Set_Diffuse(unsigned int light, float x, float y, float z, float w);
//   static void	Set_Linear_Attenuation(unsigned int light, float value);
//   static void	Set_Quadratic_Attenuation(unsigned int light, float value);
//   static void	Set_Constant_Attenuation(unsigned int light, float value);

// public :

//   static void	Set_Position(unsigned int light, float x, float y, float z);

// public :

//   static void	Enable_Material();
//   static void	Disable_Material();

// public :

//   static void	Enable_Light(unsigned int light);
//   static void	Disable_Light(unsigned int light);

// public :

//   static void	Enable_Lighting();
//   static void	Disable_Lighting();

// };


#endif

