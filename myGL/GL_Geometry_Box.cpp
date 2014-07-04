

#include "GL_Geometry.hpp"


void	GL_Geometry::MakeBox( float size_x, float size_y, float size_z,
			      unsigned int qual_x, unsigned int qual_y, unsigned int qual_z )
{
  float	hsize_x = size_x / 2;
  float	hsize_y = size_y / 2;
  float	hsize_z = size_z / 2;


  m_Vertices.clear();
  m_Normales.clear();
  m_Primitives.clear();
  m_Prim_index.clear();
  m_Norm_index.clear();


  m_Normales.push_back( sf::Vector3f( -1,0,0 ) );
  m_Normales.push_back( sf::Vector3f( +1,0,0 ) );

  m_Normales.push_back( sf::Vector3f( 0,-1,0 ) );
  m_Normales.push_back( sf::Vector3f( 0,+1,0 ) );

  m_Normales.push_back( sf::Vector3f( 0,0,-1 ) );
  m_Normales.push_back( sf::Vector3f( 0,0,+1 ) );


#define	D_FIND_V3(v3, idx)			\
  for (idx = 0; idx < m_Vertices.size(); ++idx)	\
    if ( v3 == m_Vertices[idx] )		\
      break;					\
						\
  if (idx == m_Vertices.size())			\
    m_Vertices.push_back(v3);

#define	D_PUSH_QUAD(did1, did2, did3, dnid)	\
  m_Primitives.push_back(0);			\
  m_Norm_index.push_back( dnid );		\
  m_Prim_index.push_back( did1 );		\
  m_Norm_index.push_back( dnid );		\
  m_Prim_index.push_back( did2 );		\
  m_Norm_index.push_back( dnid );		\
  m_Prim_index.push_back( did3 );		\


  for (unsigned int x = 0; x < qual_x + 1; ++x)
    for (unsigned int y = 0; y < qual_y + 1; ++y)
      {
	unsigned int	Zidx1, Zidx2, Zidx3, Zidx4;

	sf::Vector3f	Zv3_1(     x,     y, qual_z + 1 );
	sf::Vector3f	Zv3_2( x + 1,     y, qual_z + 1 );
	sf::Vector3f	Zv3_3( x + 1, y + 1, qual_z + 1 );
	sf::Vector3f	Zv3_4(     x, y + 1, qual_z + 1 );

	D_FIND_V3(Zv3_1, Zidx1);
	D_FIND_V3(Zv3_2, Zidx2);
	D_FIND_V3(Zv3_3, Zidx3);
	D_FIND_V3(Zv3_4, Zidx4);

	unsigned int	rZidx1, rZidx2, rZidx3, rZidx4;

	sf::Vector3f	rZv3_1(     x,     y, 0 );
	sf::Vector3f	rZv3_2( x + 1,     y, 0 );
	sf::Vector3f	rZv3_3( x + 1, y + 1, 0 );
	sf::Vector3f	rZv3_4(     x, y + 1, 0 );

	D_FIND_V3(rZv3_1, rZidx1);
	D_FIND_V3(rZv3_2, rZidx2);
	D_FIND_V3(rZv3_3, rZidx3);
	D_FIND_V3(rZv3_4, rZidx4);


	D_PUSH_QUAD( Zidx1, Zidx2, Zidx3, 5);
	D_PUSH_QUAD( Zidx1, Zidx3, Zidx4, 5);

	D_PUSH_QUAD( rZidx1, rZidx3, rZidx2, 4);
	D_PUSH_QUAD( rZidx1, rZidx4, rZidx3, 4);
      }


  for (unsigned int x = 0; x < qual_x + 1; ++x)
    for (unsigned int z = 0; z < qual_z + 1; ++z)
      {
	unsigned int	Yidx1, Yidx2, Yidx3, Yidx4;

	sf::Vector3f	Yv3_1(     x, qual_y + 1,     z );
	sf::Vector3f	Yv3_2( x + 1, qual_y + 1,     z );
	sf::Vector3f	Yv3_3( x + 1, qual_y + 1, z + 1 );
	sf::Vector3f	Yv3_4(     x, qual_y + 1, z + 1 );

	D_FIND_V3(Yv3_1, Yidx1);
	D_FIND_V3(Yv3_2, Yidx2);
	D_FIND_V3(Yv3_3, Yidx3);
	D_FIND_V3(Yv3_4, Yidx4);

	unsigned int	rYidx1, rYidx2, rYidx3, rYidx4;

	sf::Vector3f	rYv3_1(     x, 0,     z );
	sf::Vector3f	rYv3_2( x + 1, 0,     z );
	sf::Vector3f	rYv3_3( x + 1, 0, z + 1 );
	sf::Vector3f	rYv3_4(     x, 0, z + 1 );

	D_FIND_V3(rYv3_1, rYidx1);
	D_FIND_V3(rYv3_2, rYidx2);
	D_FIND_V3(rYv3_3, rYidx3);
	D_FIND_V3(rYv3_4, rYidx4);


	D_PUSH_QUAD( Yidx1, Yidx3, Yidx2, 3);
	D_PUSH_QUAD( Yidx1, Yidx4, Yidx3, 3);

	D_PUSH_QUAD( rYidx1, rYidx2, rYidx3, 2);
	D_PUSH_QUAD( rYidx1, rYidx3, rYidx4, 2);
      }


  for (unsigned int y = 0; y < qual_y + 1; ++y)
    for (unsigned int z = 0; z < qual_z + 1; ++z)
      {
	unsigned int	Xidx1, Xidx2, Xidx3, Xidx4;

	sf::Vector3f	Xv3_1( qual_x + 1,     y,     z );
	sf::Vector3f	Xv3_2( qual_x + 1, y + 1,     z );
	sf::Vector3f	Xv3_3( qual_x + 1, y + 1, z + 1 );
	sf::Vector3f	Xv3_4( qual_x + 1,     y, z + 1 );

	D_FIND_V3(Xv3_1, Xidx1);
	D_FIND_V3(Xv3_2, Xidx2);
	D_FIND_V3(Xv3_3, Xidx3);
	D_FIND_V3(Xv3_4, Xidx4);

	unsigned int	rXidx1, rXidx2, rXidx3, rXidx4;

	sf::Vector3f	rXv3_1( 0,     y,     z );
	sf::Vector3f	rXv3_2( 0, y + 1,     z );
	sf::Vector3f	rXv3_3( 0, y + 1, z + 1 );
	sf::Vector3f	rXv3_4( 0,     y, z + 1 );

	D_FIND_V3(rXv3_1, rXidx1);
	D_FIND_V3(rXv3_2, rXidx2);
	D_FIND_V3(rXv3_3, rXidx3);
	D_FIND_V3(rXv3_4, rXidx4);


	D_PUSH_QUAD( Xidx1, Xidx2, Xidx3, 1);
	D_PUSH_QUAD( Xidx1, Xidx3, Xidx4, 1);

	D_PUSH_QUAD( rXidx1, rXidx3, rXidx2, 0);
	D_PUSH_QUAD( rXidx1, rXidx4, rXidx3, 0);
      }

#undef	D_FIND_V3
#undef	D_PUSH_QUAD


  size_x /= (qual_x + 1);
  size_y /= (qual_y + 1);
  size_z /= (qual_z + 1);

  for (unsigned int i = 0; i < m_Vertices.size(); ++i)
    {
      m_Vertices[i].x *= size_x;
      m_Vertices[i].y *= size_y;
      m_Vertices[i].z *= size_z;

      m_Vertices[i].x -= hsize_x;
      m_Vertices[i].y -= hsize_y;
      m_Vertices[i].z -= hsize_z;
    }


  Setup();
}


GL_Geometry*	GL_Geometry::CreateBox( float size_x, float size_y, float size_z,
					unsigned int qual_x, unsigned int qual_y, unsigned int qual_z )
{
  GL_Geometry*	pGeom = NULL;

  if (size_x > 0.0f && size_y > 0.0f && size_z > 0.0f)
    {
      pGeom = new GL_Geometry(0);

      pGeom->MakeBox( size_x,size_y,size_z, qual_x,qual_y,qual_z );
    }

  return (pGeom);
}
