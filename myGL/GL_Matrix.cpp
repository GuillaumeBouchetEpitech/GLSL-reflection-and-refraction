

#include "GL_Matrix.hpp"

#include <cmath>
#include <limits>


#define SET_ROW(row, v1, v2, v3, v4)		\
  m_mat[ (row) ][0] = (v1);			\
  m_mat[ (row) ][1] = (v2);			\
  m_mat[ (row) ][2] = (v3);			\
  m_mat[ (row) ][3] = (v4);


////////////////////////////////////////

int	GL_Matrix::Compare(const GL_Matrix& m) const
{
  const float* lhs = reinterpret_cast<const float*>(m_mat);
  const float* end_lhs = lhs + 16;
  const float* rhs = reinterpret_cast<const float*>(m.m_mat);

  int	retval = 0;

  for(; !retval && lhs != end_lhs; ++lhs,++ rhs)
    {
      if (*lhs < *rhs)
	retval = -1;
      else if (*rhs < *lhs)
	retval = 1;
    }

  return (retval);
}

////////////////////////////////////////

GL_Matrix&	GL_Matrix::operator = (const GL_Matrix& rhs)
{
  if (&rhs != this)
    Set(rhs.m_tab);
  return *this;
}

////////////////////////////////////////

bool	GL_Matrix::IsIdentity() const
{
  return ( m_mat[0][0] == 1.0f &&
	   m_mat[0][1] == 0.0f &&
	   m_mat[0][2] == 0.0f &&
	   m_mat[0][3] == 0.0f &&
	   m_mat[1][0] == 0.0f &&
	   m_mat[1][1] == 1.0f &&
	   m_mat[1][2] == 0.0f &&
	   m_mat[1][3] == 0.0f &&
	   m_mat[2][0] == 0.0f &&
	   m_mat[2][1] == 0.0f &&
	   m_mat[2][2] == 1.0f &&
	   m_mat[2][3] == 0.0f &&
	   m_mat[3][0] == 0.0f &&
	   m_mat[3][1] == 0.0f &&
	   m_mat[3][2] == 0.0f &&
	   m_mat[3][3] == 1.0f );
}

void	GL_Matrix::Make_Identity()
{
  SET_ROW(0,   1.0f, 0.0f, 0.0f, 0.0f);
  SET_ROW(1,   0.0f, 1.0f, 0.0f, 0.0f);
  SET_ROW(2,   0.0f, 0.0f, 1.0f, 0.0f);
  SET_ROW(3,   0.0f, 0.0f, 0.0f, 1.0f);
}

////////////////////////////////////////

void	GL_Matrix::Make_Scale(float x, float y, float z)
{
  SET_ROW(0,   x,    0.0f, 0.0f, 0.0f);
  SET_ROW(1,   0.0f, y,    0.0f, 0.0f);
  SET_ROW(2,   0.0f, 0.0f, z,    0.0f);
  SET_ROW(3,   0.0f, 0.0f, 0.0f, 1.0f);
}

////////////////////////////////////////

void	GL_Matrix::Make_Translate(float x, float y, float z)
{
  SET_ROW(0,   1.0f, 0.0f, 0.0f, 0.0f);
  SET_ROW(1,   0.0f, 1.0f, 0.0f, 0.0f);
  SET_ROW(2,   0.0f, 0.0f, 1.0f, 0.0f);
  SET_ROW(3,   x,    y,    z,    1.0f);
}

////////////////////////////////////////

