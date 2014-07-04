

#ifndef	GL_TEXTURE_HPP
#define	GL_TEXTURE_HPP


#include "Header.hpp"

#include <iostream>


class	GL_Texture
{
private :

  GLuint	m_texId;
  sf::Vector2u	m_Size;

  bool		m_isRepeated;
  bool		m_isSmooth;


  unsigned int	m_mimap_level;
  int		curMagFilter, curMinFilter;

public :

  void	MinFilter();
  void	MagFilter();

  const std::string&	CurrentMinFilter();
  const std::string&	CurrentMagFilter();


public :

  inline const sf::Vector2u&	GetSize() const	{return (m_Size);}


private :

  static GLuint	m_curr_texId;

  inline void	CurrentBind_Save()
  {
    glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint*)&m_curr_texId);
  }

  inline void	CurrentBind_Restore() const
  {
    glBindTexture(GL_TEXTURE_2D, m_curr_texId);
  }

public :

  inline void	Bind() const
  {
    glBindTexture(GL_TEXTURE_2D, m_texId);
  }

  inline static void	UnBind()
  {
    glBindTexture(GL_TEXTURE_2D, 0);
  }

public :

  GL_Texture();
  ~GL_Texture();

public :

  inline bool	IsRepeated() const	{return (m_isRepeated);}
  inline bool	IsSmooth() const	{return (m_isSmooth);}

public :

  // void	Create(unsigned int width, unsigned int height);
  void	Create(unsigned int width, unsigned int height, const unsigned char* pixels = NULL,
	       unsigned int mimap_level = 0);
  void	Create(const sf::Vector2u& size, const unsigned char* pixels = NULL, unsigned int mimap_level = 0);

  bool	Load(const std::string& filename, unsigned int mimap_level = 0);

  void	DumpScreen();
  void	GetPixels( unsigned char* pixels );

  void	SetRepeat(bool repeat);
  void	SetSmooth(bool smooth);

};


#endif

