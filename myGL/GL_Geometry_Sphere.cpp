

#include "GL_Geometry.hpp"

#include <cmath>

#include <set>


#include <iostream>
#include <iomanip>


struct	t_index_vertex
{
public :

  unsigned int	s_index;
  sf::Vector3f*	s_pVertex;

public :

  t_index_vertex(unsigned int index, sf::Vector3f* pVertex)
    : s_index(index), s_pVertex(pVertex)
  {
  }

};

static bool	fncomp(const t_index_vertex& lhs, const t_index_vertex& rhs)
{
  if (lhs.s_pVertex->x < rhs.s_pVertex->x)
    return (true);
  else if (lhs.s_pVertex->x > rhs.s_pVertex->x)
    return (false);
  else
    {
      if (lhs.s_pVertex->y < rhs.s_pVertex->y)
	return (true);
      else if (lhs.s_pVertex->y > rhs.s_pVertex->y)
	return (false);
      else
	// {
	return (lhs.s_pVertex->z < rhs.s_pVertex->z);
      // if (lhs->z < rhs->z)
      //   return (true);
      // else if (lhs->z > rhs->z)
      //   return (false);
      // else
      //   return (false);
      // }
    }
}


typedef bool (*t_cmpfunc)(const t_index_vertex&, const t_index_vertex&);

std::set< t_index_vertex, t_cmpfunc >	set_treeSearcher(fncomp);



struct	t_drawPatch_data
{
public :

  static std::vector< sf::Vector3f >*	s_Vertices;
  static std::vector< sf::Vector3f >*	s_Normales;
  static std::vector< sf::Vector2f >*	s_TexCoords;
  static std::vector< unsigned int >*	s_Indices;
  static std::vector< unsigned int >*	s_Indices2;
  static float				s_radius;

  sf::Vector3f	&s_p1, &s_p2, &s_p3;
  int s_level;

public :

  static void	Initialize( std::vector< sf::Vector3f >& Vertices,
			    std::vector< sf::Vector3f >& Normales,
			    std::vector< sf::Vector2f >& TexCoords,
			    std::vector< unsigned int >& Indices,
			    std::vector< unsigned int >& Indices2,
			    float radius )
  {
    s_Vertices = &Vertices;
    s_Normales = &Normales;
    s_TexCoords = &TexCoords;
    s_Indices = &Indices;
    s_Indices2 = &Indices2;
    s_radius = radius;
  }

  t_drawPatch_data(sf::Vector3f& p1,
		   sf::Vector3f& p2,
		   sf::Vector3f& p3,
		   int level)
    : s_p1(p1), s_p2(p2), s_p3(p3),
      s_level(level)
  {
  }

};

std::vector< sf::Vector3f >*	t_drawPatch_data::s_Vertices;
std::vector< sf::Vector3f >*	t_drawPatch_data::s_Normales;
std::vector< sf::Vector2f >*	t_drawPatch_data::s_TexCoords;
std::vector< unsigned int >*	t_drawPatch_data::s_Indices;
std::vector< unsigned int >*	t_drawPatch_data::s_Indices2;
float				t_drawPatch_data::s_radius;