void	GL_Matrix::Make_Rotate(float angle, float x, float y, float z)
{
  sf::Vector3f	u(x, y, z);

#define	D_SQ(v)		( (v) * (v) )
#define	D_LEN2(v3)	( D_SQ( (v3).x ) + D_SQ( (v3).y ) + D_SQ( (v3).z ) )
#define	D_LEN(v3)	( sqrtf( D_LEN2(v3) ) )
#define	D_NORMALIZE(v3)				\
  {						\
    float	len = D_LEN(v3);		\
    (v3).x /= len;				\
    (v3).y /= len;				\
    (v3).z /= len;				\
  }

  D_NORMALIZE( u );

#undef	D_NORMALIZE
#undef	D_LEN
#undef	D_LEN2
#undef	D_SQ

  float sinAngle = (float)sinf(M_PI * angle / 180);
  float cosAngle = (float)cosf(M_PI * angle / 180);
  float oneMinusCosAngle = 1.0f - cosAngle;

  float*	entries = m_tab;

  memset(entries, 0, 16 * sizeof(float));
  entries[0] = 1.0f;
  entries[5] = 1.0f;
  entries[10] = 1.0f;
  entries[15] = 1.0f;

  entries[0] = (u.x) * (u.x) + cosAngle * (1 - (u.x) * (u.x));
  entries[4] = (u.x) * (u.y) * (oneMinusCosAngle) - sinAngle * u.z;
  entries[8] = (u.x) * (u.z) * (oneMinusCosAngle) + sinAngle * u.y;

  entries[1] = (u.x) * (u.y) * (oneMinusCosAngle) + sinAngle * u.z;
  entries[5] = (u.y) * (u.y) + cosAngle * (1 - (u.y) * (u.y));
  entries[9] = (u.y) * (u.z) * (oneMinusCosAngle) - sinAngle * u.x;

  entries[2] = (u.x) * (u.z) * (oneMinusCosAngle) - sinAngle * u.y;
  entries[6] = (u.y) * (u.z) * (oneMinusCosAngle) + sinAngle * u.x;
  entries[10] = (u.z) * (u.z) + cosAngle * (1 - (u.z) * (u.z));
}

////////////////////////////////////////

void	GL_Matrix::Make_Ortho(float left,   float right,
			    float bottom, float top,
			    float zNear,  float zFar)
{
  double tx = -(right + left) / (right - left);
  double ty = -(top + bottom) / (top - bottom);
  double tz = -(zFar + zNear) / (zFar - zNear);

  SET_ROW( 0, 2.0 / (right - left),                  0.0,                   0.0, 0.0 );
  SET_ROW( 1,                  0.0, 2.0 / (top - bottom),                   0.0, 0.0 );
  SET_ROW( 2,                  0.0,                  0.0, -2.0 / (zFar - zNear), 0.0 );
  SET_ROW( 3,                   tx,                   ty,                    tz, 1.0 );
}

////////////////////////////////////////

void	GL_Matrix::Make_Frustum(float left,   float right,
			      float bottom, float top,
			      float zNear,  float zFar)
{
  float A = (right + left) / (right - left);
  float B = (top + bottom) / (top - bottom);

  float C = ( (fabs(zFar) > std::numeric_limits<float>::max())
	      ? (-1.0f)
	      : (-(zFar + zNear) / (zFar - zNear)) );

  float D = ( (fabs(zFar) > std::numeric_limits<float>::max())
	      ? (-2.0f * zNear)
	      : (-2.0f * zFar * zNear / (zFar - zNear)) );

  SET_ROW(0, 2.0f * zNear / (right - left), 0.0f,                          0.0f,  0.0f);
  SET_ROW(1, 0.0f,                          2.0f * zNear / (top - bottom), 0.0f,  0.0f);
  SET_ROW(2, A,                             B,                             C,    -1.0f);
  SET_ROW(3, 0.0f,                          0.0f,                          D,     0.0f);
}

////////////////////////////////////////

void	GL_Matrix::Make_Perspective(float fovy,  float aspectRatio,
				  float zNear, float zFar)
{
  float tan_fovy = tanf(M_PI * fovy / 360.0f);
  float right  =  tan_fovy * aspectRatio * zNear;
  float left   = -right;
  float top    =  tan_fovy * zNear;
  float bottom =  -top;

  Make_Frustum(left, right, bottom, top, zNear, zFar);
}

////////////////////////////////////////

