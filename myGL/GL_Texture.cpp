

#include "GL_Texture.hpp"


#include <iostream>


GLint	filters[6] = {
  GL_NEAREST,
  GL_LINEAR,

  GL_NEAREST_MIPMAP_NEAREST,
  GL_LINEAR_MIPMAP_NEAREST,
  GL_NEAREST_MIPMAP_LINEAR,
  GL_LINEAR_MIPMAP_LINEAR
};

std::string	str_filters[6] = {
  std::string("GL_NEAREST"),
  std::string("GL_LINEAR"),

  std::string("GL_NEAREST_MIPMAP_NEAREST"),
  std::string("GL_LINEAR_MIPMAP_NEAREST"),
  std::string("GL_NEAREST_MIPMAP_LINEAR"),
  std::string("GL_LINEAR_MIPMAP_LINEAR")
};


GLuint	GL_Texture::m_curr_texId;


void	GL_Texture::MinFilter()
{
  if (m_mimap_level > 0)
    {
      CurrentBind_Save();
      glBindTexture(GL_TEXTURE_2D, m_texId);
      {
	curMinFilter = (curMinFilter + 1) % 6;

	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,  filters[curMinFilter] );

	// std::cerr << std::endl;
	// std::cerr << "curMinFilter : " << curMinFilter << std::endl;
	// std::cerr << "curMinFilter : " << str_filters[ curMinFilter ] << std::endl;
      }
      CurrentBind_Restore();
    }
}

void	GL_Texture::MagFilter()
{
  if (m_mimap_level > 0)
    {
      CurrentBind_Save();
      glBindTexture(GL_TEXTURE_2D, m_texId);
      {
	curMagFilter = (curMagFilter + 1) % 2;

	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filters[curMagFilter] );

	// std::cerr << std::endl;
	// std::cerr << "curMagFilter : " << curMagFilter << std::endl;
	// std::cerr << "curMagFilter : " << str_filters[ curMagFilter ] << std::endl;
      }
      CurrentBind_Restore();
    }
}

const std::string&	GL_Texture::CurrentMinFilter()
{
  return (str_filters[curMinFilter]);
}

const std::string&	GL_Texture::CurrentMagFilter()
{
  return (str_filters[curMagFilter]);
}

////////////////////////////////////////////////////////////////////

GL_Texture::GL_Texture()
  : m_isRepeated(false),
    m_isSmooth(false),

    curMagFilter(0), curMinFilter(0)

{
  // glEnable(GL_TEXTURE_2D);
  // glDisable(GL_TEXTURE_2D);

  glGenTextures(1, &m_texId);
}

GL_Texture::~GL_Texture()
{
  glDeleteTextures(1, &m_texId);
}

////////////////////////////////////////////////////////////////////

// void	GL_Texture::Create(unsigned int width, unsigned int height)
// {
//   m_Size.x = width;
//   m_Size.y = height;

//   CurrentBind_Save();
//   glBindTexture(GL_TEXTURE_2D, m_texId);
//   {
//     // glTexImage2D( GL_TEXTURE_2D,
//     // 		    0, GL_RGBA8, m_Size.x, m_Size.y,
//     // 		    0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );

//     gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGBA,
// 		       m_Size.x, m_Size.y,
// 		       // imageWidth, imageHeight,
// 		       GL_RGBA, GL_UNSIGNED_BYTE, NULL );

//     glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filters[ curMinFilter ] );
//     glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filters[ curMagFilter ] );
//   }
//   CurrentBind_Restore();
// }

