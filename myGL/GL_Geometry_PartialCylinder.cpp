

#include "GL_Geometry.hpp"

#include <cmath>


void	GL_Geometry::MakePartialCylinder( float length,
					  float startAngle, float sweepAngle,
					  float radius_top_in, float radius_top_out,
					  float radius_bottom_in, float radius_bottom_out,
					  unsigned int quality )
{
  m_Primitives.clear();
  m_Vertices.clear();
  m_Normales.clear();
  m_Prim_index.clear();
  m_Norm_index.clear();

  if ( length > 0.0f && sweepAngle > 0.0f &&
       radius_top_in >= 0.0f && radius_top_out > 0.0f &&
       radius_bottom_in >= 0.0f && radius_bottom_out > 0.0f &&
       quality > 1 )
    {

      if (sweepAngle == 360.0f)
	{
	  MakeCylinder( length,
			radius_top_in, radius_top_out,
			radius_bottom_in, radius_bottom_out,
			quality );
	}

      else
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
		indices_size += 4; // start + end
	      }
	    else
	      {
		indices_size += quality; // top
		indices_size += quality; // bottom
		indices_size += 4; // start + end
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

	  { // body

	    length *= 0.5f;

	    startAngle *= M_PI / 180.0f;
	    sweepAngle *= M_PI / 180.0f;

	    float	angle = float( sweepAngle) / float(quality);
	    float	sin_angle = sinf(angle);
	    float	cos_angle = cosf(angle);

	    float	nx = cosf( startAngle );
	    float	ny = sinf( startAngle );

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


	    if ( radius_top_in > 0.0f &&
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

	  } // /body


	  if ( radius_top_in > 0.0f || radius_bottom_in > 0.0f )
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



	  if ( radius_top_in > 0.0f || radius_bottom_in > 0.0f )
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





	  // if (sweepAngle != 360.0f)
	  {

	    if ( radius_top_in > 0.0f &&
		 radius_bottom_in > 0.0f )
	      {
		float	start_nx = cosf( startAngle - M_PI_2 );
		float	start_ny = sinf( startAngle - M_PI_2 );

		float	end_nx = cosf( startAngle + sweepAngle + M_PI_2 );
		float	end_ny = sinf( startAngle + sweepAngle + M_PI_2 );


		unsigned int	start_nidx = m_Normales.size();
		m_Normales.push_back( sf::Vector3f(start_nx, start_ny, 0) );

		unsigned int	end_nidx = m_Normales.size();
		m_Normales.push_back( sf::Vector3f(end_nx, end_ny, 0) );


		unsigned int	vertices_size = m_Vertices.size();
		unsigned int	half_vertices_size = vertices_size / 2;

		{ // start

		  unsigned int	top_out = 0;
		  unsigned int	top_in = half_vertices_size;

		  unsigned int	bottom_out = top_out + 1;
		  unsigned int	bottom_in = top_in + 1;


		  // m_Primitives.push_back( 0 );

		  m_Prim_index.push_back( top_out );
		  m_Prim_index.push_back( bottom_in );
		  m_Prim_index.push_back( bottom_out );

		  m_Norm_index.push_back( start_nidx );
		  m_Norm_index.push_back( start_nidx );
		  m_Norm_index.push_back( start_nidx );


		  // m_Primitives.push_back( 0 );

		  m_Prim_index.push_back( top_out );
		  m_Prim_index.push_back( top_in );
		  m_Prim_index.push_back( bottom_in );

		  m_Norm_index.push_back( start_nidx );
		  m_Norm_index.push_back( start_nidx );
		  m_Norm_index.push_back( start_nidx );

		} // /start

		{ // end

		  unsigned int	top_out = half_vertices_size - 2;
		  unsigned int	top_in = vertices_size - 2;

		  unsigned int	bottom_out = top_out + 1;
		  unsigned int	bottom_in = top_in + 1;


		  // m_Primitives.push_back( 0 );

		  m_Prim_index.push_back( top_out );
		  m_Prim_index.push_back( bottom_out );
		  m_Prim_index.push_back( bottom_in );

		  m_Norm_index.push_back( end_nidx );
		  m_Norm_index.push_back( end_nidx );
		  m_Norm_index.push_back( end_nidx );


		  // m_Primitives.push_back( 0 );

		  m_Prim_index.push_back( top_out );
		  m_Prim_index.push_back( bottom_in );
		  m_Prim_index.push_back( top_in );

		  m_Norm_index.push_back( end_nidx );
		  m_Norm_index.push_back( end_nidx );
		  m_Norm_index.push_back( end_nidx );

		} // /end

	      }

	    else
	      {
		float	start_nx = cosf( startAngle - M_PI_2 );
		float	start_ny = sinf( startAngle - M_PI_2 );

		float	end_nx = cosf( startAngle + sweepAngle );
		float	end_ny = sinf( startAngle + sweepAngle );

		unsigned int	start_nidx = m_Normales.size();
		m_Normales.push_back( sf::Vector3f(start_nx, start_ny, 0) );

		unsigned int	end_nidx = m_Normales.size();
		m_Normales.push_back( sf::Vector3f(end_nx, end_ny, 0) );


		unsigned int	vidx_bottom = m_Vertices.size() - 1;
		unsigned int	vidx_top = vidx_bottom - 1;

		unsigned int	vertices_size = m_Vertices.size();

		{ // start

		  unsigned int	top_out = 0;
		  unsigned int	top_in = vidx_top;

		  unsigned int	bottom_out = top_out + 1;
		  unsigned int	bottom_in = vidx_bottom;


		  // m_Primitives.push_back( 0 );

		  m_Prim_index.push_back( top_out );
		  m_Prim_index.push_back( bottom_in );
		  m_Prim_index.push_back( bottom_out );

		  m_Norm_index.push_back( start_nidx );
		  m_Norm_index.push_back( start_nidx );
		  m_Norm_index.push_back( start_nidx );


		  // m_Primitives.push_back( 0 );

		  m_Prim_index.push_back( top_out );
		  m_Prim_index.push_back( top_in );
		  m_Prim_index.push_back( bottom_in );

		  m_Norm_index.push_back( start_nidx );
		  m_Norm_index.push_back( start_nidx );
		  m_Norm_index.push_back( start_nidx );

		} // /start

		{ // end

		  unsigned int	top_out = vertices_size - 4;
		  unsigned int	top_in = vidx_top;

		  unsigned int	bottom_out = top_out + 1;
		  unsigned int	bottom_in = vidx_bottom;


		  // m_Primitives.push_back( 0 );

		  m_Prim_index.push_back( top_out );
		  m_Prim_index.push_back( bottom_out );
		  m_Prim_index.push_back( bottom_in );

		  m_Norm_index.push_back( end_nidx );
		  m_Norm_index.push_back( end_nidx );
		  m_Norm_index.push_back( end_nidx );


		  // m_Primitives.push_back( 0 );

		  m_Prim_index.push_back( top_out );
		  m_Prim_index.push_back( bottom_in );
		  m_Prim_index.push_back( top_in );

		  m_Norm_index.push_back( end_nidx );
		  m_Norm_index.push_back( end_nidx );
		  m_Norm_index.push_back( end_nidx );

		} // /end

	      }

	  } // if (sweepAngle != 360.0f)

	  Setup();

	}

    }
}


GL_Geometry*	GL_Geometry::CreatePartialCylinder( float length,
						    float startAngle, float sweepAngle,
						    float radius_top_in, float radius_top_out,
						    float radius_bottom_in, float radius_bottom_out,
						    unsigned int quality )
{
  GL_Geometry*	pGeom = NULL;

  if ( length > 0.0f && sweepAngle > 0.0f &&
       radius_top_in >= 0.0f && radius_top_out > 0.0f &&
       radius_bottom_in >= 0.0f && radius_bottom_out > 0.0f &&
       quality > 1 )
    {
      pGeom = new GL_Geometry(0);
      pGeom->CreatePartialCylinder( length,
				    startAngle, sweepAngle,
				    radius_top_in, radius_top_out,
				    radius_bottom_in, radius_bottom_out,
				    quality );
    }

  return (pGeom);
}
