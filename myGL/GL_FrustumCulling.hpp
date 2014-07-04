

#ifndef	FRUSTUM_HPP
#define	FRUSTUM_HPP


#include "Header.hpp"

#include "GL_Matrix.hpp"

#include <cmath>


// class	BoundingBox;

class	GL_FrustumCulling
{
private :

  enum e_FrustumSide {eRight, eLeft, eBottom, eTop, eBack, eFront};
  enum e_PlaneData {eA, eB, eC, eD};

private :

  float	m_Frustum[6][4];

private :

  void	NormalizePlane(float frustum[6][4], int side);

public :

  void	CalculateFrustum();
  void	CalculateFrustum(const GL_Matrix& proj, const GL_Matrix& modl);
  void	CalculateFrustum(float* proj, float* modl);

public :

  bool	PointInFrustum( float x, float y, float z ) const;
  bool	PointInFrustum( const sf::Vector3f& pos ) const
  {
    return (PointInFrustum(pos.x, pos.y, pos.z));
  }

  bool	SphereInFrustum( float x, float y, float z, float radius ) const;
  bool	SphereInFrustum( const sf::Vector3f& pos, float radius ) const
  {
    return (SphereInFrustum(pos.x, pos.y, pos.z, radius));
  }

  bool	CubeInFrustum( float x, float y, float z, float size ) const;
  bool	CubeInFrustum( const sf::Vector3f& pos, float size ) const
  {
    return (CubeInFrustum(pos.x, pos.y, pos.z, size));
  }

  bool	CubeInFrustum( const sf::Vector3f& v, const sf::Vector3f& s) const;
  // bool	BoundingBoxInFrustum(BoundingBox& BB);


public :

  void	CalcOrtho( sf::Vector3f eye, sf::Vector3f target,
		   float left, float right,
		   float bottom, float top,
		   float nearval, float farval );

  void	CalcPerspective( sf::Vector3f eye, sf::Vector3f target,
			 float angle, float aspect_ratio,
			 float nearval, float farval );

public :

  void	DrawOrtho( sf::Vector3f eye, sf::Vector3f target,
		   float left, float right,
		   float bottom, float top,
		   float nearval, float farval );

  void	DrawPerspective( sf::Vector3f eye, sf::Vector3f target,
			 float angle, float aspect_ratio,
			 float nearval, float farval );

};



#endif
