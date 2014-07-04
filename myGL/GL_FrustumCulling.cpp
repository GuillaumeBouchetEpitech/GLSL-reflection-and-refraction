

#include "GL_FrustumCulling.hpp"

#include "Header.hpp"


void	GL_FrustumCulling::NormalizePlane(float frustum[6][4], int side)
{
  float	magnitude = sqrtf( frustum[side][eA] * frustum[side][eA] +
			   frustum[side][eB] * frustum[side][eB] +
			   frustum[side][eC] * frustum[side][eC] );

  frustum[side][eA] /= magnitude;
  frustum[side][eB] /= magnitude;
  frustum[side][eC] /= magnitude;
  frustum[side][eD] /= magnitude;
}

void	GL_FrustumCulling::CalculateFrustum()
{
  float   proj[16];
  float   modl[16];

  glGetFloatv( GL_PROJECTION_MATRIX, proj );
  glGetFloatv( GL_MODELVIEW_MATRIX, modl );

  CalculateFrustum(proj, modl);
}

void	GL_FrustumCulling::CalculateFrustum(const GL_Matrix& mat_proj, const GL_Matrix& mat_modl)
{
  float   proj[16];
  float   modl[16];

  mat_proj.Get( proj );
  mat_modl.Get( modl );

  CalculateFrustum( proj, modl );
}

