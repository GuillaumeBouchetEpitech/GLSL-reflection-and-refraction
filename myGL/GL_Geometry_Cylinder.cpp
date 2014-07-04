

#include "GL_Geometry.hpp"

#include <cmath>


void	GL_Geometry::MakeCylinder( float length,
				   float radius_top_in, float radius_top_out,
				   float radius_bottom_in, float radius_bottom_out,
				   unsigned int quality )
{
  m_Primitives.clear();
  m_Vertices.clear();
  m_Normales.clear();
  m_Prim_index.clear();
  m_Norm_index.clear();

  if ( length > 0.0f &&
       radius_top_in >= 0.0f && radius_top_out > 0.0f &&
       radius_bottom_in >= 0.0f && radius_bottom_out > 0.0f &&
       quality > 2 )
    {

      {
	unsigned int	vertices_size = quality * 2; // body_out

	if ( radius_top_in > 0.0f || radius_bottom_in > 0.0f )
	  vertices_size *= 2; // body_in
	else
	  vertices_size += 2; // centers

	unsigned int	indices_size = quality * 2; // body_out;

	if ( radius_top_in > 0.0f || radius_bottom_in > 0.0f )
	  {
	    indices_size += quality * 2; // body_in
	    indices_size += quality * 2; // top
	    indices_size += quality * 2; // bottom
	  }
	else
	  {
	    indices_size += quality; // top
	    indices_size += quality; // bottom
	  }


	// m_Vertices.clear();
	// m_Normales.clear();
	// m_Prim_index.clear();
	// m_Norm_index.clear();
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

      { // body

	length *= 0.5f;
	float	angle = float(M_PI * 2.0) / float(quality);
	float	sin_angle = sinf(angle);
	float	cos_angle = cosf(angle);

	float	nx = 1;
	float	ny = 0;

	for (unsigned int i = 0; i <= quality; ++i)
	  {
	    m_Normales.push_back( sf::Vector3f(nx, ny, 0) );
	    m_Vertices.push_back( sf::Vector3f( nx * radius_top_out, ny * radius_top_out, length ) );
	    m_Vertices.push_back( sf::Vector3f( nx * radius_bottom_out, ny * radius_bottom_out, -length ) );

	    if (i > 0)
	      {
		unsigned int	curr_up = i * 2;
		unsigned int	curr_do = curr_up + 1;

		unsigned int	prev_up = curr_up - 2;
		unsigned int	prev_do = prev_up + 1;

		unsigned int	curr_norm = i;
		unsigned int	prev_norm = curr_norm - 1;

		// m_Primitives.push_back( 0 );

		m_Prim_index.push_back( curr_do);
		m_Prim_index.push_back( curr_up);
		m_Prim_index.push_back( prev_do);

		m_Norm_index.push_back( curr_norm );
		m_Norm_index.push_back( curr_norm );
		m_Norm_index.push_back( prev_norm );


		// m_Primitives.push_back( 0 );

		m_Prim_index.push_back( curr_up);
		m_Prim_index.push_back( prev_up);
		m_Prim_index.push_back( prev_do);

		m_Norm_index.push_back( curr_norm );
		m_Norm_index.push_back( prev_norm );
		m_Norm_index.push_back( prev_norm );
	      }

	    // rotate nx,ny
	    float	tmp = cos_angle * nx - sin_angle * ny;
	    ny = sin_angle * nx + cos_angle * ny;
	    nx = tmp;
	  }


	if ( radius_top_in > 0.0f ||
	     radius_bottom_in > 0.0f )
	  {
	    unsigned int	norm_size = m_Normales.size();
	    unsigned int	vertices_size = m_Vertices.size();

	    for (unsigned int i = 0; i <= quality; ++i)
	      {
		nx = m_Normales[i].x;
		ny = m_Normales[i].y;

		m_Normales.push_back( sf::Vector3f(-nx, -ny, 0) );

		m_Vertices.push_back( sf::Vector3f( nx * radius_top_in, ny * radius_top_in, length ) );
		m_Vertices.push_back( sf::Vector3f( nx * radius_bottom_in, ny * radius_bottom_in, -length ) );

		if (i > 0)
		  {
		    unsigned int	curr_up = vertices_size + i * 2;
		    unsigned int	curr_do = curr_up + 1;

		    unsigned int	prev_up = curr_up - 2;
		    unsigned int	prev_do = prev_up + 1;

		    unsigned int	curr_norm = norm_size + i;
		    unsigned int	prev_norm = curr_norm - 1;


		    // m_Primitives.push_back( 0 );

		    m_Prim_index.push_back( curr_do);
		    m_Prim_index.push_back( prev_do);
		    m_Prim_index.push_back( curr_up);

		    m_Norm_index.push_back( curr_norm );
		    m_Norm_index.push_back( prev_norm );
		    m_Norm_index.push_back( curr_norm );


		    // m_Primitives.push_back( 0 );

		    m_Prim_index.push_back( curr_up);
		    m_Prim_index.push_back( prev_do);
		    m_Prim_index.push_back( prev_up);

		    m_Norm_index.push_back( curr_norm );
		    m_Norm_index.push_back( prev_norm );
		    m_Norm_index.push_back( prev_norm );
		  }

	      }
	  }

      } // body


      if ( radius_top_in > 0.0f ||
	   radius_bottom_in > 0.0f )
	{
	  unsigned int	nidx = m_Normales.size();
	  m_Normales.push_back( sf::Vector3f(0, 0, 1) );

	  unsigned int	half_vertices_size = m_Vertices.size() / 2;

	  for (unsigned int i = 1; i <= quality; ++i)
	    {
	      unsigned int	curr_out = i * 2;
	      unsigned int	curr_in = curr_out + half_vertices_size;

	      unsigned int	prev_out = i * 2 - 2;
	      unsigned int	prev_in = prev_out + half_vertices_size;


	      // m_Primitives.push_back( 0 );

	      m_Prim_index.push_back( curr_out );
	      m_Prim_index.push_back( prev_in );
	      m_Prim_index.push_back( prev_out );

	      m_Norm_index.push_back( nidx );
	      m_Norm_index.push_back( nidx );
	      m_Norm_index.push_back( nidx );


	      // m_Primitives.push_back( 0 );

	      m_Prim_index.push_back( curr_out );
	      m_Prim_index.push_back( curr_in );
	      m_Prim_index.push_back( prev_in );

	      m_Norm_index.push_back( nidx );
	      m_Norm_index.push_back( nidx );
	      m_Norm_index.push_back( nidx );

	    }
	}

      else
	{
	  unsigned int	nidx = m_Normales.size();
	  unsigned int	vidx = m_Vertices.size();
	  m_Normales.push_back( sf::Vector3f(0, 0, 1) );
	  m_Vertices.push_back( sf::Vector3f( 0, 0, length ) );

	  for (unsigned int i = 0; i < quality; ++i)
	    {
	      // m_Primitives.push_back( 0 );

	      m_Prim_index.push_back( vidx );
	      m_Prim_index.push_back( i * 2 );
	      m_Prim_index.push_back( i * 2 + 2 );

	      m_Norm_index.push_back( nidx );
	      m_Norm_index.push_back( nidx );
	      m_Norm_index.push_back( nidx );
	    }

	}



      if ( radius_top_in > 0.0f ||
	   radius_bottom_in > 0.0f )
	{
	  unsigned int	nidx = m_Normales.size();
	  m_Normales.push_back( sf::Vector3f(0, 0, -1) );

	  unsigned int	half_vertices_size = m_Vertices.size() / 2;

	  for (unsigned int i = 1; i <= quality; ++i)
	    {
	      unsigned int	curr_out = i * 2 - 1;
	      unsigned int	curr_in = curr_out + half_vertices_size;

	      unsigned int	prev_out = i * 2 + 1;
	      unsigned int	prev_in = prev_out + half_vertices_size;


	      // m_Primitives.push_back( 0 );

	      m_Prim_index.push_back( curr_out );
	      m_Prim_index.push_back( prev_in );
	      m_Prim_index.push_back( prev_out );

	      m_Norm_index.push_back( nidx );
	      m_Norm_index.push_back( nidx );
	      m_Norm_index.push_back( nidx );


	      // m_Primitives.push_back( 0 );

	      m_Prim_index.push_back( curr_out );
	      m_Prim_index.push_back( curr_in );
	      m_Prim_index.push_back( prev_in );

	      m_Norm_index.push_back( nidx );
	      m_Norm_index.push_back( nidx );
	      m_Norm_index.push_back( nidx );
	    }
	}

      else
	{
	  unsigned int	nidx = m_Normales.size();
	  unsigned int	vidx = m_Vertices.size();
	  m_Normales.push_back( sf::Vector3f(0, 0, -1) );
	  m_Vertices.push_back( sf::Vector3f( 0, 0, -length ) );

	  for (unsigned int i = 0; i < quality; ++i)
	    {
	      // m_Primitives.push_back( 0 );

	      m_Prim_index.push_back( vidx );
	      m_Prim_index.push_back( i * 2 + 3 );
	      m_Prim_index.push_back( i * 2 + 1 );

	      m_Norm_index.push_back( nidx );
	      m_Norm_index.push_back( nidx );
	      m_Norm_index.push_back( nidx );
	    }

	}

      Setup();
    }
}


GL_Geometry*	GL_Geometry::CreateCylinder( float length,
					     float radius_top_in, float radius_top_out,
					     float radius_bottom_in, float radius_bottom_out,
					     unsigned int quality )
{
  GL_Geometry*	pGeom = NULL;

  if ( length > 0.0f &&
       radius_top_in >= 0.0f && radius_top_out > 0.0f &&
       radius_bottom_in >= 0.0f && radius_bottom_out > 0.0f &&
       quality > 2 )
    {
      pGeom = new GL_Geometry(0);

      pGeom->MakeCylinder( length,
			   radius_top_in, radius_top_out,
			   radius_bottom_in, radius_bottom_out,
			   quality );
    }

  return (pGeom);
}
