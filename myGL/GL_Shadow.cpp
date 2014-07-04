

#include "GL_Shadow.hpp"

#include "GL_Geometry.hpp"

#include <cmath>


////////////////////////////////////////////////////////////

void	GL_Shadow::CalcPlane(t_Triangle& plane, GL_Matrix* pMat)
{
  sf::Vector3f	v[3];

  for (int i = 0; i < 3; ++i)
    {
      v[ i ].x = m_pGeom->m_Vertices[ plane.p[ i ] ].x;
      v[ i ].y = m_pGeom->m_Vertices[ plane.p[ i ] ].y;
      v[ i ].z = m_pGeom->m_Vertices[ plane.p[ i ] ].z;


      if (pMat)
	v[ i ] = pMat->PreMult( v[ i ] );

    }

  plane.PlaneEq[0] = v[0].y * (v[1].z - v[2].z) + v[1].y * (v[2].z - v[0].z) + v[2].y * (v[0].z - v[1].z);
  plane.PlaneEq[1] = v[0].z * (v[1].x - v[2].x) + v[1].z * (v[2].x - v[0].x) + v[2].z * (v[0].x - v[1].x);
  plane.PlaneEq[2] = v[0].x * (v[1].y - v[2].y) + v[1].x * (v[2].y - v[0].y) + v[2].x * (v[0].y - v[1].y);
  plane.PlaneEq[3] = -( v[0].x * (v[1].y * v[2].z - v[2].y * v[1].z) +
			v[1].x * (v[2].y * v[0].z - v[0].y * v[2].z) +
			v[2].x * (v[0].y * v[1].z - v[1].y * v[0].z) );
}

////////////////////////////////////////////////////////////

GL_Shadow::GL_Shadow()
  : m_pGeom(NULL)
{
}

GL_Shadow::GL_Shadow(GL_Geometry& Geom)
{
  Initialize(&Geom);
}

////////////////////////////////////////////////////////////

void	GL_Shadow::Initialize(GL_Geometry* pGeom, GL_Matrix* pMat)
{
  m_pGeom = pGeom;

  m_Triangles.resize( pGeom->m_Prim_index.size() / 3 );


  for (unsigned int i = 0; i < m_pGeom->m_Prim_index.size() / 3; ++i)
    {
      m_Triangles[i].p[0] = m_pGeom->m_Prim_index[ i * 3 + 0 ];
      m_Triangles[i].p[1] = m_pGeom->m_Prim_index[ i * 3 + 1 ];
      m_Triangles[i].p[2] = m_pGeom->m_Prim_index[ i * 3 + 2 ];

      m_Triangles[i].n[0] = m_pGeom->m_Norm_index[ i * 3 + 0 ];
      m_Triangles[i].n[1] = m_pGeom->m_Norm_index[ i * 3 + 1 ];
      m_Triangles[i].n[2] = m_pGeom->m_Norm_index[ i * 3 + 2 ];

      CalcPlane( m_Triangles[i], pMat );
    }


  if (!m_Triangles.empty())
    {
      for (unsigned int i = 0; i < m_Triangles.size(); ++i)
	{
	  m_Triangles[i].neigh[0] = 0;
	  m_Triangles[i].neigh[1] = 0;
	  m_Triangles[i].neigh[2] = 0;
	}

      for (unsigned int i = 0; i < m_Triangles.size() - 1; ++i)
	{

	  for (unsigned int j = i + 1; j < m_Triangles.size(); ++j)

	    if (i != j)
	      {
		unsigned int	nb_match = 0;

		int fni_tab[] = {0, 0};
		int fnj_tab[] = {0, 0};

		for (int ni = 0; ni < 3; ++ni)
		  for (int nj = 0; nj < 3; ++nj)

		    if (m_Triangles[ i ].p[ ni ] == m_Triangles[ j ].p[ nj ])
		      {
			if (nb_match < 2)
			  {
			    fni_tab[ nb_match ] = ni;
			    fnj_tab[ nb_match ] = nj;
			  }

			++nb_match;
		      }

		if (nb_match == 2)
		  {

		    int fni = 0;
		    int fnj = 0;

#define	D_WATHEVER(tab, v1, v2)	( (tab[0] == v1 && tab[1] == v2) ||	\
				  (tab[0] == v2 && tab[1] == v1) )

		    if (D_WATHEVER(fni_tab, 0, 1))
		      fni = 0;
		    else if (D_WATHEVER(fni_tab, 1, 2))
		      fni = 1;
		    else if (D_WATHEVER(fni_tab, 2, 0))
		      fni = 2;

		    if (D_WATHEVER(fnj_tab, 0, 1))
		      fnj = 0;
		    else if (D_WATHEVER(fnj_tab, 1, 2))
		      fnj = 1;
		    else if (D_WATHEVER(fnj_tab, 2, 0))
		      fnj = 2;

		    m_Triangles[i].neigh[ fni ] = j + 1;
		    m_Triangles[j].neigh[ fnj ] = i + 1;

#undef	D_WATHEVER

		  } // if (nb_match == 2)

	      } // if (i != j)

	} // for (unsigned int i = 0; i < nPlanes - 1; ++i)

    } // if (nPlanes > 0)


  m_Vertices.reserve( pGeom->m_Vertices.capacity() * 2 );

}

