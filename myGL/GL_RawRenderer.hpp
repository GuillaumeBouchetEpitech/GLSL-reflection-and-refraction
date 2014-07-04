

#ifndef	GL_RAWRENDERER_HPP
#define	GL_RAWRENDERER_HPP


#include "Header.hpp"

#include <vector>


struct	t_Vertex3D
{
public :

  sf::Vector3f	s_coord;
  sf::Color	s_color;
  sf::Vector3f	s_normal;
  sf::Vector2f	s_texcoord0;
  sf::Vector2f	s_texcoord1;

public :

  t_Vertex3D();

  t_Vertex3D(const sf::Vector3f& coord);

  t_Vertex3D(const sf::Vector3f& coord, const sf::Color& color);

  t_Vertex3D( const sf::Vector3f& coord, const sf::Vector2f& texcoord0 );

  t_Vertex3D( const sf::Vector3f& coord,
	      const sf::Vector2f& texcoord0, const sf::Vector2f& texcoord1 );

  t_Vertex3D( const sf::Vector3f& coord, const sf::Color& color,
	      const sf::Vector2f& texcoord0 );

  t_Vertex3D( const sf::Vector3f& coord, const sf::Color& color,
	      const sf::Vector2f& texcoord0, const sf::Vector2f& texcoord1 );

};


class	GL_RawRenderer
{
public :

  enum {
    eColor =		1 << 0,
    eNormal =		1 << 1,
    eTexCoord0 =	1 << 2,
    eTexCoord1 =	1 << 3
  };

private :

  static std::vector<t_Vertex3D> *	m_pVertices;

public :

  static void	Create();
  static void	Destroy();

public :

  static void	Clear();

  static void	AddVertex(const t_Vertex3D& vertex);

  static void	Render( GLenum primitive, int flags = 0,
			sf::Texture* pT0 = NULL,
			sf::Texture* pT1 = NULL );

};


#endif