void	GL_Matrix::Make_LookAt(const sf::Vector3f& eye, const sf::Vector3f& center, const sf::Vector3f& up)
{

#define	D_SQ(v)		( (v) * (v) )
#define	D_LEN2(v3)	( D_SQ( (v3).x ) + D_SQ( (v3).y ) + D_SQ( (v3).z ) )
#define	D_LEN(v3)	( sqrtf( D_LEN2(v3) ) )
#define	D_NORMALIZE(v3)				\
  {						\
    float	len = D_LEN(v3);		\
    (v3).x /= len;				\
    (v3).y /= len;				\
    (v3).z /= len;				\
  }

  // v1 ^ v2;
#define	D_CHAPOOO(v1, v2)			\
  ( sf::Vector3f(v1.y * v2.z - v1.z * v2.y,	\
		 v1.z * v2.x - v1.x * v2.z,	\
		 v1.x * v2.y - v1.y * v2.x) )


  sf::Vector3f f(center - eye);
  D_NORMALIZE(f);

  sf::Vector3f s( D_CHAPOOO(f, up) );
  D_NORMALIZE(s);

  sf::Vector3f u( D_CHAPOOO(s, f) );
  D_NORMALIZE(u);


#undef	D_CHAPOOO

#undef	D_NORMALIZE
#undef	D_LEN
#undef	D_LEN2
#undef	D_SQ

  SET_ROW(0, s.x,  u.x,  -f.x, 0.0f);
  SET_ROW(1, s.y,  u.y,  -f.y, 0.0f);
  SET_ROW(2, s.z,  u.z,  -f.z, 0.0f);
  SET_ROW(3, 0.0f, 0.0f, 0.0f, 1.0f);

  PreMultTranslate(-eye);
}

////////////////////////////////////////

sf::Vector3f GL_Matrix::PreMult(const sf::Vector3f& v) const
{
  float d = 1.0f / (m_mat[0][3] * v.x +
		    m_mat[1][3] * v.y +
		    m_mat[2][3] * v.z +
		    m_mat[3][3]);

  return (sf::Vector3f((m_mat[0][0] * v.x + m_mat[1][0] * v.y +
			m_mat[2][0] * v.z + m_mat[3][0]) * d,
		       (m_mat[0][1] * v.x + m_mat[1][1] * v.y +
			m_mat[2][1] * v.z + m_mat[3][1]) * d,
		       (m_mat[0][2] * v.x + m_mat[1][2] * v.y +
			m_mat[2][2] * v.z + m_mat[3][2]) * d));
}

sf::Vector3f	GL_Matrix::PostMult(const sf::Vector3f& v) const
{
  float d = 1.0f / (m_mat[3][0] * v.x +
		    m_mat[3][1] * v.y +
		    m_mat[3][2] * v.z +
		    m_mat[3][3]);

  return (sf::Vector3f((m_mat[0][0] * v.x + m_mat[0][1] * v.y +
			m_mat[0][2] * v.z + m_mat[0][3]) * d,
		       (m_mat[1][0] * v.x + m_mat[1][1] * v.y +
			m_mat[1][2] * v.z + m_mat[1][3]) * d,
		       (m_mat[2][0] * v.x + m_mat[2][1] * v.y +
			m_mat[2][2] * v.z + m_mat[2][3]) * d));
}

void	GL_Matrix::PreMult(const float* v4_in, float* v4_out) const
{
  v4_out[0] = ( m_mat[0][0] * v4_in[0] + m_mat[1][0] * v4_in[1] +
		m_mat[2][0] * v4_in[2] + m_mat[3][0] * v4_in[3] );
  v4_out[1] = ( m_mat[0][1] * v4_in[0] + m_mat[1][1] * v4_in[1] +
		m_mat[2][1] * v4_in[2] + m_mat[3][1] * v4_in[3] );
  v4_out[2] = ( m_mat[0][2] * v4_in[0] + m_mat[1][2] * v4_in[1] +
		m_mat[2][2] * v4_in[2] + m_mat[3][2] * v4_in[3] );
  v4_out[3] = ( m_mat[0][3] * v4_in[0] + m_mat[1][3] * v4_in[1] +
		m_mat[2][3] * v4_in[2] + m_mat[3][3] * v4_in[3] );

  // return (Vec4f((m_mat[0][0] * v.x + m_mat[1][0] * v.y +
  // 		 m_mat[2][0] * v.z + m_mat[3][0] * v.w),
  // 		(m_mat[0][1] * v.x + m_mat[1][1] * v.y +
  // 		 m_mat[2][1] * v.z + m_mat[3][1] * v.w),
  // 		(m_mat[0][2] * v.x + m_mat[1][2] * v.y +
  // 		 m_mat[2][2] * v.z + m_mat[3][2] * v.w),
  // 		(m_mat[0][3] * v.x + m_mat[1][3] * v.y +
  // 		 m_mat[2][3] * v.z + m_mat[3][3] * v.w)));

}