void	GL_Shadow::Setup(const sf::Vector3f& lightPos, GL_Matrix* pMat, float length)
{

#define	D_IS_VISIBLE				\
  ( (m_Triangles[ i ].PlaneEq[0] * lightPos.x +	\
     m_Triangles[ i ].PlaneEq[1] * lightPos.y +	\
     m_Triangles[ i ].PlaneEq[2] * lightPos.z +	\
     m_Triangles[ i ].PlaneEq[3] * 1.0f) > 0 )

  for (unsigned int i = 0; i < m_Triangles.size(); ++i)
    m_Triangles[ i ].visible = D_IS_VISIBLE;

#undef	D_IS_VISIBLE


  m_Vertices.clear();

  sf::Vector3f	dec1, dec2, dec3;

  for (unsigned int i = 0; i < m_Triangles.size(); ++i)
    {

      if (m_Triangles[ i ].visible)
	{

	  {

	    unsigned int	p1 = m_Triangles[ i ].p[ 0 ];
	    unsigned int	p2 = m_Triangles[ i ].p[ 1 ];
	    unsigned int	p3 = m_Triangles[ i ].p[ 2 ];

#define	D_SQ(v)		( (v) * (v) )
#define	D_LEN2(v3)	( D_SQ( (v3).x ) + D_SQ( (v3).y ) + D_SQ( (v3).z ) )
#define	D_LEN(v3)	( sqrtf( D_LEN2(v3) ) )
#define	D_NORMALIZE(v3)				\
	    {					\
	      float	len = D_LEN(v3);	\
	      (v3).x /= len;			\
	      (v3).y /= len;			\
	      (v3).z /= len;			\
	    }

	    dec1.x = m_pGeom->m_Vertices[ p1 ].x - lightPos.x;
	    dec1.y = m_pGeom->m_Vertices[ p1 ].y - lightPos.y;
	    dec1.z = m_pGeom->m_Vertices[ p1 ].z - lightPos.z;

	    dec2.x = m_pGeom->m_Vertices[ p2 ].x - lightPos.x;
	    dec2.y = m_pGeom->m_Vertices[ p2 ].y - lightPos.y;
	    dec2.z = m_pGeom->m_Vertices[ p2 ].z - lightPos.z;

	    dec3.x = m_pGeom->m_Vertices[ p3 ].x - lightPos.x;
	    dec3.y = m_pGeom->m_Vertices[ p3 ].y - lightPos.y;
	    dec3.z = m_pGeom->m_Vertices[ p3 ].z - lightPos.z;

	    D_NORMALIZE(dec1);
	    D_NORMALIZE(dec2);
	    D_NORMALIZE(dec3);

	    sf::Vector3f	v3;

#define	D_LENGTH_MIN	0.0f;
// #define	D_LENGTH_MIN	length / 2;
#define	D_LENGTH_MAX	length;

#define	D_PUSH_V3(pn, dn, ln)					\
	    v3.x = m_pGeom->m_Vertices[ pn ].x + dn.x * ln;	\
	    v3.y = m_pGeom->m_Vertices[ pn ].y + dn.y * ln;	\
	    v3.z = m_pGeom->m_Vertices[ pn ].z + dn.z * ln;	\
	    m_Vertices.push_back( ( (pMat) ? (v3) : (pMat->PreMult( v3) ) ) );
	    // m_Vertices.push_back( v3 );


	    // D_PUSH_V3( p1, dec1, D_LENGTH_MIN );
	    // D_PUSH_V3( p3, dec3, D_LENGTH_MIN );
	    // D_PUSH_V3( p2, dec2, D_LENGTH_MIN );

	    D_PUSH_V3( p1, dec1, D_LENGTH_MIN );
	    D_PUSH_V3( p2, dec2, D_LENGTH_MIN );
	    D_PUSH_V3( p3, dec3, D_LENGTH_MIN );

	    D_PUSH_V3( p1, dec1, D_LENGTH_MAX );
	    D_PUSH_V3( p3, dec3, D_LENGTH_MAX );
	    D_PUSH_V3( p2, dec2, D_LENGTH_MAX );


#undef	D_PUSH_V3

#undef	D_LENGTH_MIN
#undef	D_LENGTH_MAX
	  }



	  for (int n = 0; n < 3; ++n)
	    {
	      unsigned int	k = m_Triangles[ i ].neigh[ n ];

	      if (!k)
		continue;

	      if (!m_Triangles[ k - 1 ].visible)
		{

		  int j = n;
		  // for (int j = 0; j < 3; ++j)
		  {
		    unsigned int	p1 = m_Triangles[ i ].p[ j ];
		    unsigned int	p2 = m_Triangles[ i ].p[ (j + 1) % 3 ];

		    sf::Vector3f	v3, dec1, dec2;

		    dec1.x = m_pGeom->m_Vertices[ p1 ].x - lightPos.x;
		    dec1.y = m_pGeom->m_Vertices[ p1 ].y - lightPos.y;
		    dec1.z = m_pGeom->m_Vertices[ p1 ].z - lightPos.z;

		    dec2.x = m_pGeom->m_Vertices[ p2 ].x - lightPos.x;
		    dec2.y = m_pGeom->m_Vertices[ p2 ].y - lightPos.y;
		    dec2.z = m_pGeom->m_Vertices[ p2 ].z - lightPos.z;

		    D_NORMALIZE(dec1);
		    D_NORMALIZE(dec2);

#undef	D_NORMALIZE
#undef	D_LEN
#undef	D_LEN2
#undef	D_SQ


#define	D_LENGTH_MIN	0.0f;
// #define	D_LENGTH_MIN	length / 2;
#define	D_LENGTH_MAX	length;

#define	D_PUSH_V3(pn, dn, ln)						\
		    v3.x = m_pGeom->m_Vertices[ pn ].x + dn.x * ln;	\
		    v3.y = m_pGeom->m_Vertices[ pn ].y + dn.y * ln;	\
		    v3.z = m_pGeom->m_Vertices[ pn ].z + dn.z * ln;	\
		    m_Vertices.push_back( ( (pMat) ? (v3) : (pMat->PreMult( v3) ) ) );
		    // m_Vertices.push_back( v3 );


		    D_PUSH_V3( p1, dec1, D_LENGTH_MIN );
		    D_PUSH_V3( p2, dec2, D_LENGTH_MAX );
		    D_PUSH_V3( p2, dec2, D_LENGTH_MIN );

		    D_PUSH_V3( p1, dec1, D_LENGTH_MIN );
		    D_PUSH_V3( p1, dec1, D_LENGTH_MAX );
		    D_PUSH_V3( p2, dec2, D_LENGTH_MAX );


#undef	D_PUSH_V3

#undef	D_LENGTH_MIN
#undef	D_LENGTH_MAX

		  }

		}

	    }

	}

    }


}

