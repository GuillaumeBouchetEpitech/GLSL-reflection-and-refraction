

#include "GL_Shader.hpp"

#include "Exception.hpp"

// #include <SFML/Graphics/Texture.hpp>
// #include <SFML/Graphics/GLCheck.hpp>
// #include <SFML/System/InputStream.hpp>
// #include <SFML/System/Err.hpp>
#include <fstream>
#include <vector>


namespace
{
  // Retrieve the maximum number of texture units available
  GLint getMaxTextureUnits()
  {
    GLint maxUnits;
    glGetIntegerv(GL_MAX_TEXTURE_COORDS_ARB, &maxUnits);
    return maxUnits;
  }

  // Read the contents of a file into an array of char
  bool getFileContents(const std::string& filename, std::vector<char>& buffer)
  {
    std::ifstream file(filename.c_str(), std::ios_base::binary);
    if (file)
      {
	file.seekg(0, std::ios_base::end);
	std::streamsize size = file.tellg();
	if (size > 0)
	  {
	    file.seekg(0, std::ios_base::beg);
	    buffer.resize(static_cast<std::size_t>(size));
	    file.read(&buffer[0], size);
	  }
	buffer.push_back('\0');
	return true;
      }
    else
      {
	return false;
      }
  }

  // // Read the contents of a stream into an array of char
  // bool getStreamContents(sf::InputStream& stream, std::vector<char>& buffer)
  // {
  //     bool success = true;
  //     sf::Int64 size = stream.getSize();
  //     if (size > 0)
  //     {
  //         buffer.resize(static_cast<std::size_t>(size));
  //         stream.seek(0);
  //         sf::Int64 read = stream.read(&buffer[0], size);
  //         success = (read == size);
  //     }
  //     buffer.push_back('\0');
  //     return success;
  // }

}


// namespace
// {
// ////////////////////////////////////////////////////////////
// GL_Shader::CurrentTextureType GL_Shader::CurrentTexture;


////////////////////////////////////////////////////////////
GL_Shader::GL_Shader() :
  m_shaderProgram (0),
  m_currentTexture(-1),
  m_textures      (),
  m_params        ()
{
}


////////////////////////////////////////////////////////////
GL_Shader::~GL_Shader()
{
  // ensureGlContext();

  // Destroy effect program
  if (m_shaderProgram)
    glDeleteObjectARB(m_shaderProgram);
}


////////////////////////////////////////////////////////////
bool GL_Shader::loadFromFile(const std::string& filename, Type type)
{
  // Read the file
  std::vector<char> shader;
  if (!getFileContents(filename, shader))
    throw Exception_Ex("Failed to open a shader file");
    // {
    //   std::cerr << "Failed to open shader file \"" << filename << "\"" << std::endl;
    //   return false;
    // }

  // Compile the shader program
  if (type == Vertex)
    return compile(&shader[0], NULL);
  else
    return compile(NULL, &shader[0]);
}


////////////////////////////////////////////////////////////
bool GL_Shader::loadFromFile(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename)
{
  // Read the vertex shader file
  std::vector<char> vertexShader;
  if (!getFileContents(vertexShaderFilename, vertexShader))
    throw Exception_Ex("Failed to open a vertexshader file");
    // {
    //   std::cerr << "Failed to open vertex shader file \"" << vertexShaderFilename << "\"" << std::endl;
    //   return false;
    // }

  // Read the fragment shader file
  std::vector<char> fragmentShader;
  if (!getFileContents(fragmentShaderFilename, fragmentShader))
    throw Exception_Ex("Failed to open a fragmentshader file");
    // {
    //   std::cerr << "Failed to open fragment shader file \"" << fragmentShaderFilename << "\"" << std::endl;
    //   return false;
    // }

  // Compile the shader program
  return compile(&vertexShader[0], &fragmentShader[0]);
}

