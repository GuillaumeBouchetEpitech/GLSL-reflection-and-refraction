

#ifndef	PROGRAM_HPP
#define	PROGRAM_HPP


#include "Header.hpp"

#include "myGL/myGL.hpp"


class	Program
{
private :

  sf::RenderWindow	m_RWindow;

private :

  GL_Camera_FreeFly		m_FreeFly;

  bool				m_vbo_enabled;

private :

  GL_CubeMap	m_CubeMap;

  GL_Shader	m_BackgroundSphereShader;
  GL_Geometry*	m_pBackgroundSphere;

  GL_Shader	m_MainSphereShader;
  GL_Geometry*	m_pFresnelGeometry;
  GL_Matrix	m_FresnelModelMatrix;

  GL_CubeMap	m_CubeMap_dynamic;
  GL_Texture	m_pTexture[ GL_CubeMap::eCubeMapSens_Count ];


  GL_Shader	m_IntruderSphereShader;
  GL_Geometry*	m_pIntruderGeometry;
  float		m_Intruder_angle;


public :

  Program();
  ~Program();

private :

  void	Initialize_2D();
  void	Initialize_3D();

  void	Process_Events(sf::Event& event);
  void	Update(float elapsed_time);

  void	Render_3D();

private :

  void	Render_3D_cubmap_update();

  void	Render_3D_BackgroudSphere(GL_Matrix& viewProjectionMatrix);
  void	Render_3D_FresnelTorus(GL_Matrix& viewProjectionMatrix);
  void	Render_3D_IntruderSphere(GL_Matrix& viewProjectionMatrix);

public :

  void	Run();

};


#endif

