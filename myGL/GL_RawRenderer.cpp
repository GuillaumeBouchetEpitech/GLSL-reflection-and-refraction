

#include "GL_RawRenderer.hpp"


////////////////////////////////////////////////////////////


t_Vertex3D::t_Vertex3D()
{
}

t_Vertex3D::t_Vertex3D(const sf::Vector3f& coord)
  : s_coord(coord)
{
}

t_Vertex3D::t_Vertex3D(const sf::Vector3f& coord, const sf::Color& color)
  : s_coord(coord), s_color(color)
{
}

t_Vertex3D::t_Vertex3D( const sf::Vector3f& coord,
			const sf::Vector2f& texcoord0 )
  : s_coord(coord), s_texcoord0(texcoord0)
{
}

t_Vertex3D::t_Vertex3D( const sf::Vector3f& coord,
			const sf::Vector2f& texcoord0,
			const sf::Vector2f& texcoord1 )
  : s_coord(coord), s_texcoord0(texcoord0), s_texcoord1(texcoord1)
{
}

t_Vertex3D::t_Vertex3D( const sf::Vector3f& coord,
			const sf::Color& color,
			const sf::Vector2f& texcoord0 )
  : s_coord(coord), s_color(color), s_texcoord0(texcoord0)
{
}

t_Vertex3D::t_Vertex3D( const sf::Vector3f& coord,
			const sf::Color& color,
			const sf::Vector2f& texcoord0,
			const sf::Vector2f& texcoord1 )
  : s_coord(coord), s_color(color),
    s_texcoord0(texcoord0), s_texcoord1(texcoord1)
{
}


////////////////////////////////////////////////////////////


std::vector<t_Vertex3D> *	GL_RawRenderer::m_pVertices = NULL;


////////////////////////////////////////////////////////////


void	GL_RawRenderer::Create()
{
  if (m_pVertices == NULL)
    {
      m_pVertices = new std::vector<t_Vertex3D>;
      m_pVertices->reserve(1000);
    }
}

void	GL_RawRenderer::Destroy()
{
  delete m_pVertices, m_pVertices = NULL;
}

////////////////////////////////////////////////////////////

void	GL_RawRenderer::Clear()
{
  if (m_pVertices != NULL)
    m_pVertices->clear();
}

void	GL_RawRenderer::AddVertex(const t_Vertex3D& vertex)
{
  if (m_pVertices != NULL)
    m_pVertices->push_back( vertex );
}

void	GL_RawRenderer::Render( GLenum primitive, int flags /*= 0*/,
				sf::Texture* pT0 /*= NULL*/,
				sf::Texture* pT1 /*= NULL*/ )
{

  bool	tex0 = (flags & eTexCoord0 && pT0);
  bool	tex1 = (flags & eTexCoord1 && pT1);

  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer( 3, GL_FLOAT, sizeof(t_Vertex3D), &(m_pVertices->at(0).s_coord.x) );

  if (flags & eColor)
    {
      glEnableClientState(GL_COLOR_ARRAY);
      glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(t_Vertex3D), &(m_pVertices->at(0).s_color.r) );
    }

  if (flags & eNormal)
    {
      glEnableClientState(GL_NORMAL_ARRAY);
      glNormalPointer( GL_FLOAT, sizeof(t_Vertex3D), &(m_pVertices->at(0).s_normal.x) );
    }

  // if (flags & eTexCoord0)
  //   {
  // 	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  // 	glTexCoordPointer( 2, GL_FLOAT, sizeof(t_Vertex3D), &(m_pVertices->at(0).s_texcoord0.x) );
  //   }



  if (tex1)
    {
      glClientActiveTexture(GL_TEXTURE1);
      glActiveTextureARB(GL_TEXTURE1_ARB);

      glEnableClientState(GL_TEXTURE_COORD_ARRAY);

      glEnable(GL_TEXTURE_2D);
      sf::Texture::bind( pT1 );

      glTexCoordPointer( 2, GL_FLOAT, sizeof(t_Vertex3D), &(m_pVertices->at(0).s_texcoord1.x) );
    }

  if (tex0)
    {
      glClientActiveTexture(GL_TEXTURE0);
      glActiveTextureARB(GL_TEXTURE0_ARB);

      glEnableClientState(GL_TEXTURE_COORD_ARRAY);

      glEnable(GL_TEXTURE_2D);
      sf::Texture::bind( pT0 );

      glTexCoordPointer( 2, GL_FLOAT, sizeof(t_Vertex3D), &(m_pVertices->at(0).s_texcoord0.x) );
    }



  glDrawArrays( primitive, 0, m_pVertices->size() );


  // if (flags & eTexCoord)
  //   glDisableClientState(GL_TEXTURE_COORD_ARRAY);

  if (tex1)
    {
      glClientActiveTexture(GL_TEXTURE1);
      glActiveTexture(GL_TEXTURE1);

      glDisable(GL_TEXTURE_2D);
      glDisableClientState( GL_TEXTURE_COORD_ARRAY );
    }

  if (tex0 || tex1)
    {
      glClientActiveTexture( GL_TEXTURE0 );
      glActiveTexture(GL_TEXTURE0);
    }

  if (tex0)
    {
      glDisable(GL_TEXTURE_2D);
      glDisableClientState( GL_TEXTURE_COORD_ARRAY );
    }



  if (flags & eNormal)
    glDisableClientState(GL_NORMAL_ARRAY);

  if (flags & eColor)
    glDisableClientState(GL_COLOR_ARRAY);

  glDisableClientState(GL_VERTEX_ARRAY);
}