/**

////////////////////////////////////////////////////////////
bool Shader::loadFromMemory(const std::string& shader, Type type)
{
// Compile the shader program
if (type == Vertex)
return compile(shader.c_str(), NULL);
else
return compile(NULL, shader.c_str());
}


////////////////////////////////////////////////////////////
bool Shader::loadFromMemory(const std::string& vertexShader, const std::string& fragmentShader)
{
// Compile the shader program
return compile(vertexShader.c_str(), fragmentShader.c_str());
}


////////////////////////////////////////////////////////////
bool Shader::loadFromStream(InputStream& stream, Type type)
{
// Read the shader code from the stream
std::vector<char> shader;
if (!getStreamContents(stream, shader))
{
std::cerr << "Failed to read shader from stream" << std::endl;
return false;
}

// Compile the shader program
if (type == Vertex)
return compile(&shader[0], NULL);
else
return compile(NULL, &shader[0]);
}


////////////////////////////////////////////////////////////
bool Shader::loadFromStream(InputStream& vertexShaderStream, InputStream& fragmentShaderStream)
{
// Read the vertex shader code from the stream
std::vector<char> vertexShader;
if (!getStreamContents(vertexShaderStream, vertexShader))
{
std::cerr << "Failed to read vertex shader from stream" << std::endl;
return false;
}

// Read the fragment shader code from the stream
std::vector<char> fragmentShader;
if (!getStreamContents(fragmentShaderStream, fragmentShader))
{
std::cerr << "Failed to read fragment shader from stream" << std::endl;
return false;
}

// Compile the shader program
return compile(&vertexShader[0], &fragmentShader[0]);
}

//*/

////////////////////////////////////////////////////////////

void	GL_Shader::setAttrib( const std::string& name, GLint size, GLenum type,
			      GLboolean normalized, GLsizei stride,
			      const GLvoid * pointer )
{
  if (m_shaderProgram)
    {
      // Enable program
      GLhandleARB program = glGetHandleARB(GL_PROGRAM_OBJECT_ARB);
      glUseProgramObjectARB(m_shaderProgram);

      // Get parameter location and assign it new values
      GLint location = getAttribLocation(name);
      if (location != -1)
	{
	  glVertexAttribPointer( location, size, type, normalized, stride, pointer );
	  glEnableVertexAttribArray( location );
	}

      // Disable program
      glUseProgramObjectARB(program);
    }
  else
    throw Exception_Ex("no valid shaderProgram");
}

////////////////////////////////////////////////////////////
void GL_Shader::setParameter(const std::string& name, float x)
{
  if (m_shaderProgram)
    {
      // ensureGlContext();

      // Enable program
      GLhandleARB program = glGetHandleARB(GL_PROGRAM_OBJECT_ARB);
      glUseProgramObjectARB(m_shaderProgram);

      // Get parameter location and assign it new values
      GLint location = getParamLocation(name);
      if (location != -1)
	glUniform1fARB(location, x);

      // Disable program
      glUseProgramObjectARB(program);
    }
  else
    throw Exception_Ex("no valid shaderProgram");
}


////////////////////////////////////////////////////////////
void GL_Shader::setParameter(const std::string& name, float x, float y)
{
  if (m_shaderProgram)
    {
      // ensureGlContext();

      // Enable program
      GLhandleARB program = glGetHandleARB(GL_PROGRAM_OBJECT_ARB);
      glUseProgramObjectARB(m_shaderProgram);

      // Get parameter location and assign it new values
      GLint location = getParamLocation(name);
      if (location != -1)
	glUniform2fARB(location, x, y);

      // Disable program
      glUseProgramObjectARB(program);
    }
  else
    throw Exception_Ex("no valid shaderProgram");
}


////////////////////////////////////////////////////////////
void GL_Shader::setParameter(const std::string& name, float x, float y, float z)
{
  if (m_shaderProgram)
    {
      // ensureGlContext();

      // Enable program
      GLhandleARB program = glGetHandleARB(GL_PROGRAM_OBJECT_ARB);
      glUseProgramObjectARB(m_shaderProgram);

      // Get parameter location and assign it new values
      GLint location = getParamLocation(name);
      if (location != -1)
	glUniform3fARB(location, x, y, z);

      // Disable program
      glUseProgramObjectARB(program);
    }
  else
    throw Exception_Ex("no valid shaderProgram");
}


