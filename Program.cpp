

#include "Program.hpp"



#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>

#include <sstream>
#include <fstream>
#include <iomanip>



#define	D_SCREENSHOTS_PATH	"../screenshots"




static const sf::Vector2u	s_tex_size(128,128);

Program::Program()
 : m_FreeFly( m_RWindow ),
   m_Intruder_angle(0.0f)
{
}

Program::~Program()
{
}

//////////////////////////////////////

void	Program::Initialize_2D()
{
  { // window initialize

    sf::ContextSettings   settings;

    settings.depthBits = 24;
    // settings.stencilBits = 8;
    settings.stencilBits = 0;
    settings.antialiasingLevel = 4;
    settings.minorVersion = 1;
    settings.majorVersion = 2;

    m_RWindow.create( sf::VideoMode(800, 600, 32), "GLSL reflection and refraction",
		      sf::Style::Close | sf::Style::Resize, settings );

    m_RWindow.setFramerateLimit(60);
    m_RWindow.setVerticalSyncEnabled(true);

  } // /window initialize
}

void	Program::Initialize_3D()
{
  { // GLEW

    // glewExperimental = GL_TRUE;

    GLenum	ret_val = glewInit();

    if (ret_val != GLEW_OK)
      std::cerr << "Failed to initialize GLEW, " << glewGetErrorString( ret_val ) << std::endl;

  } // /GLEW

  glClearDepth(1.f);
  // glClearStencil(0);
  glClearColor(0.7f, 0.7f, 0.7f, 0.0f);

  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  m_FreeFly.Initialize();


  {

    m_pBackgroundSphere = GL_Geometry::CreateSphere( 300, 2 );



    m_CubeMap.SetTexture_FileName( GL_CubeMap::ePositive_x, "./res/cubemap/cm_pos_x.tga");
    m_CubeMap.SetTexture_FileName( GL_CubeMap::eNegative_x, "./res/cubemap/cm_neg_x.tga");
    m_CubeMap.SetTexture_FileName( GL_CubeMap::eNegative_y, "./res/cubemap/cm_pos_y.tga");
    m_CubeMap.SetTexture_FileName( GL_CubeMap::ePositive_y, "./res/cubemap/cm_neg_y.tga");
    m_CubeMap.SetTexture_FileName( GL_CubeMap::ePositive_z, "./res/cubemap/cm_pos_z.tga");
    m_CubeMap.SetTexture_FileName( GL_CubeMap::eNegative_z, "./res/cubemap/cm_neg_z.tga");

    m_CubeMap.Load();

    m_BackgroundSphereShader.loadFromFile( "./res/shaders/background.vert.glsl.c",
					   "./res/shaders/background.frag.glsl.c" );

  }



  {

    m_FresnelModelMatrix.Make_Identity();
    // m_pFresnelGeometry = GL_Geometry::CreateTorus( 0.0f, 180.0f,
				// 		   100.0f, 50.0f,70.0f,
				// 		   60, 60 );
    m_pFresnelGeometry = GL_Geometry::CreateTorus( 0.0f, 180.0f,
                                                50.0f, 15.0f, 20.0f,
                                                60, 60 );
    // m_pFresnelGeometry = GL_Geometry::CreateSphere( 25.0f, 3 );

    m_MainSphereShader.loadFromFile( "./res/shaders/glass.vert.glsl.c",
				     "./res/shaders/glass.frag.glsl.c" );

  }

  for ( int i = 0; i < GL_CubeMap::eCubeMapSens_Count; ++i )
    m_pTexture[ i ].Create( s_tex_size.x, s_tex_size.y );


  m_IntruderSphereShader.loadFromFile( "./res/shaders/basic.vert.glsl.c",
				       "./res/shaders/basic.frag.glsl.c" );

  m_pIntruderGeometry = GL_Geometry::CreateSphere( 30, 0 );


}