void	GL_Matrix::PostMult(const float* v4_in, float* v4_out) const
{

  v4_out[0] = (m_mat[0][0] * v4_in[0] + m_mat[1][0] * v4_in[1] +
	       m_mat[2][0] * v4_in[2] + m_mat[3][0] * v4_in[3]);
  v4_out[1] = (m_mat[0][1] * v4_in[0] + m_mat[1][1] * v4_in[1] +
	       m_mat[2][1] * v4_in[2] + m_mat[3][1] * v4_in[3]);
  v4_out[2] = (m_mat[0][2] * v4_in[0] + m_mat[1][2] * v4_in[1] +
	       m_mat[2][2] * v4_in[2] + m_mat[3][2] * v4_in[3]);
  v4_out[3] = (m_mat[0][3] * v4_in[0] + m_mat[1][3] * v4_in[1] +
	       m_mat[2][3] * v4_in[2] + m_mat[3][3] * v4_in[3]);

  // return (Vec4f((m_mat[0][0] * v.x + m_mat[1][0] * v.y +
  // 		 m_mat[2][0] * v.z + m_mat[3][0] * v.w),
  // 		(m_mat[0][1] * v.x + m_mat[1][1] * v.y +
  // 		 m_mat[2][1] * v.z + m_mat[3][1] * v.w),
  // 		(m_mat[0][2] * v.x + m_mat[1][2] * v.y +
  // 		 m_mat[2][2] * v.z + m_mat[3][2] * v.w),
  // 		(m_mat[0][3] * v.x + m_mat[1][3] * v.y +
  // 		 m_mat[2][3] * v.z + m_mat[3][3] * v.w)));

}

////////////////////////////////////////

#define INNER_PRODUCT(a,b,r,c)			\
  (((a).m_mat[r][0] * (b).m_mat[0][c]) +	\
   ((a).m_mat[r][1] * (b).m_mat[1][c]) +	\
   ((a).m_mat[r][2] * (b).m_mat[2][c]) +	\
   ((a).m_mat[r][3] * (b).m_mat[3][c]))

void	GL_Matrix::Mult(const GL_Matrix& lhs, const GL_Matrix& rhs)
{
  if (&lhs == this)
    PostMult(rhs);

  else if (&rhs == this)
    PreMult(lhs);

  else
    {
      m_mat[0][0] = INNER_PRODUCT(lhs, rhs, 0, 0);
      m_mat[0][1] = INNER_PRODUCT(lhs, rhs, 0, 1);
      m_mat[0][2] = INNER_PRODUCT(lhs, rhs, 0, 2);
      m_mat[0][3] = INNER_PRODUCT(lhs, rhs, 0, 3);

      m_mat[1][0] = INNER_PRODUCT(lhs, rhs, 1, 0);
      m_mat[1][1] = INNER_PRODUCT(lhs, rhs, 1, 1);
      m_mat[1][2] = INNER_PRODUCT(lhs, rhs, 1, 2);
      m_mat[1][3] = INNER_PRODUCT(lhs, rhs, 1, 3);

      m_mat[2][0] = INNER_PRODUCT(lhs, rhs, 2, 0);
      m_mat[2][1] = INNER_PRODUCT(lhs, rhs, 2, 1);
      m_mat[2][2] = INNER_PRODUCT(lhs, rhs, 2, 2);
      m_mat[2][3] = INNER_PRODUCT(lhs, rhs, 2, 3);

      m_mat[3][0] = INNER_PRODUCT(lhs, rhs, 3, 0);
      m_mat[3][1] = INNER_PRODUCT(lhs, rhs, 3, 1);
      m_mat[3][2] = INNER_PRODUCT(lhs, rhs, 3, 2);
      m_mat[3][3] = INNER_PRODUCT(lhs, rhs, 3, 3);
    }
}

