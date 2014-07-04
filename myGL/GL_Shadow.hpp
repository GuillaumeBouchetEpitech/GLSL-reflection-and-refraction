

#ifndef	GL_SHADOW_HPP
#define	GL_SHADOW_HPP


#include "Header.hpp"


#include "GL_Matrix.hpp"


class	GL_Geometry;

class	GL_Shadow
{
private :

  struct		t_Triangle
  {
    unsigned int	p[3];
    unsigned int	n[3];

    unsigned int	neigh[3];

    float		PlaneEq[4];
    bool		visible;
  };

private :

  GL_Geometry*		m_pGeom;

  std::vector< t_Triangle >	m_Triangles;
  std::vector< sf::Vector3f >	m_Vertices;

private : // utils

  void	CalcPlane(t_Triangle& plane, GL_Matrix* pMat = NULL);

public : // ctor / dtor

  GL_Shadow();
  GL_Shadow(GL_Geometry& Geom);

public : // init

  void	Initialize(GL_Geometry* pGeom, GL_Matrix* pMat = NULL);

  void	Setup(const sf::Vector3f& lightPos, GL_Matrix* pMat = NULL, float length = 50.0f);

public : // step

  void	Render();

  void	Cast();

};


#endif