void	Program::Process_Events(sf::Event& event)
{
  if (event.type == sf::Event::Closed ||
      (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
    m_RWindow.close();

  //////////////////////

  else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F1)
    m_vbo_enabled = !m_vbo_enabled;

  else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F12)
    {

      /// get picture
      sf::Image	screenshot = m_RWindow.capture();


      bool	done = false;
      while (!done)
	{

	  static int	count = 0;

	  /// get name
	  std::stringstream	sstr;
	  sstr << D_SCREENSHOTS_PATH << "/screenshot_" << count << ".png";


	  std::string	str = sstr.str();


	  /// check if file exist
	  std::ifstream	infile( str.c_str() );
	  if (!infile.good())
	    {

	      /// save it
	      screenshot.saveToFile( str );

	      std::cerr << "Saved in : " << str << std::endl;

	      done = true;
	    }
	  else
	    std::cerr << "Not saved in : " << str << std::endl;


	  ++count;
	}

    }


  else if (event.type == sf::Event::Resized)
    {
      glViewport(0, 0, event.size.width, event.size.height);

      m_FreeFly.Resize();
    }

  //////////////////////

  else
    m_FreeFly.Event( event );

}

void	Program::Update(float elapsed_time)
{

  m_FreeFly.Update( elapsed_time );


  m_Intruder_angle += 0.01f;

}


void		Program::Render_3D_cubmap_update()
{

  struct	t_viewMatrix
  {
    sf::Vector3f	s_center, s_vertAxis;

    t_viewMatrix() {}
    t_viewMatrix( const sf::Vector3f& center,
		  const sf::Vector3f& vertAxis )
      : s_center(center), s_vertAxis(vertAxis)
    {
    }
  };


  sf::Vector3f	lookAt_eye(0,0,0);

  t_viewMatrix	lookAts[ GL_CubeMap::eCubeMapSens_Count ] = {

    t_viewMatrix( sf::Vector3f(+1,0,0), sf::Vector3f(0,-1,0) ),
    t_viewMatrix( sf::Vector3f(-1,0,0), sf::Vector3f(0,-1,0) ),

    t_viewMatrix( sf::Vector3f(0,+1,0), sf::Vector3f(0,0,+1) ),
    t_viewMatrix( sf::Vector3f(0,-1,0), sf::Vector3f(0,0,-1) ),

    t_viewMatrix( sf::Vector3f(0,0,+1), sf::Vector3f(0,-1,0) ),
    t_viewMatrix( sf::Vector3f(0,0,-1), sf::Vector3f(0,-1,0) )
  };


  GL_Matrix	Mat4x4_proj;
  GL_Matrix	Mat4x4_view;
  GL_Matrix	viewProjectionMatrix;

  Mat4x4_proj.Make_Perspective( 90.0, (GLdouble)s_tex_size.x / s_tex_size.y, 0.1,1000.0 );


  glEnable( GL_CULL_FACE );

  for (int i = 0; i < GL_CubeMap::eCubeMapSens_Count; ++i)
    {
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

      {
	glViewport( 0, 0, s_tex_size.x, s_tex_size.y );

	Mat4x4_view.Make_LookAt( lookAt_eye,
				 lookAts[ i ].s_center,
				 lookAts[ i ].s_vertAxis );

	viewProjectionMatrix = Mat4x4_proj * Mat4x4_view;
      }


      {

	Render_3D_IntruderSphere(viewProjectionMatrix);

	Render_3D_BackgroudSphere(viewProjectionMatrix);

      }

      glFlush();

      m_pTexture[ i ].DumpScreen();
    }

  glDisable( GL_CULL_FACE );


  m_CubeMap_dynamic.Load( m_pTexture );


  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport( 0, 0, m_RWindow.getSize().x, m_RWindow.getSize().y );

}




void	Program::Render_3D_BackgroudSphere(GL_Matrix& viewProjectionMatrix)
{
  glFrontFace( GL_CW );

  m_BackgroundSphereShader.setParameter( "u_viewProjectionMatrix", viewProjectionMatrix );

  m_BackgroundSphereShader.setParameter( "u_cubemap", m_CubeMap );
  // m_BackgroundSphereShader.setParameter( "u_cubemap", m_CubeMap_dynamic );

  m_BackgroundSphereShader.setAttrib( "a_vertex", 3, GL_FLOAT, GL_FALSE, 0,
				      &(m_pBackgroundSphere->m_Vertices[0].x) );

  GL_Shader::bind( &m_BackgroundSphereShader );
  {
    glDrawArrays( GL_TRIANGLES, 0, m_pBackgroundSphere->m_Vertices.size() );
  }
  GL_Shader::bind( NULL );

  glFrontFace( GL_CCW );
}

