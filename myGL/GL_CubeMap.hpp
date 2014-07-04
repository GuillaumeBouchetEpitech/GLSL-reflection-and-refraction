

#ifndef	GL_CUBEMAP_HPP
#define	GL_CUBEMAP_HPP


#include "Header.hpp"

#include "Exception.hpp"


#include "GL_Texture.hpp"


class	GL_CubeMap
{
public :

  enum e_CubeMapSens {

    ePositive_x = 0,
    eNegative_x,

    ePositive_y,
    eNegative_y,

    ePositive_z,
    eNegative_z,

    eCubeMapSens_Count
  };

private :

  std::string	m_TexturesFilesName[ eCubeMapSens_Count ];
  GL_Texture*	m_pTextures[ eCubeMapSens_Count ];

  GLuint	m_cubemapId;

public :

  GL_CubeMap();
  ~GL_CubeMap();

public :

  void	SetTexture_FileName(e_CubeMapSens sens, const std::string& FileName);
  void	SetTexture_Ptr(e_CubeMapSens sens, GL_Texture* pTexture);

public :

  void	Load();
  // void	Load( const sf::Vector2u& size, const unsigned char* pPixels[ eCubeMapSens_Count ] );
  // void	Load( const sf::Vector2u& size, GL_Texture* pTexture );
  void	Load( GL_Texture* pTexture );

  inline void	Destroy()
  {
    if (m_cubemapId)
      glDeleteTextures( 1, &m_cubemapId );
  }

public :

  inline void	Bind() const
  {
    glBindTexture( GL_TEXTURE_CUBE_MAP, m_cubemapId );
  }

  inline static void	UnBind()
  {
    glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );
  }

};


#endif