////////////////////////////////////////////////////////////
void GL_Shader::setParameter(const std::string& name, float x, float y, float z, float w)
{
  if (m_shaderProgram)
    {
      // ensureGlContext();

      // Enable program
      GLhandleARB program = glGetHandleARB(GL_PROGRAM_OBJECT_ARB);
      glUseProgramObjectARB(m_shaderProgram);

      // Get parameter location and assign it new values
      GLint location = getParamLocation(name);
      if (location != -1)
	glUniform4fARB(location, x, y, z, w);

      // Disable program
      glUseProgramObjectARB(program);
    }
  else
    throw Exception_Ex("no valid shaderProgram");
}


////////////////////////////////////////////////////////////
void GL_Shader::setParameter(const std::string& name, const sf::Vector2f& v)
{
  setParameter(name, v.x, v.y);
}


////////////////////////////////////////////////////////////
void GL_Shader::setParameter(const std::string& name, const sf::Vector3f& v)
{
  setParameter(name, v.x, v.y, v.z);
}


////////////////////////////////////////////////////////////
void GL_Shader::setParameter(const std::string& name, const sf::Color& color)
{
  setParameter(name, color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f);
}


////////////////////////////////////////////////////////////
// void GL_Shader::setParameter(const std::string& name, const sf::Transform& transform)
// void GL_Shader::setParameter(const std::string& name, const GL_Matrix& mat)
void GL_Shader::setParameter(const std::string& name, const GL_Matrix& mat, unsigned int square_size)
{
  if (m_shaderProgram)
    {
      // ensureGlContext();

      // Enable program
      GLhandleARB program = glGetHandleARB(GL_PROGRAM_OBJECT_ARB);
      glUseProgramObjectARB(m_shaderProgram);

      // Get parameter location and assign it new values
      GLint location = getParamLocation(name);
      if (location != -1)
	// glCheck(glUniformMatrix4fvARB(location, 1, GL_FALSE, transform.getMatrix()));
	// glUniformMatrix4fvARB( location, 1, GL_FALSE, transform.Ptr() );

	// glUniformMatrix4fvARB( location, 1, GL_FALSE, mat.Ptr() );

	{
	  if (square_size == 4)
	    glUniformMatrix4fvARB( location, 1, GL_FALSE, mat.Ptr() );
	  else if (square_size == 3)
	    {
	      float	pMat3x3[9];

	      // pMat3x3[0] = mat[ 0];
	      // pMat3x3[1] = mat[ 1];
	      // pMat3x3[2] = mat[ 2];

	      // pMat3x3[3] = mat[ 4];
	      // pMat3x3[4] = mat[ 5];
	      // pMat3x3[5] = mat[ 6];

	      // pMat3x3[6] = mat[ 8];
	      // pMat3x3[7] = mat[ 9];
	      // pMat3x3[8] = mat[10];

	      for (int i = 0; i < 9; ++i)
		pMat3x3[i] = mat( i % 3, i / 3);

	      // pMat3x3[0] = mat(0, 0);
	      // pMat3x3[1] = mat(0, 1);
	      // pMat3x3[2] = mat(0, 2);

	      // pMat3x3[3] = mat(1, 0);
	      // pMat3x3[4] = mat(1, 1);
	      // pMat3x3[5] = mat(1, 2);

	      // pMat3x3[6] = mat(2, 0);
	      // pMat3x3[7] = mat(2, 1);
	      // pMat3x3[8] = mat(2, 2);

	      glUniformMatrix3fvARB( location, 1, GL_FALSE, pMat3x3 );
	    }
	}

      // Disable program
      glUseProgramObjectARB(program);
    }
  else
    throw Exception_Ex("no valid shaderProgram");
}