void	Program::Render_3D_FresnelTorus(GL_Matrix& viewProjectionMatrix)
{
  m_MainSphereShader.setParameter( "u_viewProjectionMatrix", viewProjectionMatrix );

  // m_MainSphereShader.setParameter( "u_cubemap", m_CubeMap );
  m_MainSphereShader.setParameter( "u_cubemap", m_CubeMap_dynamic );

  m_MainSphereShader.setAttrib( "a_vertex", 3, GL_FLOAT, GL_FALSE, 0,
				&(m_pFresnelGeometry->m_Vertices[0].x) );

  m_MainSphereShader.setAttrib( "a_normal", 3, GL_FLOAT, GL_FALSE, 0,
				&(m_pFresnelGeometry->m_Normales[0].x) );

  m_MainSphereShader.setParameter( "u_modelMatrix", m_FresnelModelMatrix );
  m_MainSphereShader.setParameter( "u_normalMatrix", m_FresnelModelMatrix, 3 );

  m_MainSphereShader.setParameter( "u_camera", m_FreeFly.GetPosition() );


  GL_Shader::bind( &m_MainSphereShader );
  {
    glDrawArrays( GL_TRIANGLES, 0, m_pFresnelGeometry->m_Vertices.size() );
  }
  GL_Shader::bind( NULL );

  m_CubeMap.UnBind();
}

void	Program::Render_3D_IntruderSphere(GL_Matrix& viewProjectionMatrix)
{
  sf::Vector3f	lightDir;
  sf::Vector3f	pos;

  for (int i = 0; i < 6; ++i)
    {

      {
	lightDir.x = -cosf( m_Intruder_angle + ((float)i * 6.28f / 6) );
	lightDir.y = -sinf( m_Intruder_angle + ((float)i * 6.28f / 6) );
	lightDir.z = 0.0f;

	pos.x = 100.0f * cosf( m_Intruder_angle + ((float)i * 6.28f / 6) );
	pos.y = 100.0f * sinf( m_Intruder_angle + ((float)i * 6.28f / 6) );
	pos.z = 0.0f;
      }

      GL_Matrix	modelMatrix;
      modelMatrix.Make_Translate( pos );

      GL_Matrix	modelViewProjectionMatrix = viewProjectionMatrix * modelMatrix;

      m_IntruderSphereShader.setParameter( "u_modelViewProjectionMatrix", modelViewProjectionMatrix );
      m_IntruderSphereShader.setParameter( "u_lightDir", lightDir );

      m_IntruderSphereShader.setAttrib( "a_vertex", 3, GL_FLOAT, GL_FALSE, 0,
					&(m_pIntruderGeometry->m_Vertices[0].x) );

      m_IntruderSphereShader.setAttrib( "a_normal", 3, GL_FLOAT, GL_FALSE, 0,
					&(m_pIntruderGeometry->m_Normales[0].x) );

      GL_Shader::bind( &m_IntruderSphereShader );
      {
	glDrawArrays( GL_TRIANGLES, 0, m_pIntruderGeometry->m_Vertices.size() );
      }
      GL_Shader::bind( NULL );

    }
}



void	Program::Render_3D()
{

  Render_3D_cubmap_update();


  GL_Matrix	viewProjectionMatrix = m_FreeFly.Get_ProjectionMatrix() * m_FreeFly.Get_ViewMatrix();


  glEnable( GL_CULL_FACE );


  Render_3D_IntruderSphere(viewProjectionMatrix);
  Render_3D_BackgroudSphere(viewProjectionMatrix);
  Render_3D_FresnelTorus(viewProjectionMatrix);


  glDisable( GL_CULL_FACE );


}

//////////////////////////////////////

void	Program::Run()
{
  Initialize_2D();
  Initialize_3D();

  sf::Clock	MainClock;

  while (m_RWindow.isOpen())
    {

      { // step

	sf::Event	event;

	while (m_RWindow.pollEvent( event ))
	  Process_Events( event );

	Update( MainClock.restart().asSeconds() );

      } // /step

      { // render

	Render_3D();

	glFlush();

	m_RWindow.display();

      } // /render

    } // while (RWindow.isOpen())

} // void Run()

