

#ifndef VOX_MATRIXF
#define VOX_MATRIXF


#include "Header.hpp"

#include <cstring>


class	GL_Matrix
{
protected :

  union
  {
    float	m_mat[4][4];
    float	m_tab[16];
  };

public :

  inline GL_Matrix()
  {
    Make_Identity();
  }

public :

  inline void	Use_Mult() const
  {
    glMultMatrixf(m_tab);
  }

  inline void	Use_Load() const
  {
    glLoadMatrixf(m_tab);
  }

private :

  int	Compare(const GL_Matrix& m) const;

public :

  inline bool	operator == (const GL_Matrix& m) const
  {
    return (Compare(m) == 0);
  }

  inline bool	operator != (const GL_Matrix& m) const
  {
    return (Compare(m) != 0);
  }

public :

  inline float& operator()(int row, int col)
  {
    return (m_mat[row][col]);
  }

  inline float operator()(int row, int col) const
  {
    return (m_mat[row][col]);
  }

public :

  inline float& operator[](unsigned int idx)
  {
    return (m_tab[idx]);
  }

  inline float operator[](unsigned int idx) const
  {
    return (m_tab[idx]);
  }

public :

  inline void	Get(float *ptr) const
  {
    memcpy(ptr, m_tab, 16 * sizeof(float));
  }

  inline const sf::Transform&   Get() const
  {
    return ((sf::Transform&)m_tab);
  }

  inline const float*	Ptr() const
  {
    return (m_tab);
  }

public :

  GL_Matrix&	operator = (const GL_Matrix& rhs);

public :

  inline void Set(float const * const ptr)
  {
    memcpy(m_tab, ptr, 16 * sizeof(float));
  }

  inline void Set(const GL_Matrix& mat)
  {
    memcpy(m_tab, mat.m_tab, 16 * sizeof(float));
  }

public :

  bool	IsIdentity() const;
  void	Make_Identity();

public :

  void	Make_Scale(float x, float y, float z);

  inline void	Make_Scale(const sf::Vector3f& v3)
  {
    Make_Scale(v3.x, v3.y, v3.z);
  }

public :

  void	Make_Translate(float x, float y, float z);

  inline void	Make_Translate(const sf::Vector3f& v3)
  {
    Make_Translate(v3.x, v3.y, v3.z);
  }

public :

  void	Make_Rotate(float angle, float x, float y, float z);

  inline void	Make_Rotate(float angle, const sf::Vector3f& axis)
  {
    Make_Rotate(angle, axis.x, axis.y, axis.z);
  }

public :

  void	Make_Ortho( float left,   float right,
		    float bottom, float top,
		    float zNear,  float zFar );

public :

  void	Make_Frustum( float left,   float right,
		      float bottom, float top,
		      float zNear,  float zFar );

public :

  void	Make_Perspective( float fovy,  float aspectRatio,
			  float zNear, float zFar );

public :

  void	Make_LookAt( const sf::Vector3f& eye,
		     const sf::Vector3f& center,
		     const sf::Vector3f& up );

public :

  sf::Vector3f PreMult(const sf::Vector3f& v) const;
  sf::Vector3f PostMult(const sf::Vector3f& v) const;

  inline sf::Vector3f	operator * (const sf::Vector3f& v) const
  {
    return (PostMult(v));
  }

  inline GL_Matrix	operator * (const GL_Matrix& m) const
  {
    GL_Matrix	ret_val;
    ret_val.Set(*this);
    ret_val.PreMult(m);
    return (ret_val);
  }

public :

  // sf::Vector3f PreMult(const float* v4) const;
  // sf::Vector3f PostMult(const float* v4) const;
  void	PreMult(const float* v4_in, float* v4_out) const;
  void	PostMult(const float* v4_in, float* v4_out) const;

  // inline sf::Vector3f operator * (const sf::Vector3f& v) const
  // {
  //   // return (PostMult(v));
  //   return (PreMult(v));
  // }

public :

  void	Mult(const GL_Matrix& lhs, const GL_Matrix& rhs);

  void	PreMult(const GL_Matrix& other);
  void	PostMult(const GL_Matrix& other);

  ///

  void	PreMultTranslate(const sf::Vector3f& v);
  void	PostMultTranslate(const sf::Vector3f& v);

  void	PreMultScale(const sf::Vector3f& v3);
  void	PostMultScale(const sf::Vector3f& v3);

  void	PreMultRotate(float angle, float x, float y, float z);
  void	PostMultRotate(float angle, float x, float y, float z);

};


#endif	// VOX_MATRIXF

