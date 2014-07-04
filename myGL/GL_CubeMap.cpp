

#include "GL_CubeMap.hpp"



int	cm_define[] = {
  GL_TEXTURE_CUBE_MAP_POSITIVE_X,
  GL_TEXTURE_CUBE_MAP_NEGATIVE_X,

  GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
  GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,

  GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
  GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
};



GL_CubeMap::GL_CubeMap()
  : m_cubemapId(0)
{
}

GL_CubeMap::~GL_CubeMap()
{
  Destroy();
}

////////////////////////////////////////////////////////////////////////////////////

void	GL_CubeMap::SetTexture_FileName(e_CubeMapSens sens, const std::string& FileName)
{
  if ( !(sens < eCubeMapSens_Count) )
    throw Exception_Ex("!(sens < eCubeMapSens_Count)");

  m_TexturesFilesName[ sens ] = FileName;
}

////////////////////////////////////////////////////////////////////////////////////

// void	GL_CubeMap::Load()
// {
//   for (int i = 0; i < eCubeMapSens_Count; ++i)
//     if (m_TexturesFilesName[ i ].empty())
//       throw Exception_Ex("m_TexturesFilesName[ sens ].empty()");

//   sf::Image	tmp_Images[ eCubeMapSens_Count ];

//   for (int i = 0; i < eCubeMapSens_Count; ++i)
//     if ( !tmp_Images[ i ].loadFromFile( m_TexturesFilesName[ i ] ) )
//       throw Exception_Ex("loadFromFile == false");

//   unsigned char*	pPixels[ eCubeMapSens_Count ];

//   for (int i = 0; i < eCubeMapSens_Count; ++i)
//     pPixels[ i ] = (unsigned char*)tmp_Images[ i ].getPixelsPtr();

//   Load( pPixels );
// }

// void	GL_CubeMap::Load( const sf::Vector2u& size,
// 			  unsigned char* pPixels[ eCubeMapSens_Count ] )

void	GL_CubeMap::Load()
{
  Destroy();

  {
    glGenTextures( 1, &m_cubemapId );
    glBindTexture( GL_TEXTURE_CUBE_MAP, m_cubemapId );
  }

  for (int i = 0; i < eCubeMapSens_Count; ++i)
    {
      sf::Image	tmp_Image;

      if ( !tmp_Image.loadFromFile( m_TexturesFilesName[ i ] ) )
	throw Exception_Ex("loadFromFile == false");

      const unsigned char*	tmp_pixels = (unsigned char*)tmp_Image.getPixelsPtr();
      const sf::Vector2u	tmp_size = tmp_Image.getSize();

      glTexImage2D( cm_define[ i ], 0,
		    GL_RGBA8, tmp_size.x, tmp_size.y, 0,
		    GL_RGBA, GL_UNSIGNED_BYTE, tmp_pixels );
    }

  glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  // glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  // glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

  glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
  glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

  glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );
}

// void	GL_CubeMap::Load( const sf::Vector2u& tex_size, GL_Texture* pTextures )
void	GL_CubeMap::Load( GL_Texture* pTextures )
{
  if (pTextures == NULL)
    throw Exception_Ex("(pTexture == NULL)");

  Destroy();

  glGenTextures( 1, &m_cubemapId );
  glBindTexture( GL_TEXTURE_CUBE_MAP, m_cubemapId );
  {

    for (int i = 0; i < eCubeMapSens_Count; ++i)
      {
	sf::Vector2u	tmp_size = pTextures[ i ].GetSize();
	unsigned char*	tmp_pPixels = new unsigned char[ tmp_size.x * tmp_size.y * 4 ];

	pTextures[ i ].GetPixels( tmp_pPixels );

	glTexImage2D( cm_define[ i ], 0,
		      GL_RGBA8, tmp_size.x, tmp_size.y, 0,
		      GL_RGBA, GL_UNSIGNED_BYTE, tmp_pPixels );

	delete[] tmp_pPixels;
      }

    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

  }
  glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );
}
