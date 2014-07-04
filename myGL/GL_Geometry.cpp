

#include "GL_Geometry.hpp"

#include "GL_OBJLoader.hpp"
#include "GL_Shadow.hpp"

#include <iostream>


GL_Geometry::GL_Geometry(unsigned int reserved_size /*= 0*/)

  // : m_vbo_id(0)

  : m_vbo_computed(false)//,
    // m_vbo_indices_id(0),
    // m_vbo_normals_id(0),
    // m_vbo_vertices_id(0),
    // m_vbo_texcoords_id(0)

{
  m_Vertices.reserve( reserved_size > 0 ? reserved_size : 0 );
  m_Normales.reserve( reserved_size > 0 ? reserved_size : 0 );
}

GL_Geometry::~GL_Geometry()
{
  DestroyVBO();
  // if (m_vbo_id != 0)
  //   glDeleteBuffers(1, &m_vbo_id);
}



void	GL_Geometry::ComputeVBO()
{
  if (m_vbo_computed)
    DestroyVBO();

  memset( m_vbo_ids, 0, eIdsCount * sizeof(*m_vbo_ids) );

  {
    glGenBuffers( eIdsCount, m_vbo_ids );

    ///

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_vbo_ids[ eIndices ] );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER,
		  // m_Prim_index.size() * std::vector< unsigned int >::size_type,
		  m_Prim_index.size() * sizeof( std::vector< unsigned int >::value_type ),
		  (GLuint*) &(m_Prim_index[0]),
		  GL_STATIC_DRAW );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

    ///

    glBindBuffer( GL_ARRAY_BUFFER, m_vbo_ids[ eVertices ] );
    glBufferData( GL_ARRAY_BUFFER,
		  m_Vertices.size() * sizeof( std::vector< sf::Vector3f >::value_type ),
		  (GLfloat*) &(m_Vertices[0]),
		  GL_STATIC_DRAW );

    glBindBuffer( GL_ARRAY_BUFFER, m_vbo_ids[ eNormals ] );
    glBufferData( GL_ARRAY_BUFFER,
		  m_Normales.size() * sizeof( std::vector< sf::Vector3f >::value_type ),
		  (GLfloat*) &(m_Normales[0]),
		  GL_STATIC_DRAW );

    glBindBuffer(GL_ARRAY_BUFFER, 0);


    // glBindBuffer( GL_ARRAY_BUFFER, m_vbo_ids[ eTexcoords ] );
    // glBufferData( GL_ARRAY_BUFFER,
    // 		m_TexCoords.size() * m_Vertices::size_type,
    // 		(GLfloat*) &(m_TexCoords[0]),
    // 		GL_STATIC_DRAW );
  }

  m_vbo_computed = true;
}

void	GL_Geometry::DestroyVBO()
{
  if (m_vbo_computed)
    {

      glDeleteBuffers( eIdsCount, m_vbo_ids );

      m_vbo_computed = false;
    }
}