void	GL_FrustumCulling::CalculateFrustum(float* proj, float* modl)
{
  float   clip[16];

#define	D_SET_CLIP(X)							\
  clip[X * 4 + 0] = modl[X * 4]*proj[ 0] + modl[X * 4 + 1]*proj[ 4] + modl[X * 4 + 2]*proj[ 8] + modl[X * 4 + 3]*proj[12]; \
  clip[X * 4 + 1] = modl[X * 4]*proj[ 1] + modl[X * 4 + 1]*proj[ 5] + modl[X * 4 + 2]*proj[ 9] + modl[X * 4 + 3]*proj[13]; \
  clip[X * 4 + 2] = modl[X * 4]*proj[ 2] + modl[X * 4 + 1]*proj[ 6] + modl[X * 4 + 2]*proj[10] + modl[X * 4 + 3]*proj[14]; \
  clip[X * 4 + 3] = modl[X * 4]*proj[ 3] + modl[X * 4 + 1]*proj[ 7] + modl[X * 4 + 2]*proj[11] + modl[X * 4 + 3]*proj[15];

  D_SET_CLIP(0);
  D_SET_CLIP(1);
  D_SET_CLIP(2);
  D_SET_CLIP(3);

#undef	D_SET_CLIP

  // clip[ 0] = modl[ 0] * proj[ 0] + modl[ 1] * proj[ 4] + modl[ 2] * proj[ 8] + modl[ 3] * proj[12];
  // clip[ 1] = modl[ 0] * proj[ 1] + modl[ 1] * proj[ 5] + modl[ 2] * proj[ 9] + modl[ 3] * proj[13];
  // clip[ 2] = modl[ 0] * proj[ 2] + modl[ 1] * proj[ 6] + modl[ 2] * proj[10] + modl[ 3] * proj[14];
  // clip[ 3] = modl[ 0] * proj[ 3] + modl[ 1] * proj[ 7] + modl[ 2] * proj[11] + modl[ 3] * proj[15];

  // clip[ 4] = modl[ 4] * proj[ 0] + modl[ 5] * proj[ 4] + modl[ 6] * proj[ 8] + modl[ 7] * proj[12];
  // clip[ 5] = modl[ 4] * proj[ 1] + modl[ 5] * proj[ 5] + modl[ 6] * proj[ 9] + modl[ 7] * proj[13];
  // clip[ 6] = modl[ 4] * proj[ 2] + modl[ 5] * proj[ 6] + modl[ 6] * proj[10] + modl[ 7] * proj[14];
  // clip[ 7] = modl[ 4] * proj[ 3] + modl[ 5] * proj[ 7] + modl[ 6] * proj[11] + modl[ 7] * proj[15];

  // clip[ 8] = modl[ 8] * proj[ 0] + modl[ 9] * proj[ 4] + modl[10] * proj[ 8] + modl[11] * proj[12];
  // clip[ 9] = modl[ 8] * proj[ 1] + modl[ 9] * proj[ 5] + modl[10] * proj[ 9] + modl[11] * proj[13];
  // clip[10] = modl[ 8] * proj[ 2] + modl[ 9] * proj[ 6] + modl[10] * proj[10] + modl[11] * proj[14];
  // clip[11] = modl[ 8] * proj[ 3] + modl[ 9] * proj[ 7] + modl[10] * proj[11] + modl[11] * proj[15];

  // clip[12] = modl[12] * proj[ 0] + modl[13] * proj[ 4] + modl[14] * proj[ 8] + modl[15] * proj[12];
  // clip[13] = modl[12] * proj[ 1] + modl[13] * proj[ 5] + modl[14] * proj[ 9] + modl[15] * proj[13];
  // clip[14] = modl[12] * proj[ 2] + modl[13] * proj[ 6] + modl[14] * proj[10] + modl[15] * proj[14];
  // clip[15] = modl[12] * proj[ 3] + modl[13] * proj[ 7] + modl[14] * proj[11] + modl[15] * proj[15];



  m_Frustum[eRight][eA] = clip[ 3] - clip[ 0];
  m_Frustum[eRight][eB] = clip[ 7] - clip[ 4];
  m_Frustum[eRight][eC] = clip[11] - clip[ 8];
  m_Frustum[eRight][eD] = clip[15] - clip[12];
  NormalizePlane(m_Frustum, eRight);

  m_Frustum[eLeft][eA] = clip[ 3] + clip[ 0];
  m_Frustum[eLeft][eB] = clip[ 7] + clip[ 4];
  m_Frustum[eLeft][eC] = clip[11] + clip[ 8];
  m_Frustum[eLeft][eD] = clip[15] + clip[12];
  NormalizePlane(m_Frustum, eLeft);


  m_Frustum[eBottom][eA] = clip[ 3] + clip[ 1];
  m_Frustum[eBottom][eB] = clip[ 7] + clip[ 5];
  m_Frustum[eBottom][eC] = clip[11] + clip[ 9];
  m_Frustum[eBottom][eD] = clip[15] + clip[13];
  NormalizePlane(m_Frustum, eBottom);

  m_Frustum[eTop][eA] = clip[ 3] - clip[ 1];
  m_Frustum[eTop][eB] = clip[ 7] - clip[ 5];
  m_Frustum[eTop][eC] = clip[11] - clip[ 9];
  m_Frustum[eTop][eD] = clip[15] - clip[13];
  NormalizePlane(m_Frustum, eTop);


  m_Frustum[eBack][eA] = clip[ 3] - clip[ 2];
  m_Frustum[eBack][eB] = clip[ 7] - clip[ 6];
  m_Frustum[eBack][eC] = clip[11] - clip[10];
  m_Frustum[eBack][eD] = clip[15] - clip[14];
  NormalizePlane(m_Frustum, eBack);

  m_Frustum[eFront][eA] = clip[ 3] + clip[ 2];
  m_Frustum[eFront][eB] = clip[ 7] + clip[ 6];
  m_Frustum[eFront][eC] = clip[11] + clip[10];
  m_Frustum[eFront][eD] = clip[15] + clip[14];
  NormalizePlane(m_Frustum, eFront);

}

bool	GL_FrustumCulling::PointInFrustum( float x, float y, float z ) const
{
  for (int i = 0; i < 6; ++i)
    if ( m_Frustum[i][eA] * x +
	 m_Frustum[i][eB] * y +
	 m_Frustum[i][eC] * z +
	 m_Frustum[i][eD] <= 0 )
      return false;

  return true;
}

bool	GL_FrustumCulling::SphereInFrustum( float x, float y, float z, float radius ) const
{
  for (int i = 0; i < 6; ++i)
    if ( m_Frustum[i][eA] * x +
	 m_Frustum[i][eB] * y +
	 m_Frustum[i][eC] * z +
	 m_Frustum[i][eD] <= -radius )
      return false;

  return true;
}