void	drawPatch( const t_drawPatch_data& data )
{
  if (data.s_level > 0)
    {
      sf::Vector3f	q1, q2, q3;

#define	D_SET_VERTEX(q, l, r)		\
      (q).x = 0.5f * ((l).x + (r).x);	\
      (q).y = 0.5f * ((l).y + (r).y);	\
      (q).z = 0.5f * ((l).z + (r).z);

      D_SET_VERTEX(q1, data.s_p1, data.s_p2);
      D_SET_VERTEX(q2, data.s_p2, data.s_p3);
      D_SET_VERTEX(q3, data.s_p3, data.s_p1);

#undef	D_SET_VERTEX

#define	D_SQ(v)			((v) * (v))
#define	D_VEC_LENGTH(v3)	(sqrtf( D_SQ( (v3).x ) + D_SQ( (v3).y ) + D_SQ( (v3).z ) ))

#define	D_NORM(q)						\
      length = 1.0f / D_VEC_LENGTH(q);				\
      (q).x *= length; (q).y *= length; (q).z *= length;

      float	length;

      D_NORM(q1);
      D_NORM(q2);
      D_NORM(q3);

#undef	D_NORM
#undef	D_VEC_LENGTH
#undef	D_SQ

      drawPatch( t_drawPatch_data( data.s_p1, q1, q3, data.s_level - 1 ) );
      drawPatch( t_drawPatch_data( q1, data.s_p2, q2, data.s_level - 1 ) );
      drawPatch( t_drawPatch_data( q1, q2, q3,        data.s_level - 1 ) );
      drawPatch( t_drawPatch_data( q3, q2, data.s_p3, data.s_level - 1 ) );
    }

  else
    {
      sf::Vector3f*	px_array[] = { &data.s_p1, &data.s_p2, &data.s_p3 };

      for (int i_point = 0; i_point < 3; ++i_point)
	{
	  sf::Vector3f&	curr_px = *px_array[i_point];

	  unsigned int	i_search = 0;

	  ////
	  std::set< t_index_vertex, t_cmpfunc >::iterator	set_itr;
	  t_index_vertex	index_vertex(data.s_Normales->size(), &curr_px);

	  set_itr = set_treeSearcher.find( index_vertex );

	  if (set_itr != set_treeSearcher.end())
	    i_search = set_itr->s_index;

	  else
	    {
	      i_search = index_vertex.s_index;

	      data.s_Vertices->push_back( curr_px * data.s_radius );
	      data.s_Normales->push_back( curr_px );


	      {
		sf::Vector2f	texCoord;

		{ // billboard

		  float	theta = 0;
		  float	phi = 0;

		  {
		    // sf::Vector3f	Center(0, 0, 0);
		    sf::Vector3f	Normal(curr_px);

		    // theta = atan2f( Center.y - Normal.y, Center.x - Normal.x ) * 180.0f / 3.14f;
		    theta = atan2f( Normal.y, Normal.x ) * 180.0f / 3.14f;

#define	D_SQ(v)		((v) * (v))
// #define	D_LENGTH(v1, v2) sqrtf( D_SQ((v1).x - (v2).x) + D_SQ((v1).y - (v2).y) )
#define	D_LENGTH(v3)	sqrtf( D_SQ( (v3).x ) + D_SQ( (v3).y ) )

		    sf::Vector2f	Profile_Pos;

		    // Profile_Pos.x = D_LENGTH( Center, Normal );
		    // Profile_Pos.y = Center.z - Normal.z;
		    Profile_Pos.x = D_LENGTH( Normal );
		    Profile_Pos.y = Normal.z;

#undef	D_SQ

		    phi = atan2f( Profile_Pos.y, Profile_Pos.x ) * 180.0f / 3.14f;

		  }


		  // std::cerr << "phi : " << phi << ", theta : " << theta << std::endl;

		  texCoord.x = (180.0f + theta) / 360.0f;
		  texCoord.y = (90.0f + phi) / 180.0f;

		  // std::cerr << std::fixed << std::setprecision(2)
		  // 	    << "v (" << std::setw(5) << texCoord.x
		  // 	    << " / " << std::setw(5) << texCoord.y
		  // 	    << "), theta (" << std::setw(7) << theta
		  // 	    << "), phi (" << std::setw(7) << phi << ")" << std::endl;

		} // /billboard

		// texCoord.x = 0.5f + 0.5f * curr_px.x;
		// texCoord.y = 0.5f + 0.5f * curr_px.y;


		// texCoord.x = (1.0f - curr_px.x) * 0.5f;
		// texCoord.y = (1.0f - curr_px.y) * 0.5f;


		// std::cerr << "tx : " << texCoord.x << " / " << texCoord.y << std::endl;

		data.s_TexCoords->push_back( texCoord );
	      }

	      index_vertex.s_pVertex = &data.s_Normales->back();

	      set_treeSearcher.insert( index_vertex );
	    }
	  ////


	  // for (; i_search < data.s_Normales->size(); ++i_search)
	  //   if ( (*data.s_Normales)[i_search].x == curr_px.x &&
	  //   	 (*data.s_Normales)[i_search].y == curr_px.y &&
	  //   	 (*data.s_Normales)[i_search].z == curr_px.z )
	  //     break;

	  // if (i_search == data.s_Normales->size())
	  //   {
	  //     data.s_Vertices->push_back( curr_px * data.s_radius );
	  //     data.s_Normales->push_back( curr_px );
	  //   }

	  data.s_Indices->push_back( i_search );
	  data.s_Indices2->push_back( i_search );
	}

    }
}