////////////////////////////////////////////////////////////
// void GL_Shader::setParameter(const std::string& name, const Texture& texture)
void GL_Shader::setParameter(const std::string& name, const GL_Texture& texture)
{
  if (m_shaderProgram)
    {
      // ensureGlContext();

      // Find the location of the variable in the shader
      int location = getParamLocation(name);
      if (location != -1)
	{
	  // Store the location -> texture mapping
	  TextureTable::iterator it = m_textures.find(location);

	  if (it == m_textures.end())
	    {
	      // New entry, make sure there are enough texture units
	      static const GLint maxUnits = getMaxTextureUnits();

	      std::size_t	curr_size = m_textures.size() + m_cubemaps.size() + 1;

	      // if (m_textures.size() + 1 >= static_cast<std::size_t>(maxUnits))
	      if ( curr_size >= static_cast<std::size_t>(maxUnits) )
		{
		  std::cerr << "Impossible to use texture \"" << name
			    << "\" for shader: all available texture units are used"
			    << std::endl;
		  return;
		}

	      m_textures[ location ] = &texture;
	    }
	  else
	    {
	      // Location already used, just replace the texture
	      it->second = &texture;
	    }
	}
    }
  else
    throw Exception_Ex("no valid shaderProgram");
}

void GL_Shader::setParameter(const std::string& name, const GL_CubeMap& cubemap)
{
  if (m_shaderProgram)
    {
      // ensureGlContext();

      // Find the location of the variable in the shader
      int location = getParamLocation(name);
      if (location != -1)
	{
	  // Store the location -> texture mapping
	  CubeMapTable::iterator it = m_cubemaps.find(location);

	  if (it == m_cubemaps.end())
	    {
	      // New entry, make sure there are enough texture units
	      static const GLint maxUnits = getMaxTextureUnits();

	      std::size_t	curr_size = m_textures.size() + m_cubemaps.size() + 1;

	      // if (m_textures.size() + 1 >= static_cast<std::size_t>(maxUnits))
	      if ( curr_size >= static_cast<std::size_t>(maxUnits) )
	      	{
	      	  std::cerr << "Impossible to use texture \"" << name
	      		    << "\" for shader: all available texture units are used"
	      		    << std::endl;
	      	  return;
	      	}

	      m_cubemaps[ location ] = &cubemap;
	    }
	  else
	    {
	      // Location already used, just replace the texture
	      it->second = &cubemap;
	    }
	}
    }
  else
    throw Exception_Ex("no valid shaderProgram");
}


////////////////////////////////////////////////////////////
void GL_Shader::setParameter(const std::string& name, CurrentTextureType)
{
  if (m_shaderProgram)
    {
      // ensureGlContext();

      // Find the location of the variable in the shader
      m_currentTexture = getParamLocation(name);
    }
  else
    throw Exception_Ex("no valid shaderProgram");
}


void GL_Shader::setParameter(const std::string& name, float mat[9])
{
  if (m_shaderProgram)
    {
      // ensureGlContext();

      // Enable program
      GLhandleARB program = glGetHandleARB(GL_PROGRAM_OBJECT_ARB);
      glUseProgramObjectARB(m_shaderProgram);

      // Get parameter location and assign it new values
      GLint location = getParamLocation(name);
      if (location != -1)
	// glCheck(glUniformMatrix4fvARB(location, 1, GL_FALSE, transform.getMatrix()));
	// glUniformMatrix4fvARB( location, 1, GL_FALSE, transform.Ptr() );
	// glUniformMatrix4fvARB( location, 1, GL_FALSE, mat.Ptr() );
	glUniformMatrix3fvARB( location, 1, GL_FALSE, mat );

      // Disable program
      glUseProgramObjectARB(program);
    }
  else
    throw Exception_Ex("no valid shaderProgram");
}