////////////////////////////////////////////////////////////

void	GL_Shadow::Render()
{
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, &(m_Vertices[0].x));

  glDrawArrays(GL_TRIANGLES, 0, m_Vertices.size());

  glDisableClientState(GL_VERTEX_ARRAY);
}

void	GL_Shadow::Cast()
{
  glDisable(GL_LIGHTING);

  glEnable(GL_DEPTH_TEST); // Activate the depth test
  glEnable(GL_CULL_FACE); // Activate the culling

  // glCullFace(GL_BACK);   // We are drawing front face

  glDisable(GL_TEXTURE_2D); // no texture here
  glDisable(GL_BLEND);   // no blending


  glClear(GL_STENCIL_BUFFER_BIT);
  glDepthMask(GL_FALSE);


  // We change the z-testing function to LESS, to avoid little bugs in shadow
  glDepthFunc(GL_LESS);

  glEnable(GL_STENCIL_TEST);
  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

  glStencilFunc(GL_ALWAYS, 1, 0xffffffff); // work
  // glStencilFunc(GL_LESS, 1, 0xffffffff);  // not working
  // glStencilFunc(GL_GREATER, 1, 0xffffffff);  // not working
  // glStencilFunc(GL_LEQUAL, 1, 0xffffffff);  // not working
  // glStencilFunc(GL_NOTEQUAL, 1, 0xffffffff);  // not working




  { // first pass, stencil operation decreases stencil value

    glCullFace(GL_FRONT); // We are drawing the back faces first
    glStencilOp(GL_KEEP, GL_INCR, GL_KEEP); // We increment if the depth test fails

    Render();

  } // first pass, stencil operation decreases stencil value



  // glStencilFunc(GL_LEQUAL, 1, 0xffffffff); // test



  { // second pass, stencil operation increases stencil value

    glCullFace(GL_BACK); // We are now drawing the front faces
    glStencilOp(GL_KEEP, GL_DECR, GL_KEEP); // We decrement if the depth test fails

    Render();

  } // second pass, stencil operation increases stencil value



  { //draw a shadowing rectangle covering the entire screen

    glFrontFace(GL_CCW);

    glColorMask(1, 1, 1, 1);

    // glColor4f(0.0f, 0.0f, 0.0f, 0.4f);
    // glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    // glColor4fv(color);
    glColor4f(0.0f, 1.0f, 1.0f, 0.5f);
    // glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
    // glColor4f(0.7f, 0.7f, 0.7f, 1.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glStencilFunc(GL_NOTEQUAL, 0, 0xffffffff);
    // glStencilFunc(GL_EQUAL, 0x0, 0xffffffff);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);



    /**/
    {
      glPushMatrix();
      {
	glLoadIdentity();

	glBegin(GL_TRIANGLE_STRIP);
	{
	  glVertex3f(-0.1f,  0.1f, -0.10f);
	  glVertex3f(-0.1f, -0.1f, -0.10f);
	  glVertex3f( 0.1f,  0.1f, -0.10f);
	  glVertex3f( 0.1f, -0.1f, -0.10f);
	}
	glEnd();
      }
      glPopMatrix();
    }
    //*/


    glDisable(GL_BLEND);

  } //draw a shadowing rectangle covering the entire screen



  glDepthFunc(GL_LEQUAL);
  glDepthMask(GL_TRUE);
  glDisable(GL_STENCIL_TEST);

}