void	GL_Geometry::MakeSphere( float radius, unsigned int quality )
{
  m_Primitives.clear();
  m_Vertices.clear();
  m_Normales.clear();
  m_TexCoords.clear();
  m_Prim_index.clear();
  m_Norm_index.clear();

  set_treeSearcher.clear();

  {
    unsigned int	vertices_size = 12;
    unsigned int	indices_size = 20;

    int	inc = (quality > 0 ? 30 : 0);

    for (unsigned int i = 0; i < quality; ++i)
      {
	indices_size *= 4;

	vertices_size += inc;
	inc *= 4;
      }

    m_Primitives.resize( indices_size, 0 );

    if (vertices_size <= m_Vertices.capacity())
      m_Vertices.reserve( vertices_size );

    if (vertices_size <= m_Normales.capacity())
      m_Normales.reserve( vertices_size );

    if (indices_size * 3 <= m_Prim_index.capacity())
      m_Prim_index.reserve( indices_size * 3 );

    if (indices_size * 3 <= m_Norm_index.capacity())
      m_Norm_index.reserve( indices_size * 3 );
  }

#define	D_ICX	0.525731112119133606f
#define	D_ICZ	0.850650808352039932f

  static sf::Vector3f	idata[12] = {
    sf::Vector3f(-D_ICX, 0.0f, +D_ICZ),
    sf::Vector3f(+D_ICX, 0.0f, +D_ICZ),
    sf::Vector3f(-D_ICX, 0.0f, -D_ICZ),
    sf::Vector3f(+D_ICX, 0.0f, -D_ICZ),
    sf::Vector3f(0.0f, +D_ICZ, +D_ICX),
    sf::Vector3f(0.0f, +D_ICZ, -D_ICX),
    sf::Vector3f(0.0f, -D_ICZ, +D_ICX),
    sf::Vector3f(0.0f, -D_ICZ, -D_ICX),
    sf::Vector3f(+D_ICZ, +D_ICX, 0.0f),
    sf::Vector3f(-D_ICZ, +D_ICX, 0.0f),
    sf::Vector3f(+D_ICZ, -D_ICX, 0.0f),
    sf::Vector3f(-D_ICZ, -D_ICX, 0.0f)
  };

#undef	D_ICX
#undef	D_ICZ

  static int	index[20][3] = {
    {0,  4,  1},	{ 0,  9,  4},
    {9,  5,  4},	{ 4,  5,  8},
    {4,  8,  1},	{ 8, 10,  1},
    {8,  3, 10},	{ 5,  3,  8},
    {5,  2,  3},	{ 2,  7,  3},
    {7, 10,  3},	{ 7,  6, 10},
    {7, 11,  6},	{11,  0,  6},
    {0,  1,  6},	{ 6,  1, 10},
    {9,  0, 11},	{ 9, 11,  2},
    {9,  2,  5},	{ 7,  2, 11},
  };

  t_drawPatch_data::Initialize( m_Vertices, m_Normales, m_TexCoords,
				m_Prim_index, m_Norm_index,
				radius );

  for (int i = 0; i < 20; i++)
    {
      drawPatch( t_drawPatch_data( idata[ index[i][2] ],
				   idata[ index[i][1] ],
				   idata[ index[i][0] ],
				   quality ) );
    }

  // std::cerr << std::endl;

  // std::cerr << "Sphere stats :" << std::endl;
  // std::cerr << "m_Vertices   :" << m_Vertices.size() << std::endl;
  // std::cerr << "m_Normales   :" << m_Normales.size() << std::endl;
  // std::cerr << "m_TexCoords  :" << m_TexCoords.size() << std::endl;
  // std::cerr << "m_Prim_index :" << m_Prim_index.size() << std::endl;
  // std::cerr << "m_Norm_index :" << m_Norm_index.size() << std::endl;

  // std::cerr << std::endl;

  // for ( unsigned int i = 0; i < m_Vertices.size(); ++i )
  //   std::cerr << std::fixed << std::setprecision(2)
  // 	      << "v (" << std::setw(6) << m_Vertices[i].x
  // 	      << " / " << std::setw(6) << m_Vertices[i].y
  // 	      << " / " << std::setw(6) << m_Vertices[i].z << "), "
  // 	      << "t (" << std::setw(6) << m_TexCoords[i].x
  // 	      << " / " << std::setw(6) << m_TexCoords[i].y << ")"
  // 	      << std::endl;

  // for ( std::vector< sf::Vector3f >::iterator itr = m_Vertices.begin();
  // 	itr != m_Vertices.end();
  //       ++itr )
  //   std::cerr << "v :" << itr->x << " / " << itr->y << " / " << itr->z << std::endl;

  // std::cerr << std::endl;

  // for ( std::vector< sf::Vector2f >::iterator itr = m_TexCoords.begin();
  // 	itr != m_TexCoords.end();
  //       ++itr )
  //   std::cerr << "t :" << itr->x << " / " << itr->y << std::endl;

  // std::cerr << std::endl;
  // std::cerr << std::endl;

  // static std::vector< sf::Vector3f >*	s_Vertices;
  // static std::vector< sf::Vector3f >*	s_Normales;
  // static std::vector< sf::Vector2f >*	s_TexCoords;
  // static std::vector< unsigned int >*	s_Indices;
  // static std::vector< unsigned int >*	s_Indices2;
  // static float				s_radius;

  set_treeSearcher.clear();

  Setup();
}


GL_Geometry*	GL_Geometry::CreateSphere( float radius, unsigned int quality )
{
  GL_Geometry*	pGeom = NULL;

  if (radius > 0.0f)
    {
      pGeom = new GL_Geometry(0);

      pGeom->MakeSphere( radius, quality );
    }

  return (pGeom);
}