bool	GL_FrustumCulling::CubeInFrustum( float x, float y, float z, float size ) const
{
  for (int i = 0; i < 6; ++i)
    {
      if (m_Frustum[i][eA] * (x - size) +
	  m_Frustum[i][eB] * (y - size) +
	  m_Frustum[i][eC] * (z - size) +
	  m_Frustum[i][eD] > 0)
	continue;

      if (m_Frustum[i][eA] * (x + size) +
	  m_Frustum[i][eB] * (y - size) +
	  m_Frustum[i][eC] * (z - size) +
	  m_Frustum[i][eD] > 0)
	continue;

      if (m_Frustum[i][eA] * (x - size) +
	  m_Frustum[i][eB] * (y + size) +
	  m_Frustum[i][eC] * (z - size) +
	  m_Frustum[i][eD] > 0)
	continue;

      if (m_Frustum[i][eA] * (x + size) +
	  m_Frustum[i][eB] * (y + size) +
	  m_Frustum[i][eC] * (z - size) +
	  m_Frustum[i][eD] > 0)
	continue;

      if (m_Frustum[i][eA] * (x - size) +
	  m_Frustum[i][eB] * (y - size) +
	  m_Frustum[i][eC] * (z + size) +
	  m_Frustum[i][eD] > 0)
	continue;

      if (m_Frustum[i][eA] * (x + size) +
	  m_Frustum[i][eB] * (y - size) +
	  m_Frustum[i][eC] * (z + size) +
	  m_Frustum[i][eD] > 0)
	continue;

      if (m_Frustum[i][eA] * (x - size) +
	  m_Frustum[i][eB] * (y + size) +
	  m_Frustum[i][eC] * (z + size) +
	  m_Frustum[i][eD] > 0)
	continue;

      if (m_Frustum[i][eA] * (x + size) +
	  m_Frustum[i][eB] * (y + size) +
	  m_Frustum[i][eC] * (z + size) +
	  m_Frustum[i][eD] > 0)
	continue;

      return false;
    }

  return true;
}


bool	GL_FrustumCulling::CubeInFrustum(const sf::Vector3f& v, const sf::Vector3f& s2) const
{
  sf::Vector3f	s(s2);
  s.x *= 0.5;
  s.y *= 0.5;
  s.z *= 0.5;

  for (int i = 0; i < 6; ++i)
    {
      if (m_Frustum[i][eA] * (v.x - s.x) +
	  m_Frustum[i][eB] * (v.y - s.y) +
	  m_Frustum[i][eC] * (v.z - s.z) +
	  m_Frustum[i][eD] > 0)
	continue;

      if (m_Frustum[i][eA] * (v.x + s.x) +
	  m_Frustum[i][eB] * (v.y - s.y) +
	  m_Frustum[i][eC] * (v.z - s.z) +
	  m_Frustum[i][eD] > 0)
	continue;

      if (m_Frustum[i][eA] * (v.x - s.x) +
	  m_Frustum[i][eB] * (v.y + s.y) +
	  m_Frustum[i][eC] * (v.z - s.z) +
	  m_Frustum[i][eD] > 0)
	continue;

      if (m_Frustum[i][eA] * (v.x + s.x) +
	  m_Frustum[i][eB] * (v.y + s.y) +
	  m_Frustum[i][eC] * (v.z - s.z) +
	  m_Frustum[i][eD] > 0)
	continue;

      if (m_Frustum[i][eA] * (v.x - s.x) +
	  m_Frustum[i][eB] * (v.y - s.y) +
	  m_Frustum[i][eC] * (v.z + s.z) +
	  m_Frustum[i][eD] > 0)
	continue;

      if (m_Frustum[i][eA] * (v.x + s.x) +
	  m_Frustum[i][eB] * (v.y - s.y) +
	  m_Frustum[i][eC] * (v.z + s.z) +
	  m_Frustum[i][eD] > 0)
	continue;

      if (m_Frustum[i][eA] * (v.x - s.x) +
	  m_Frustum[i][eB] * (v.y + s.y) +
	  m_Frustum[i][eC] * (v.z + s.z) +
	  m_Frustum[i][eD] > 0)
	continue;

      if (m_Frustum[i][eA] * (v.x + s.x) +
	  m_Frustum[i][eB] * (v.y + s.y) +
	  m_Frustum[i][eC] * (v.z + s.z) +
	  m_Frustum[i][eD] > 0)
	continue;

      return false;
    }

  return true;
}

