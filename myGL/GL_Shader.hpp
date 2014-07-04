

#ifndef GL_SHADER_HPP
#define GL_SHADER_HPP


#include "Header.hpp"

#include "GL_Matrix.hpp"
#include "GL_Texture.hpp"
#include "GL_CubeMap.hpp"

#include <map>
#include <string>


// class InputStream;
class Texture;

class GL_Shader
{
public :

  enum Type
    {
      Vertex,  ///< Vertex shader
      Fragment ///< Fragment (pixel) shader
    };

  struct CurrentTextureType {};
  static CurrentTextureType CurrentTexture;

public :

  GL_Shader();
  ~GL_Shader();

public :

  bool loadFromFile( const std::string& filename, Type type );
  bool loadFromFile( const std::string& vertexShaderFilename,
		     const std::string& fragmentShaderFilename );
  // bool loadFromMemory(const std::string& shader, Type type);
  // bool loadFromMemory(const std::string& vertexShader, const std::string& fragmentShader);
  // bool loadFromStream(InputStream& stream, Type type);
  // bool loadFromStream(InputStream& vertexShaderStream, InputStream& fragmentShaderStream);

public :

  void setAttrib( const std::string& name, GLint size, GLenum type, GLboolean normalized,
		  GLsizei stride, const GLvoid * pointer );

public :

  void setParameter(const std::string& name, float x);
  void setParameter(const std::string& name, float x, float y);
  void setParameter(const std::string& name, float x, float y, float z);
  void setParameter(const std::string& name, float x, float y, float z, float w);
  void setParameter(const std::string& name, const sf::Vector2f& vector);
  void setParameter(const std::string& name, const sf::Vector3f& vector);
  void setParameter(const std::string& name, const sf::Color& color);

  // void setParameter(const std::string& name, const GL_Matrix& mat);
  void setParameter(const std::string& name, const GL_Matrix& mat, unsigned int square_size = 4);

  void setParameter(const std::string& name, const GL_Texture& texture);

  void setParameter(const std::string& name, const GL_CubeMap& cubemap);

  void setParameter(const std::string& name, CurrentTextureType);


  void setParameter(const std::string& name, float mat[9]);

public :

  static void bind(const GL_Shader* shader);
  static bool isAvailable();

private :

  bool compile(const char* vertexShaderCode, const char* fragmentShaderCode);
  void bindTextures() const;
  int getParamLocation(const std::string& name);
  int getAttribLocation(const std::string& name);

private :

  // typedef std::map<int, const Texture*> TextureTable;
  typedef std::map<int, const GL_Texture*> TextureTable;
  typedef std::map<int, const GL_CubeMap*> CubeMapTable;

  typedef std::map<std::string, int> ParamTable;
  typedef std::map<std::string, int> AttribTable;

private :

  unsigned int	m_shaderProgram;
  int		m_currentTexture;
  TextureTable	m_textures;
  CubeMapTable	m_cubemaps;
  ParamTable	m_params;
  AttribTable	m_attribs;

};


#endif

