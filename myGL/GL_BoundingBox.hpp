

#ifndef	GL_BOUNDINGBOX_HPP
#define	GL_BOUNDINGBOX_HPP


#include "Header.hpp"

#include "GL_Geometry.hpp"
#include "GL_Matrix.hpp"

#include <iostream>


class	GL_BoundingBox
{
private :

  // GL_Matrix	m_mat;
  // GL_Geometry	m_Geom;
  sf::Vector3f	m_Min, m_Max;

public :

  GL_BoundingBox();

public :

  const sf::Vector3f&	GetMin() const	{return (m_Min);}
  const sf::Vector3f&	GetMax() const	{return (m_Max);}

public :

  void	Reset(const sf::Vector3f& v3);

  void	ExpandBy(float x, float y, float z);
  inline void	ExpandBy(const sf::Vector3f& v3)
  {
    ExpandBy(v3.x, v3.y, v3.z);
  }

  void	Render();

};


#endif