////////////////////////////////////////////////////////////
void GL_Shader::bind(const GL_Shader* shader)
{
  // ensureGlContext();

  if (shader && shader->m_shaderProgram)
    {
      // Enable the program
      glUseProgramObjectARB( shader->m_shaderProgram );

      // Bind the textures
      shader->bindTextures();

      // Bind the current texture
      if (shader->m_currentTexture != -1)
      	glUniform1iARB(shader->m_currentTexture, 0);
    }
  else
    {
      // Bind no shader
      glUseProgramObjectARB( 0 );
    }
}


////////////////////////////////////////////////////////////
bool GL_Shader::isAvailable()
{
  // ensureGlContext();

  // Make sure that GLEW is initialized
  // priv::ensureGlewInit();

  return GLEW_ARB_shading_language_100 &&
    GLEW_ARB_shader_objects       &&
    GLEW_ARB_vertex_shader        &&
    GLEW_ARB_fragment_shader;
}


////////////////////////////////////////////////////////////
bool GL_Shader::compile(const char* vertexShaderCode, const char* fragmentShaderCode)
{
  // ensureGlContext();

  // First make sure that we can use shaders
  if (!isAvailable())
    {
      std::cerr << "Failed to create a shader: your system doesn't support shaders "
	    << "(you should test Shader::isAvailable() before trying to use the Shader class)" << std::endl;
      return false;
    }

  // Destroy the shader if it was already created
  if (m_shaderProgram)
    glDeleteObjectARB(m_shaderProgram);

  // Reset the internal state
  m_currentTexture = -1;
  m_textures.clear();
  m_params.clear();

  // Create the program
  m_shaderProgram = glCreateProgramObjectARB();

  // Create the vertex shader if needed
  if (vertexShaderCode)
    {
      // Create and compile the shader
      GLhandleARB vertexShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
      glShaderSourceARB(vertexShader, 1, &vertexShaderCode, NULL);
      glCompileShaderARB(vertexShader);

      // Check the compile log
      GLint success;
      glGetObjectParameterivARB(vertexShader, GL_OBJECT_COMPILE_STATUS_ARB, &success);
      if (success == GL_FALSE)
	{
	  char log[1024];
	  glGetInfoLogARB(vertexShader, sizeof(log), 0, log);
	  std::cerr << "Failed to compile vertex shader:" << std::endl
		<< log << std::endl;
	  glDeleteObjectARB(vertexShader);
	  glDeleteObjectARB(m_shaderProgram);
	  m_shaderProgram = 0;
	  return false;
	}

      // Attach the shader to the program, and delete it (not needed anymore)
      glAttachObjectARB(m_shaderProgram, vertexShader);
      glDeleteObjectARB(vertexShader);
    }

  // Create the fragment shader if needed
  if (fragmentShaderCode)
    {
      // Create and compile the shader
      GLhandleARB fragmentShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
      glShaderSourceARB(fragmentShader, 1, &fragmentShaderCode, NULL);
      glCompileShaderARB(fragmentShader);

      // Check the compile log
      GLint success;
      glGetObjectParameterivARB(fragmentShader, GL_OBJECT_COMPILE_STATUS_ARB, &success);
      if (success == GL_FALSE)
	{
	  char log[1024];
	  glGetInfoLogARB(fragmentShader, sizeof(log), 0, log);
	  std::cerr << "Failed to compile fragment shader:" << std::endl
		<< log << std::endl;
	  glDeleteObjectARB(fragmentShader);
	  glDeleteObjectARB(m_shaderProgram);
	  m_shaderProgram = 0;
	  return false;
	}

      // Attach the shader to the program, and delete it (not needed anymore)
      glAttachObjectARB(m_shaderProgram, fragmentShader);
      glDeleteObjectARB(fragmentShader);
    }

  // Link the program
  glLinkProgramARB(m_shaderProgram);

  // Check the link log
  GLint success;
  glGetObjectParameterivARB(m_shaderProgram, GL_OBJECT_LINK_STATUS_ARB, &success);
  if (success == GL_FALSE)
    {
      char log[1024];
      glGetInfoLogARB(m_shaderProgram, sizeof(log), 0, log);
      std::cerr << "Failed to link shader:" << std::endl
		<< log << std::endl;
      glDeleteObjectARB(m_shaderProgram);
      m_shaderProgram = 0;
      return false;
    }

  // Force an OpenGL flush, so that the shader will appear updated
  // in all contexts immediately (solves problems in multi-threaded apps)
  glFlush();

  return true;
}