void	GL_Matrix::PreMult(const GL_Matrix& other)
{
  float	t[4];
  for(int col = 0; col < 4; ++col)
    {
      t[0] = INNER_PRODUCT( other, *this, 0, col );
      t[1] = INNER_PRODUCT( other, *this, 1, col );
      t[2] = INNER_PRODUCT( other, *this, 2, col );
      t[3] = INNER_PRODUCT( other, *this, 3, col );
      m_mat[0][col] = t[0];
      m_mat[1][col] = t[1];
      m_mat[2][col] = t[2];
      m_mat[3][col] = t[3];
    }
}

void	GL_Matrix::PostMult(const GL_Matrix& other)
{
  float	t[4];
  for (int row = 0; row < 4; ++row)
    {
      t[0] = INNER_PRODUCT(*this, other, row, 0);
      t[1] = INNER_PRODUCT(*this, other, row, 1);
      t[2] = INNER_PRODUCT(*this, other, row, 2);
      t[3] = INNER_PRODUCT(*this, other, row, 3);
      SET_ROW(row, t[0], t[1], t[2], t[3]);
    }
}

#undef	INNER_PRODUCT

#undef	SET_ROW

///

void	GL_Matrix::PreMultTranslate(const sf::Vector3f& v)
{
  for (unsigned i = 0; i < 3; ++i)
    {
      float tmp = ((float*)(&v.x))[i];

      if (tmp == 0)
	continue;

      m_mat[3][0] += tmp * m_mat[i][0];
      m_mat[3][1] += tmp * m_mat[i][1];
      m_mat[3][2] += tmp * m_mat[i][2];
      m_mat[3][3] += tmp * m_mat[i][3];
    }
}

void	GL_Matrix::PostMultTranslate(const sf::Vector3f& v)
{
  for (unsigned i = 0; i < 3; ++i)
    {
      float tmp = ((float*)(&v.x))[i];

      if (tmp == 0)
	continue;

      m_mat[0][i] += tmp * m_mat[0][3];
      m_mat[1][i] += tmp * m_mat[1][3];
      m_mat[2][i] += tmp * m_mat[2][3];
      m_mat[3][i] += tmp * m_mat[3][3];
    }
}


void	GL_Matrix::PreMultScale(const sf::Vector3f& v3)
{
  m_mat[0][0] *= v3.x; m_mat[0][1] *= v3.x; m_mat[0][2] *= v3.x; m_mat[0][3] *= v3.x;
  m_mat[1][0] *= v3.y; m_mat[1][1] *= v3.y; m_mat[1][2] *= v3.y; m_mat[1][3] *= v3.y;
  m_mat[2][0] *= v3.z; m_mat[2][1] *= v3.z; m_mat[2][2] *= v3.z; m_mat[2][3] *= v3.z;
}

void	GL_Matrix::PostMultScale(const sf::Vector3f& v3)
{
  m_mat[0][0] *= v3.x; m_mat[1][0] *= v3.x; m_mat[2][0] *= v3.x; m_mat[3][0] *= v3.x;
  m_mat[0][1] *= v3.y; m_mat[1][1] *= v3.y; m_mat[2][1] *= v3.y; m_mat[3][1] *= v3.y;
  m_mat[0][2] *= v3.z; m_mat[1][2] *= v3.z; m_mat[2][2] *= v3.z; m_mat[3][2] *= v3.z;
}


void	GL_Matrix::PreMultRotate(float angle, float x, float y, float z)
{
  if (angle == 0)
    return;

  GL_Matrix	r;
  r.Make_Rotate(angle, x, y, z);
  PreMult(r);
}

void	GL_Matrix::PostMultRotate(float angle, float x, float y, float z)
{
  if (angle == 0)
    return;

  GL_Matrix r;
  r.Make_Rotate(angle, x, y, z);
  PostMult(r);
}


#undef SET_ROW