void	GL_Geometry::Setup()
{

  { // old Setup

    // // if (m_vbo_id == 0)
    // // {
    // m_vbo_Vertices.clear();
    // m_vbo_Vertices.resize(m_Primitives.size() * 3);

    // // // if (m_vbo_id)
    // // if (m_vbo_computed)
    // //   {
    // // 	std::cerr << "delete vbo !" << std::endl;

    // // 	glDeleteBuffers(1, &m_vbo_id);
    // // 	m_vbo_id = 0;
    // //   }

  } // /old Setup

  ///////////////////////////////////////////////////////////////

  { // proto Setup

    std::vector< sf::Vector3f >	Vertices_tmp;
    std::vector< sf::Vector3f >	Normales_tmp;

    unsigned int	prim_nbr = m_Primitives.size() * 3;

    Vertices_tmp.reserve( prim_nbr );
    Normales_tmp.reserve( prim_nbr );

    for (unsigned int i = 0; i < prim_nbr; ++i)
      {
    	unsigned int	index = i;

    	unsigned int	vert = m_Prim_index[ index ];
    	unsigned int	norm = m_Norm_index[ index ];

    	Vertices_tmp.push_back( m_Vertices[ vert ] );
    	Normales_tmp.push_back( m_Normales[ norm ] );
      }

    m_Vertices.swap( Vertices_tmp );
    m_Normales.swap( Normales_tmp );


  } // /proto Setup

  ///////////////////////////////////////////////////////////////

  /**
  for (unsigned int i = 0; i < m_Primitives.size(); ++i)
    {
      for (unsigned int k = 0; k < 3; ++k)
	{
	  unsigned int	index = i * 3 + k;

	  unsigned int	vert = m_Prim_index[ index ];
	  unsigned int	norm = m_Norm_index[ index ];

	  unsigned int	texc = 0;

	  if (!m_TexC_index.empty())
	    texc = m_TexC_index[ index ];

	  t_Vertex*	curr = &(m_vbo_Vertices[ index ]);

	  curr->x = (m_Vertices)[ vert ].x;
	  curr->y = (m_Vertices)[ vert ].y;
	  curr->z = (m_Vertices)[ vert ].z;

	  curr->nx = (m_Normales)[ norm ].x;
	  curr->ny = (m_Normales)[ norm ].y;
	  curr->nz = (m_Normales)[ norm ].z;

	  curr->r = 1.0f;
	  curr->g = 1.0f;
	  curr->b = 1.0f;

	  if (!m_TexC_index.empty())
	    {
	      curr->tx = (m_TexCoords)[ texc ].x;
	      curr->ty = (m_TexCoords)[ texc ].y;
	    }

	  else if (!m_TexCoords.empty())
	    {
	      curr->tx = (m_TexCoords)[ vert ].x;
	      curr->ty = (m_TexCoords)[ vert ].y;
	    }

	  // // float	color = 0.66f + (k + 1) * 0.11f;
	  // float	color = (k + 1) * 0.33f;
	  // curr->r = color;
	  // curr->g = color;
	  // curr->b = color;
	  // // curr->g = 0;
	  // // curr->b = 0;
	}
    }
  //*/

  // int	size = m_vbo_Vertices.size() * sizeof(t_Vertex);

  // glGenBuffers(1, &m_vbo_id);
  // glBindBuffer(GL_ARRAY_BUFFER, m_vbo_id);

  // glBufferData(GL_ARRAY_BUFFER, size, &(m_vbo_Vertices[0]), GL_STATIC_DRAW);

  // glBindBuffer(GL_ARRAY_BUFFER, 0);

  // }

}