////////////////////////////////////////////////////////////
void GL_Shader::bindTextures() const
{
  TextureTable::const_iterator it = m_textures.begin();

  GLsizei	texture_unit = 0;

  for (std::size_t i = 0; i < m_textures.size(); ++i)
    {
      // GLint index = static_cast<GLsizei>(i + 1);
      // glUniform1iARB(it->first, index);
      // glActiveTextureARB(GL_TEXTURE0_ARB + index);

      glUniform1iARB(it->first, texture_unit);
      glActiveTextureARB(GL_TEXTURE0_ARB + texture_unit);

      ++texture_unit;

      // GL_Texture::bind(it->second);
      it->second->Bind();

      {
	GLint	param = (it->second->IsRepeated() ? GL_REPEAT : GL_CLAMP);

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, param );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, param );
      }

      {
	GLint	param = (it->second->IsSmooth() ? GL_LINEAR : GL_NEAREST);

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param );
      }

      ++it;
    }

  /**/
  CubeMapTable::const_iterator it2 = m_cubemaps.begin();

  for (std::size_t i = 0; i < m_cubemaps.size(); ++i)
    {
      // GLint index = static_cast<GLsizei>(i + 1);
      // glUniform1iARB(it->first, index);
      // glActiveTextureARB(GL_TEXTURE0_ARB + index);

      glUniform1iARB(it->first, texture_unit);
      glActiveTextureARB(GL_TEXTURE0_ARB + texture_unit);

      ++texture_unit;

      // GL_Texture::bind(it->second);
      it2->second->Bind();

      glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
      glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
      glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
      glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

      // {
      // 	GLint	param = (it2->second->IsRepeated() ? GL_REPEAT : GL_CLAMP);

      // 	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, param );
      // 	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, param );
      // }

      // {
      // 	GLint	param = (it2->second->IsSmooth() ? GL_LINEAR : GL_NEAREST);

      // 	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param );
      // 	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param );
      // }

      ++it2;
    }
  //*/

  // Make sure that the texture unit which is left active is the number 0
  glActiveTextureARB(GL_TEXTURE0_ARB);
}

////////////////////////////////////////////////////////////
int GL_Shader::getAttribLocation(const std::string& name)
{
  // Check the cache
  AttribTable::const_iterator it = m_attribs.find(name);

  if (it != m_attribs.end())
    {
      // Already in cache, return it
      return it->second;
    }
  else
    {
      // Not in cache, request the location from OpenGL
      int location = glGetAttribLocation( m_shaderProgram, name.c_str() );

      if (location != -1)
	{
	  // Location found: add it to the cache
	  m_attribs.insert(std::make_pair(name, location));
	}
      else
	{
	  // Error: location not found
	  std::cerr << "Attrib \"" << name << "\" not found in shader" << std::endl;
	}

      return location;
    }
}

////////////////////////////////////////////////////////////
int GL_Shader::getParamLocation(const std::string& name)
{
  // Check the cache
  ParamTable::const_iterator it = m_params.find(name);

  if (it != m_params.end())
    {
      // Already in cache, return it
      return it->second;
    }
  else
    {
      // Not in cache, request the location from OpenGL
      int location = glGetUniformLocationARB( m_shaderProgram, name.c_str() );

      if (location != -1)
	{
	  // Location found: add it to the cache
	  m_params.insert(std::make_pair(name, location));
	}
      else
	{
	  // Error: location not found
	  std::cerr << "Parameter \"" << name << "\" not found in shader" << std::endl;
	}

      return location;
    }
}

// } // namespace sf
