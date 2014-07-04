

#ifndef	CAMERA_FREEFLY_HH
#define	CAMERA_FREEFLY_HH


#define _USE_MATH_DEFINES
#include <cmath>


#include "Header.hpp"


#include "GL_Matrix.hpp"


#include "GL_FrustumCulling.hpp"



class	GL_Camera_FreeFly
{
private :

  sf::RenderWindow&	m_RWindow;

  sf::Vector3f		m_Position;
  sf::Vector3f		m_Forward;
  sf::Vector3f		m_Up;
  sf::Vector3f		m_Left;
  sf::Vector3f		m_Target;

  float			m_speed;
  float			m_sensivity;
  float			m_theta;
  float			m_phi;

  bool			m_active;


  // GL_Matrix		m_modl;
  // GL_Matrix		m_proj;
  GL_Matrix		m_Mat4x4_proj;
  GL_Matrix		m_Mat4x4_view;


  GL_FrustumCulling	m_Frustum;


public : // ctor / dtor

  GL_Camera_FreeFly(sf::RenderWindow& RWindow);

public : // init

  void	Initialize();

  void  Resize();

public : // step

  void  Active();
  void  Desactive();

  void	Event(const sf::Event& Event);
  void	Update(const float elapsedTime);
  void	Look() const;

public : // getter

  inline const sf::Vector3f&	GetPosition() const {return (m_Position);}
  inline const sf::Vector3f&	GetForward() const {return (m_Forward);}
  inline bool	IsActive() const	{return (m_active);}

  inline float	GetTheta() const	{return (m_theta);}

  const GL_FrustumCulling&	GetFrustum() const	{return (m_Frustum);}


  // const GL_Matrix&	Get_ModelView() const	{return (m_modl);}
  // const GL_Matrix&	Get_Projection() const	{return (m_proj);}
  const GL_Matrix&	Get_ViewMatrix() const		{return (m_Mat4x4_view);}
  const GL_Matrix&	Get_ProjectionMatrix() const	{return (m_Mat4x4_proj);}


private : // utils

  void	VectorsFromAngles();
  void	SetActive(bool Active);

public : // utils

  inline float	GetSpeed() const	{return (m_speed);}

public :

  bool	Get2DPoint(const sf::Vector3f& Target, sf::Vector2f& Position);

  void	Do_BillBoard(const sf::Vector3f& Object_Pos);

};

#endif
