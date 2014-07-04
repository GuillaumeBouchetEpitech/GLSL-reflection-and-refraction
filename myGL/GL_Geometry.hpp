

#ifndef	GL_GEOMETRY_HPP
#define	GL_GEOMETRY_HPP


#include <iostream>
#include <fstream>


#include "Header.hpp"


#include <sstream>
#include <fstream>

#include <cstdlib>
#include <cstdio>
#include <cstring>


class	GL_OBJLoader;
class	GL_Shadow;

class	GL_Geometry
{
private :

  friend class	SceneElement;
  friend class	GL_OBJLoader;
  friend class	GL_Shadow;

// protected : // va
public : // va

  std::vector< sf::Vector3f >	m_Vertices;
  std::vector< sf::Vector3f >	m_Normales;
  std::vector< sf::Vector2f >	m_TexCoords;

  std::vector< unsigned int >	m_Primitives;

  std::vector< unsigned int >	m_Prim_index;
  std::vector< unsigned int >	m_Norm_index;
  std::vector< unsigned int >	m_TexC_index;

// protected : // vbo
public :

  enum {
    eIndices = 0,
    eVertices,
    eNormals,
    eTexcoords,

    eIdsCount
  };

  bool		m_vbo_computed;
  GLuint	m_vbo_ids[ eIdsCount ];

  // GLuint	m_vbo_id;

  struct	t_Vertex
  {
    float	x, y, z;
    float	nx, ny, nz;
    float	r, g, b;
    float	tx, ty;
  };

  std::vector< t_Vertex >	m_vbo_Vertices;

  // float		m_ModelView[16];

public : // ctor / dtor

  GL_Geometry(unsigned int reserved_size = 0);
  ~GL_Geometry();

public : // vbo

  void	ComputeVBO();
  void	DestroyVBO();

public : // init

  virtual void	Setup();

  /**

public : // step

  void	Reset_Matrix()
  {
    memset(m_ModelView, 0, 16 * sizeof(float));
  }

  void	Save_Matrix()
  {
    glGetFloatv(GL_MODELVIEW_MATRIX, m_ModelView);
  }

  sf::Vector3f	GetCenter()
  {

    return ( sf::Vector3f(m_ModelView[12], m_ModelView[13], m_ModelView[14]) );

    // sf::Vector3f	to_mult();

    // GLfloat res[4];

    // res[0] =
    //   m_ModelView[ 0] * to_mult.x +
    //   m_ModelView[ 4] * to_mult.y +
    //   m_ModelView[ 8] * to_mult.z +
    //   m_ModelView[12] * 1.0f;

    // res[1] =
    //   m_ModelView[ 1] * to_mult.x +
    //   m_ModelView[ 5] * to_mult.y +
    //   m_ModelView[ 9] * to_mult.z +
    //   m_ModelView[13] * 1.0f;

    // res[2] =
    //   m_ModelView[ 2] * to_mult.x +
    //   m_ModelView[ 6] * to_mult.y +
    //   m_ModelView[10] * to_mult.z +
    //   m_ModelView[14] * 1.0f;

    // res[3] =
    //   m_ModelView[ 3] * to_mult.x +
    //   m_ModelView[ 7] * to_mult.y +
    //   m_ModelView[11] * to_mult.z +
    //   m_ModelView[15] * 1.0f;

    // to_mult.x = res[0];
    // to_mult.y = res[1];
    // to_mult.z = res[2];
    // // v[3] = res[3];
  }
  //*/

  void	Render(bool use_vbo, bool use_backLine);

public : // utils

  void	MakeBox( float size_x, float size_y, float size_z,
  		 unsigned int qual_x, unsigned int qual_y, unsigned int qual_z );

  void	MakeSphere( float radius, unsigned int quality );

  void	MakeCylinder( float length,
  		      float radius_top_in, float radius_top_out,
  		      float radius_bottom_in, float radius_bottom_out,
  		      unsigned int quality );

  void	MakePartialCylinder( float length,
  			     float startAngle, float sweepAngle,
  			     float radius_top_in, float radius_top_out,
  			     float radius_bottom_in, float radius_bottom_out,
  			     unsigned int quality );

  void	MakeTorus( float startAngle, float sweepAngle,
  		   float radius, float radius_in, float radius_out,
  		   unsigned int cyl_qual, unsigned int cyl_nbr );

public : // utils

  static GL_Geometry*	CreateBox( float size_x, float size_y, float size_z,
  				   unsigned int qual_x, unsigned int qual_y, unsigned int qual_z );

  static GL_Geometry*	CreateSphere( float radius, unsigned int quality );

  static GL_Geometry*	CreateCylinder( float length,
  					float radius_top_in, float radius_top_out,
  					float radius_bottom_in, float radius_bottom_out,
  					unsigned int quality );

  static GL_Geometry*	CreatePartialCylinder( float length,
  					       float startAngle, float sweepAngle,
  					       float radius_top_in, float radius_top_out,
  					       float radius_bottom_in, float radius_bottom_out,
  					       unsigned int quality );

  static GL_Geometry*	CreateTorus( float startAngle, float sweepAngle,
  				     float radius, float radius_in, float radius_out,
  				     unsigned int cyl_qual, unsigned int cyl_nbr );

};


#endif