// bool	Frustum::BoundingBoxInFrustum(BoundingBox& BB)
// {
//   return ( CubeInFrustum(BB.Center(), BB.Size())) ;
// }



////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////



void	GL_FrustumCulling::CalcOrtho( sf::Vector3f eye, sf::Vector3f target,
				      float left, float right,
				      float bottom, float top,
				      float nearval, float farval )
{
  float   proj[16];
  float   modl[16];


  glMatrixMode(GL_PROJECTION);

  glPushMatrix();
  {
    glLoadIdentity();

    glOrtho( left, right,
	     bottom, top,
	     nearval, farval );

    glGetFloatv( GL_PROJECTION_MATRIX, proj );
  }
  glPopMatrix();


  glMatrixMode(GL_MODELVIEW);

  glPushMatrix();
  {
    glLoadIdentity();

    gluLookAt( eye.x, eye.y, eye.z,
	       target.x, target.y, target.z,
	       0,0,1 );

    glGetFloatv( GL_MODELVIEW_MATRIX, modl );
  }
  glPopMatrix();


  CalculateFrustum(proj, modl);
}


void	GL_FrustumCulling::CalcPerspective( sf::Vector3f eye, sf::Vector3f target,
					    float angle, float aspect_ratio,
					    float nearval, float farval )
{
  float   proj[16];
  float   modl[16];


  glMatrixMode(GL_PROJECTION);

  glPushMatrix();
  {
    glLoadIdentity();

    gluPerspective(angle, aspect_ratio, nearval, farval);

    glGetFloatv( GL_PROJECTION_MATRIX, proj );
  }
  glPopMatrix();


  glMatrixMode(GL_MODELVIEW);

  glPushMatrix();
  {
    glLoadIdentity();

    gluLookAt( eye.x, eye.y, eye.z,
	       target.x, target.y, target.z,
	       0,0,1 );

    glGetFloatv( GL_MODELVIEW_MATRIX, modl );
  }
  glPopMatrix();


  CalculateFrustum(proj, modl);
}

////////////////////////////////////////////////////////////////////////////

void	GL_FrustumCulling::DrawOrtho( sf::Vector3f eye, sf::Vector3f target,
				      float left, float right,
				      float bottom, float top,
				      float nearval, float farval )
{
  float	theta = 0;
  float	phi = 0;

  {
    sf::Vector3f	diff;
    diff.x = target.x - eye.x;
    diff.y = target.y - eye.y;
    diff.z = target.z - eye.z;
    theta = atan2f(diff.y, diff.x) * 180.0f / 3.14f;

    sf::Vector2f	vec;
    vec.x = sqrtf( diff.x * diff.x + diff.y * diff.y );
    vec.y = -diff.z;
    phi = atan2f(vec.y, vec.x) * 180.0f / 3.14f;
  }


  sf::Vector3f	vertices[8];
  {
    vertices[0] = sf::Vector3f(nearval, left,  top);
    vertices[1] = sf::Vector3f(nearval, right, top);
    vertices[2] = sf::Vector3f(nearval, left,  bottom);
    vertices[3] = sf::Vector3f(nearval, right, bottom);

    vertices[4] = sf::Vector3f(farval, left,  top);
    vertices[5] = sf::Vector3f(farval, right, top);
    vertices[6] = sf::Vector3f(farval, left,  bottom);
    vertices[7] = sf::Vector3f(farval, right, bottom);
  }


  int	indices[] = {
    0,1, 1,3, 3,2, 2,0,
    0,4, 1,5, 2,6, 3,7,
    4,5, 5,7, 7,6, 6,4
  };

  int	indices2[] = {
    0,1,3,2, 0,1,5,4, 0,2,6,4,
    4,5,7,6, 2,3,7,6, 1,3,7,5
  };

  glPushMatrix();
  {
    glTranslatef(eye.x, eye.y, eye.z);
    glRotatef(theta, 0,0,1);
    glRotatef(phi, 0,1,0);

    glDisable(GL_BLEND);
    glBegin(GL_LINES);
    {
      for (int i = 0; i < 24; ++i)
	glVertex3fv( &(vertices[ indices[i] ].x) );
    }
    glEnd();

    glEnable(GL_BLEND);
    glBegin(GL_QUADS);
    {
      for (int i = 0; i < 24; ++i)
	glVertex3fv( &(vertices[ indices2[i] ].x) );
    }
    glEnd();
  }
  glPopMatrix();

}