void	GL_Texture::Create(unsigned int width, unsigned int height, const unsigned char* pixels,
			   unsigned int mimap_level)
{
  m_Size.x = width;
  m_Size.y = height;

  CurrentBind_Save();
  glBindTexture(GL_TEXTURE_2D, m_texId);
  {
    // glTexImage2D( GL_TEXTURE_2D,
    // 		    0, GL_RGBA8, m_Size.x, m_Size.y,
    // 		    0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );


    // gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGBA,
    // 		       m_Size.x, m_Size.y,
    // 		       // imageWidth, imageHeight,
    // 		       GL_RGBA, GL_UNSIGNED_BYTE, pixels );

    {
      // int	num_mipmaps = 6;

      // glEnable(GL_TEXTURE_2D);

      // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
      // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 6);


      // glTexStorage2D(GL_TEXTURE_2D, num_mipmaps, GL_RGBA8, width, height);
      // glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

      // glGenerateMipmap(GL_TEXTURE_2D);

      // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

      // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }

    {
      glEnable(GL_TEXTURE_2D);


      m_mimap_level = mimap_level;


      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, m_mimap_level);

      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

      if (m_mimap_level > 0)
	{
	  glGenerateMipmap(GL_TEXTURE_2D);  //Generate mipmaps now!!!

	  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filters[ curMinFilter ] );
	  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filters[ curMagFilter ] );
	}


      // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      // // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    }

    {
      // const unsigned char*	pixels = (unsigned char*)Img.getPixelsPtr();

      // unsigned char	pixels[ 64 * 64 * 4 ];

      // int	size = 64;

      // while (size > 0)
      //   {
      // 	int	inc = 0;

      // 	for (int y = 0; y < size; ++y)
      // 	  for (int y = 0; y < size; ++y)
      // 	    {
      // 	      pixels[ inc * 4 ] = ;
      // 	      ++inc;
      // 	    }

      // 	size /= 2;
      //   }

      // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, MIPLevels[0]);
      // glTexImage2D(GL_TEXTURE_2D, 1, GL_RGBA, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, MIPLevels[0]);
      // glTexImage2D(GL_TEXTURE_2D, 2, GL_RGBA, 16, 16, 0, GL_RGBA, GL_UNSIGNED_BYTE, MIPLevels[0]);
      // glTexImage2D(GL_TEXTURE_2D, 3, GL_RGBA,  8,  8, 0, GL_RGBA, GL_UNSIGNED_BYTE, MIPLevels[0]);
      // glTexImage2D(GL_TEXTURE_2D, 4, GL_RGBA,  4,  4, 0, GL_RGBA, GL_UNSIGNED_BYTE, MIPLevels[0]);
      // glTexImage2D(GL_TEXTURE_2D, 5, GL_RGBA,  2,  2, 0, GL_RGBA, GL_UNSIGNED_BYTE, MIPLevels[0]);
      // glTexImage2D(GL_TEXTURE_2D, 6, GL_RGBA,  1,  1, 0, GL_RGBA, GL_UNSIGNED_BYTE, MIPLevels[0]);
    }

    // glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filters[ curMinFilter ] );
    // glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filters[ curMagFilter ] );
  }
  CurrentBind_Restore();
}

void	GL_Texture::Create(const sf::Vector2u& size, const unsigned char* pixels,
			   unsigned int mimap_level)
{
  Create(size.x, size.y, pixels, mimap_level);
}

bool	GL_Texture::Load(const std::string& filename,
			 unsigned int mimap_level)
{
  sf::Image	Img;

  if (Img.loadFromFile( filename ))
    {
      const unsigned char*	pixels = (unsigned char*)Img.getPixelsPtr();

      glBindTexture(GL_TEXTURE_2D, m_texId);

      Create( Img.getSize(), pixels, mimap_level );

      // for (unsigned int i = 0; i < m_Size.y; ++i)
      // 	{
      // 	  glTexSubImage2D( GL_TEXTURE_2D, 0, 0, i, m_Size.x,
      // 			   1, GL_RGBA, GL_UNSIGNED_BYTE, pixels );

      // 	  pixels += 4 * m_Size.x;
      // 	}

      return (true);
    }

  return (false);
}

void	GL_Texture::DumpScreen()
{
  CurrentBind_Save();
  glBindTexture(GL_TEXTURE_2D, m_texId);
  {
    glCopyTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, 0, 0, m_Size.x, m_Size.y );
  }
  CurrentBind_Restore();
}

void	GL_Texture::GetPixels( unsigned char* pixels )
{
  CurrentBind_Save();
  glBindTexture(GL_TEXTURE_2D, m_texId);
  {
    glGetTexImage( GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );
  }
  CurrentBind_Restore();
}

void	GL_Texture::SetRepeat(bool repeat)
{
  if (repeat != m_isRepeated)
    {
      m_isRepeated = repeat;

      CurrentBind_Save();
      glBindTexture(GL_TEXTURE_2D, m_texId);
      {
	GLint	param = (repeat ? GL_REPEAT : GL_CLAMP);

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, param );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, param );
      }
      CurrentBind_Restore();
    }
}

void	GL_Texture::SetSmooth(bool smooth)
{
  if (smooth != m_isSmooth)
    {
      m_isSmooth = smooth;

      CurrentBind_Save();
      glBindTexture(GL_TEXTURE_2D, m_texId);
      {
	GLint	param = (smooth ? GL_LINEAR : GL_NEAREST);

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param );
      }
      CurrentBind_Restore();
    }
}
