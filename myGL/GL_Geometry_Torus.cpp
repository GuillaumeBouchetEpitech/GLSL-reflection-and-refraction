

#include "GL_Geometry.hpp"

#include <cmath>


void	GL_Geometry::MakeTorus( float startAngle, float sweepAngle,
				float radius, float radius_in, float radius_out,
				unsigned int cyl_qual, unsigned int cyl_nbr )
{
  m_Primitives.clear();
  m_Vertices.clear();
  m_Normales.clear();
  m_Prim_index.clear();
  m_Norm_index.clear();

  if ( sweepAngle > 0.0f &&
       radius_in >= 0.0f && radius_out > 0.0f &&
       cyl_qual >= 3 && cyl_nbr >= 2 )
    {

      {
	unsigned int	vertices_size = cyl_qual * (cyl_nbr - 1); // body_out

	if ( radius_in > 0.0f )
	  vertices_size *= 2; // body_in
	else
	  vertices_size += 2; // centers

	unsigned int	indices_size = cyl_qual * 2 * (cyl_nbr - 1); // body_out;

	if ( radius_in > 0.0f )
	  {
	    indices_size += cyl_qual * 2 * (cyl_nbr - 1); // body_in

	    if ( sweepAngle != 360.0f )
	      {
		indices_size += cyl_qual * 2; // start
		indices_size += cyl_qual * 2; // end
	      }
	  }

	else if ( sweepAngle != 360.0f )
	  {
	    indices_size += cyl_qual; // start
	    indices_size += cyl_qual; // end
	  }


	// std::cerr << std::endl;
	// std::cerr << "ver : " << vertices_size << std::endl;
	// std::cerr << "idx : " << indices_size << std::endl;


	// m_Primitives.resize( indices_size, 0 );

	if (vertices_size <= m_Vertices.capacity())
	  m_Vertices.reserve( vertices_size );

	if (vertices_size <= m_Normales.capacity())
	  m_Normales.reserve( vertices_size );

	if (indices_size * 3 <= m_Prim_index.capacity())
	  m_Prim_index.reserve( indices_size * 3 );

	if (indices_size * 3 <= m_Norm_index.capacity())
	  m_Norm_index.reserve( indices_size * 3 );

      }


      float	twopi = 2.0f * (float)M_PI;
      float	twopi2 = sweepAngle * M_PI / 180.0f;


      { // body

	startAngle *= M_PI / 180.0f;

	for (unsigned int cylnbr_i = 0; cylnbr_i <= cyl_nbr; ++cylnbr_i)
	  {

	    float	t = cylnbr_i;
	    float	t_cos = cosf( t * twopi2 / cyl_nbr + startAngle );
	    float	t_sin = sinf( t * twopi2 / cyl_nbr + startAngle );

	    for (unsigned int cylqual_i = 0; cylqual_i < cyl_qual; ++cylqual_i)
	      {

		float	s = cylqual_i + 0.5f;
		float	s_cos = cosf( s * twopi / cyl_qual);
		float	s_sin = sinf( s * twopi / cyl_qual);


		m_Normales.push_back( sf::Vector3f( s_cos * t_cos,
						    s_cos * t_sin,
						    s_sin ) );

		m_Vertices.push_back( sf::Vector3f( (radius + radius_out * s_cos) * t_cos,
						    (radius + radius_out * s_cos) * t_sin,
						    radius_out * s_sin ) );


		if (radius_in > 0.0f)
		  {
		    m_Normales.push_back( sf::Vector3f( -s_cos * t_cos,
							-s_cos * t_sin,
							-s_sin ) );

		    m_Vertices.push_back( sf::Vector3f( (radius + radius_in * s_cos) * t_cos,
							(radius + radius_in * s_cos) * t_sin,
							radius_in * s_sin ) );
		  }

	      }

	    if (cylnbr_i > 0)
	      {

		if (radius_in > 0.0f)
		  {

		    unsigned int	cyl_dec = cyl_qual * 2;
		    unsigned int	curr_i = cylnbr_i * cyl_dec;
		    unsigned int	prev_i = (cylnbr_i - 1) * cyl_dec;

		    for (unsigned int i = 0; i < cyl_qual; ++i)
		      {

#define	D_CURR_INDEX(index)	( curr_i + ( (index) % cyl_dec) )
#define	D_PREV_INDEX(index)	( prev_i + ( (index) % cyl_dec) )

#define	D_PUSH_PRIM(index)					\
			m_Prim_index.push_back( index );	\
			m_Norm_index.push_back( index );

			//

			m_Primitives.push_back(0);

			D_PUSH_PRIM( D_PREV_INDEX(i * 2 + 2) );
			D_PUSH_PRIM( D_PREV_INDEX(i * 2 + 0) );
			D_PUSH_PRIM( D_CURR_INDEX(i * 2 + 2) );

			m_Primitives.push_back(0);

			D_PUSH_PRIM( D_PREV_INDEX(i * 2 + 0) );
			D_PUSH_PRIM( D_CURR_INDEX(i * 2 + 0) );
			D_PUSH_PRIM( D_CURR_INDEX(i * 2 + 2) );

			//

			m_Primitives.push_back(0);

			D_PUSH_PRIM( D_PREV_INDEX(i * 2 + 3) );
			D_PUSH_PRIM( D_CURR_INDEX(i * 2 + 3) );
			D_PUSH_PRIM( D_PREV_INDEX(i * 2 + 1) );

			m_Primitives.push_back(0);

			D_PUSH_PRIM( D_PREV_INDEX(i * 2 + 1) );
			D_PUSH_PRIM( D_CURR_INDEX(i * 2 + 3) );
			D_PUSH_PRIM( D_CURR_INDEX(i * 2 + 1) );

			//

#undef	D_PUSH_PRIM

#undef	D_PREV_INDEX
#undef	D_CURR_INDEX

		      }

		  } // if (radius_in > 0.0f)

		else
		  {

		    unsigned int	cyl_dec = cyl_qual;
		    unsigned int	curr_i = cylnbr_i * cyl_dec;
		    unsigned int	prev_i = (cylnbr_i - 1) * cyl_dec;

		    for (unsigned int i = 0; i <= cyl_qual; ++i)
		      {

#define	D_CURR_INDEX(index)	( curr_i + ( (index) % cyl_qual) )
#define	D_PREV_INDEX(index)	( prev_i + ( (index) % cyl_qual) )

#define	D_PUSH_PRIM(index)					\
			m_Prim_index.push_back( index );	\
			m_Norm_index.push_back( index );


			m_Primitives.push_back(0);

			D_PUSH_PRIM( D_PREV_INDEX( i + 1 ) );
			D_PUSH_PRIM( D_PREV_INDEX( i + 0 ) );
			D_PUSH_PRIM( D_CURR_INDEX( i + 1 ) );


			m_Primitives.push_back(0);

			D_PUSH_PRIM( D_PREV_INDEX(i + 0) );
			D_PUSH_PRIM( D_CURR_INDEX(i + 0) );
			D_PUSH_PRIM( D_CURR_INDEX(i + 1) );

#undef	D_PUSH_PRIM

#undef	D_PREV_INDEX
#undef	D_CURR_INDEX

		      }

		  } // else

	      } // if (cylnbr_i > 0)

	  }

      } // /body


      { // start

	if ( radius_in > 0.0f )
	  {

	    unsigned int	cyl_dec = cyl_qual * 2;
	    unsigned int	nidx = m_Normales.size();

	    m_Normales.push_back( sf::Vector3f( cosf( startAngle - M_PI_2 ),
						sinf( startAngle - M_PI_2 ),
						0 ) );


	    for (unsigned int i = 0; i < cyl_qual; ++i)
	      {
		m_Primitives.push_back( 0 );

		m_Primitives.push_back( 0 );


		m_Prim_index.push_back( (i * 2 + 0) % cyl_dec );
		m_Prim_index.push_back( (i * 2 + 2) % cyl_dec );
		m_Prim_index.push_back( (i * 2 + 1) % cyl_dec );

		m_Prim_index.push_back( (i * 2 + 2) % cyl_dec );
		m_Prim_index.push_back( (i * 2 + 3) % cyl_dec );
		m_Prim_index.push_back( (i * 2 + 1) % cyl_dec );


		m_Norm_index.push_back( nidx );
		m_Norm_index.push_back( nidx );
		m_Norm_index.push_back( nidx );

		m_Norm_index.push_back( nidx );
		m_Norm_index.push_back( nidx );
		m_Norm_index.push_back( nidx );
	      }
	  }

	else
	  {
	    unsigned int	nidx = m_Normales.size();
	    unsigned int	vidx = m_Vertices.size();

	    m_Normales.push_back( sf::Vector3f( cosf( startAngle - M_PI_2 ),
						sinf( startAngle - M_PI_2 ),
						0 ) );

	    m_Vertices.push_back( sf::Vector3f( radius * cosf( startAngle ),
						radius * sinf( startAngle ),
						0 ) );

	    for (unsigned int i = 0; i < cyl_qual; ++i)
	      {
		m_Primitives.push_back( 0 );

		m_Prim_index.push_back( i );
		m_Prim_index.push_back( (i + 1) % cyl_qual );
		m_Prim_index.push_back( vidx );

		m_Norm_index.push_back( nidx );
		m_Norm_index.push_back( nidx );
		m_Norm_index.push_back( nidx );
	      }

	  }

      } // /start


      { // end
	if ( radius_in > 0.0f )
	  {

	    unsigned int	cyl_dec = cyl_qual * 2;
	    unsigned int	nidx = m_Normales.size();

	    m_Normales.push_back( sf::Vector3f( cosf( startAngle + twopi2 + M_PI_2 ),
						sinf( startAngle + twopi2 + M_PI_2 ),
						0 ) );

	    unsigned int	index = m_Vertices.size() - cyl_dec;

	    for (unsigned int i = 0; i < cyl_qual; ++i)
	      {
		m_Primitives.push_back( 0 );

		m_Primitives.push_back( 0 );


		m_Prim_index.push_back( index + (i * 2 + 2) % cyl_dec );
		m_Prim_index.push_back( index + (i * 2 + 0) % cyl_dec );
		m_Prim_index.push_back( index + (i * 2 + 1) % cyl_dec );

		m_Prim_index.push_back( index + (i * 2 + 3) % cyl_dec );
		m_Prim_index.push_back( index + (i * 2 + 2) % cyl_dec );
		m_Prim_index.push_back( index + (i * 2 + 1) % cyl_dec );


		m_Norm_index.push_back( nidx );
		m_Norm_index.push_back( nidx );
		m_Norm_index.push_back( nidx );

		m_Norm_index.push_back( nidx );
		m_Norm_index.push_back( nidx );
		m_Norm_index.push_back( nidx );
	      }
	  }

	else
	  {
	    unsigned int	nidx = m_Normales.size();
	    unsigned int	vidx = m_Vertices.size();

	    float		twopi2 = sweepAngle * M_PI / 180.0f;

	    m_Normales.push_back( sf::Vector3f( cosf( startAngle + twopi2 + M_PI_2 ),
						sinf( startAngle + twopi2 + M_PI_2 ),
						0 ) );

	    m_Vertices.push_back( sf::Vector3f( radius * cosf( startAngle + twopi2 ),
						radius * sinf( startAngle + twopi2 ),
						0 ) );

	    unsigned int	index = vidx - 1 - cyl_qual;

	    for (unsigned int i = 0; i < cyl_qual; ++i)
	      {
		m_Primitives.push_back( 0 );

		m_Prim_index.push_back( index + (i + 1) % cyl_qual );
		m_Prim_index.push_back( index + i );
		m_Prim_index.push_back( vidx );

		m_Norm_index.push_back( nidx );
		m_Norm_index.push_back( nidx );
		m_Norm_index.push_back( nidx );
	      }

	  }

	//*/

      } // /end

      Setup();

    }
}


GL_Geometry*	GL_Geometry::CreateTorus( float startAngle, float sweepAngle,
					  float radius, float radius_in, float radius_out,
					  unsigned int cyl_qual, unsigned int cyl_nbr )
{
  GL_Geometry*	pGeom = NULL;

  if ( sweepAngle > 0.0f &&
       radius_in >= 0.0f && radius_out > 0.0f &&
       cyl_qual >= 3 && cyl_nbr >= 2 )
    {
      pGeom = new GL_Geometry(0);
      pGeom->MakeTorus( startAngle, sweepAngle,
			radius, radius_in, radius_out,
			cyl_qual, cyl_nbr );
    }

  return (pGeom);
}