void	GL_FrustumCulling::DrawPerspective( sf::Vector3f eye, sf::Vector3f target,
					    float angle, float aspect_ratio,
					    float nearval, float farval )
{
  float	theta = 0;
  float	phi = 0;

  {
    sf::Vector3f	diff;
    diff.x = target.x - eye.x;
    diff.y = target.y - eye.y;
    diff.z = target.z - eye.z;
    theta = atan2f(diff.y, diff.x) * 180.0f / 3.14f;

    sf::Vector2f	vec;
    vec.x = sqrtf( diff.x * diff.x + diff.y * diff.y );
    vec.y = -diff.z;
    phi = atan2f(vec.y, vec.x) * 180.0f / 3.14f;
  }


  sf::Vector3f	vertices[8];
  {
    float	left, right, bottom, top;

#define	D_PERSPECTIVE(fovY, aspect, zNear, zFar)	\
    {							\
      const GLdouble pi = 3.1415926f;			\
      GLdouble fW, fH;					\
							\
      fH = tan( fovY / 360.0 * pi ) * zNear;		\
      fW = fH * aspect;					\
							\
      left = -fW;					\
      right = +fW;					\
							\
      top = +fH;					\
      bottom = -fH;					\
    }

    D_PERSPECTIVE(angle, aspect_ratio, nearval, farval);

#undef	D_PERSPECTIVE

    vertices[0] = sf::Vector3f(nearval, left,  top);
    vertices[1] = sf::Vector3f(nearval, right, top);
    vertices[2] = sf::Vector3f(nearval, left,  bottom);
    vertices[3] = sf::Vector3f(nearval, right, bottom);

    float	half_y = farval * sinf(angle * 3.14f / 180.0f);
    float	half_z = half_y / aspect_ratio;

    vertices[4] = sf::Vector3f( farval, -half_y, +half_z );
    vertices[5] = sf::Vector3f( farval, +half_y, +half_z );
    vertices[6] = sf::Vector3f( farval, -half_y, -half_z );
    vertices[7] = sf::Vector3f( farval, +half_y, -half_z );
  }


  int	indices[] = {
    0,1, 1,3, 3,2, 2,0,
    0,4, 1,5, 2,6, 3,7,
    4,5, 5,7, 7,6, 6,4
  };

  int	indices2[] = {
    0,1,3,2, 0,1,5,4, 0,2,6,4,
    4,5,7,6, 2,3,7,6, 1,3,7,5
  };

  glPushMatrix();
  {
    glTranslatef(eye.x, eye.y, eye.z);
    glRotatef(theta, 0,0,1);
    glRotatef(phi, 0,1,0);

    glDisable(GL_BLEND);
    glBegin(GL_LINES);
    {
      for (int i = 0; i < 24; ++i)
	glVertex3fv( &(vertices[ indices[i] ].x) );
    }
    glEnd();

    glEnable(GL_BLEND);
    glBegin(GL_QUADS);
    {
      for (int i = 0; i < 24; ++i)
	glVertex3fv( &(vertices[ indices2[i] ].x) );
    }
    glEnd();
  }
  glPopMatrix();

}