void	GL_Geometry::Render(bool use_vbo, bool use_backLine)
{
  // if (!m_vbo_Vertices.empty())
    {

      if (use_vbo && !m_vbo_computed)
	ComputeVBO();

      // if (use_vbo)
      // 	{
      // 	  if (!m_vbo_id)
      // 	    {

      // 	      std::cerr << "create vbo" << std::endl;

      // 	      glGenBuffers(1, &m_vbo_id);
      // 	      glBindBuffer(GL_ARRAY_BUFFER, m_vbo_id);

      // 	      glBufferData( GL_ARRAY_BUFFER,
      // 			    m_vbo_Vertices.size() * sizeof(t_Vertex),
      // 			    &(m_vbo_Vertices[0]), GL_STATIC_DRAW );
      // 	    }
      // 	  else
      // 	    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_id);
      // 	}


      glEnableClientState(GL_VERTEX_ARRAY);
      glEnableClientState(GL_NORMAL_ARRAY);
      // glEnableClientState(GL_COLOR_ARRAY);

      // if (use_vbo)
      if (use_vbo && m_vbo_computed)
	{

	  glBindBuffer( GL_ARRAY_BUFFER, m_vbo_ids[ eVertices ] );
	  glVertexPointer( 3, GL_FLOAT, 0, NULL );

	  glBindBuffer( GL_ARRAY_BUFFER, m_vbo_ids[ eNormals ] );
	  glNormalPointer( GL_FLOAT, 0, NULL );

	  // glVertexPointer( 3, GL_FLOAT, sizeof(t_Vertex), NULL );
	  // glNormalPointer( GL_FLOAT, sizeof(t_Vertex), (char*)NULL + 3 * sizeof(float) );
	  // // glColorPointer( 3, GL_FLOAT, sizeof(t_Vertex), (char*)NULL + 6 * sizeof(float) );
	}
      else
	{
	  glVertexPointer(3, GL_FLOAT, 0, &(m_Vertices[0].x));
	  glNormalPointer(GL_FLOAT, 0, &(m_Normales[0].x));

	  // glVertexPointer(3, GL_FLOAT, sizeof(t_Vertex), &(m_vbo_Vertices[0].x));
	  // glNormalPointer(GL_FLOAT, sizeof(t_Vertex), &(m_vbo_Vertices[0].nx));
	  // // glColorPointer(3, GL_FLOAT, sizeof(t_Vertex), &(m_vbo_Vertices[0].r));
	}


      // if (!use_vbo && !m_TexCoords.empty())
      if (false)
	{

	  glEnable( GL_TEXTURE_2D );

	  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	  glTexCoordPointer( 2, GL_FLOAT, 0, &(m_TexCoords[0].x));

	  // // glTexCoordPointer( 2, GL_FLOAT, 0, &(m_TexCoords[0].x) );
	  // glTexCoordPointer( 2, GL_FLOAT, sizeof(t_Vertex), &(m_vbo_Vertices[0].tx));
	}

      // glEnable(GL_CULL_FACE);
      // glCullFace(GL_BACK);

      // 	    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      // 	    glPolygonMode(GL_FRONT, GL_LINE);

      // glDrawArrays(GL_TRIANGLES, 0, m_vbo_Vertices.size());

      // glDrawElements( GL_TRIANGLES, m_Primitives.size() * 3,
      // 		      GL_UNSIGNED_INT, &(m_Prim_index[0]) );


      if (use_vbo && m_vbo_computed)
	// {
	  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_vbo_ids[ eIndices ] );

      // 	  // glDrawElements( GL_TRIANGLES, m_Prim_index.size(), GL_UNSIGNED_INT, NULL );
      // 	  glDrawArrays(GL_TRIANGLES, 0, m_Vertices.size());
      // 	}
      // else
      // 	// glDrawElements( GL_TRIANGLES, m_Prim_index.size(), GL_UNSIGNED_INT, &(m_Prim_index[0]) );
      glDrawArrays(GL_TRIANGLES, 0, m_Vertices.size());


	    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


      glDisableClientState(GL_NORMAL_ARRAY);

      // if (!use_vbo && !m_TexCoords.empty())
      if (false)
	{
	  glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	  glDisable( GL_TEXTURE_2D );
	}

      // glDisableClientState(GL_COLOR_ARRAY);


      if (use_backLine)
	{

	  {
	    glPushAttrib(GL_CURRENT_BIT);

	    glEnable(GL_CULL_FACE);

	    glDisable(GL_LIGHTING);
	    glDisable(GL_TEXTURE_1D);
	    // glColor3ub(0,0,0);
	    glColor4f(1,1,1, 1);

	    glPolygonMode(GL_BACK, GL_LINE);

	    glCullFace(GL_FRONT);
	    glDepthFunc(GL_LEQUAL);
	  }

	  // glDrawArrays(GL_TRIANGLES, 0, m_vbo_Vertices.size());
	  // glDrawArrays(GL_TRIANGLES, 0, m_Prim_index.size());
	  // glDrawElements( GL_TRIANGLES, m_Prim_index.size(), GL_UNSIGNED_INT, NULL );


	  glDrawArrays(GL_TRIANGLES, 0, m_Vertices.size());
	  // if (use_vbo && m_vbo_computed)
	  //   glDrawElements( GL_TRIANGLES, m_Prim_index.size(), GL_UNSIGNED_INT, NULL );
	  // else
	  //   glDrawElements( GL_TRIANGLES, m_Prim_index.size(), GL_UNSIGNED_INT, &(m_Prim_index[0]) );


	  {
	    glDepthFunc(GL_LESS);
	    glCullFace(GL_BACK);

	    glDisable(GL_LINE_STIPPLE);

	    glPolygonMode(GL_BACK, GL_FILL);

	    glEnable(GL_LIGHTING);

	    glDisable(GL_CULL_FACE);

	    glPopAttrib();
	  }

	} // if (use_backLine)

      glDisableClientState(GL_VERTEX_ARRAY);


      // if (use_vbo)
      if (use_vbo && m_vbo_computed)
	{
	  glBindBuffer(GL_ARRAY_BUFFER, 0);
	  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	}

    }
}

