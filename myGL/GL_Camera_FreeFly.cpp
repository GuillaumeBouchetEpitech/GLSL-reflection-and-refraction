

#include "GL_Camera_FreeFly.hpp"


GL_Camera_FreeFly::GL_Camera_FreeFly(sf::RenderWindow& RWindow)
  : m_RWindow(RWindow)
{
  m_Position = sf::Vector3f(0, 0, 0);
  m_speed = 5;
  m_sensivity = 0.1;
  m_phi = 0;
  m_theta = 0;


}

void	GL_Camera_FreeFly::VectorsFromAngles()
{
  // sf::Vector3f	Up(0,0,1);
  sf::Vector3f	Up(0,-1,0);

  if (m_phi >= 89)
    m_phi = 89;
  else if (m_phi <= -89)
    m_phi = -89;

  if (m_theta > 360)
    m_theta -= 360;
  else if (m_theta < 0)
    m_theta += 360;

  float	upRadius = cosf((m_phi - 90.0f) * M_PI / 180);
  // Up.z = sinf( (m_phi - 90.0f) * M_PI / 180.0f);
  // Up.x = upRadius * cosf(m_theta * M_PI / 180.0f);
  // Up.y = upRadius * sinf(m_theta * M_PI / 180.0f);
  Up.x = upRadius * cosf(m_theta * M_PI / 180.0f);
  Up.z = upRadius * sinf(m_theta * M_PI / 180.0f);
  Up.y = sinf( (m_phi - 90.0f) * M_PI / 180.0f);

  float	forwardRadius = cosf(m_phi * M_PI / 180.0f);
  // m_Forward.z = sin(m_phi * M_PI / 180.0f);
  // m_Forward.x = forwardRadius * cosf(m_theta * M_PI / 180.0f);
  // m_Forward.y = forwardRadius * sinf(m_theta * M_PI / 180.0f);
  m_Forward.x = forwardRadius * cosf(m_theta * M_PI / 180.0f);
  m_Forward.z = forwardRadius * sinf(m_theta * M_PI / 180.0f);
  m_Forward.y = sin(m_phi * M_PI / 180.0f);

  m_Left.x = Up.y * m_Forward.z - Up.z * m_Forward.y;
  m_Left.y = Up.z * m_Forward.x - Up.x * m_Forward.z;
  m_Left.z = Up.x * m_Forward.y - Up.y * m_Forward.x;
  m_Left.x = -m_Left.x;
  m_Left.z = -m_Left.z;

  m_Target.x = m_Position.x + m_Forward.x;
  m_Target.y = m_Position.y + m_Forward.y;
  m_Target.z = m_Position.z + m_Forward.z;

  // m_modl.Make_LookAt( m_Position,
  // 		      m_Target,
  // 		      sf::Vector3f(0,0,1) );

  // m_Frustum.CalculateFrustum( m_proj, m_modl );

  m_Mat4x4_view.Make_LookAt( m_Position,
			     m_Target,
			     // sf::Vector3f(0,0,1) );
			     sf::Vector3f(0,-1,0) );

  m_Frustum.CalculateFrustum( m_Mat4x4_proj, m_Mat4x4_view );

}

void	GL_Camera_FreeFly::Initialize()
{
  Active();
}

void	GL_Camera_FreeFly::Resize()
{
  glMatrixMode(GL_PROJECTION);

  sf::Vector2u	WSize = m_RWindow.getSize();

  glViewport(0,0, WSize.x, WSize.y);

  double	aspectRatio = static_cast<double>(WSize.x) / WSize.y;

  // m_proj.Make_Perspective( 70.0, aspectRatio, 0.1,500.0 );
  // m_proj.Use_Load();
  m_Mat4x4_proj.Make_Perspective( 70.0, aspectRatio, 0.1,1000.0 );
  m_Mat4x4_proj.Use_Load();

  glMatrixMode(GL_MODELVIEW);
}

void	GL_Camera_FreeFly::Active()
{
  VectorsFromAngles();

  sf::Vector2i	middle(m_RWindow.getSize().x / 2, m_RWindow.getSize().y / 2);
  sf::Mouse::setPosition(middle, m_RWindow);

  SetActive(true);

  Resize();
}

void	GL_Camera_FreeFly::Desactive()
{
  SetActive(false);
}

void	GL_Camera_FreeFly::Event(const sf::Event& Event)
{
  if ( Event.type == sf::Event::KeyPressed &&
       Event.key.code == sf::Keyboard::Space )
    {
      if (m_active)
	Desactive();
      else
	Active();
    }

  else if (Event.type == sf::Event::LostFocus)
    Desactive();

  else if (m_active)
    {
      if (Event.type == sf::Event::MouseWheelMoved)
	m_Position.z += ((Event.mouseWheel.delta == 1) ? (m_speed) : (-m_speed));
    }
}

void	GL_Camera_FreeFly::Update(const float elapsedTime)
{
  static_cast<void>(elapsedTime);

  if (m_active)
    {
      sf::Vector2i	middle(m_RWindow.getSize().x / 2, m_RWindow.getSize().y / 2);

      sf::Vector2f	dec(sf::Mouse::getPosition(m_RWindow).x - middle.x,
			    sf::Mouse::getPosition(m_RWindow).y - middle.y);

      m_theta	-= (dec.x) * m_sensivity;
      // m_phi	-= (dec.y) * m_sensivity;
      m_phi	+= (dec.y) * m_sensivity;

      VectorsFromAngles();

      sf::Mouse::setPosition(middle, m_RWindow);

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	m_Position -= m_Left * m_speed;
      else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	m_Position += m_Left * m_speed;

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	m_Position +=  m_Forward * m_speed;
      else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	m_Position -=  m_Forward * m_speed;

      m_Target.x = m_Position.x + m_Forward.x;
      m_Target.y = m_Position.y + m_Forward.y;
      m_Target.z = m_Position.z + m_Forward.z;
    }
}

void	GL_Camera_FreeFly::Look() const
{
  // m_modl.Use_Load();
  m_Mat4x4_view.Use_Load();
}


void	GL_Camera_FreeFly::SetActive(bool active)
{
  m_active = active;

  // if (m_active == true)
  //   m_RWindow.ShowMouseCursor(false);
  // else
  //   m_RWindow.ShowMouseCursor(true);
}

////////////////////////////////////////////////////
////////////////////////////////////////////////////


bool	GL_Camera_FreeFly::Get2DPoint(const sf::Vector3f& Target, sf::Vector2f& Position)
{
  unsigned int	Width = m_RWindow.getSize().x;
  unsigned int	Height = m_RWindow.getSize().y;

  int	viewport[4];

  viewport[0] = 0;
  viewport[1] = 0;
  viewport[2] = Width;
  viewport[3] = Height;

  float	in[4];
  float	out[4];

  in[0] = Target.x;
  in[1] = Target.y;
  in[2] = Target.z;
  in[3] = 1;

  // m_modl.PreMult(in, out);
  // m_proj.PreMult(out, in);
  m_Mat4x4_view.PreMult( in, out );
  m_Mat4x4_proj.PreMult( out, in );

  in[0] /= in[3];
  in[1] /= in[3];
  in[2] /= in[3];

  in[0] = in[0] * 0.5f + 0.5f;
  in[1] = in[1] * 0.5f + 0.5f;
  in[2] = in[2] * 0.5f + 0.5f;

  in[0] = in[0] * viewport[2] + viewport[0];
  in[1] = in[1] * viewport[3] + viewport[1];

  Position.x = in[0];
  Position.y = (float)viewport[3] - in[1];

  return (true);
}


void	GL_Camera_FreeFly::Do_BillBoard(const sf::Vector3f& Object_Pos)
{

  //// GET THETA

  float	theta = atan2f( Object_Pos.y - m_Position.y,
			Object_Pos.x - m_Position.x );

  //// GET PHI

#define	D_SQ(v) ((v) * (v))
#define	D_LENGTH(v1, v2) sqrtf( D_SQ((v1).x - (v2).x) + D_SQ((v1).y - (v2).y) )

  sf::Vector2f	Profile_Pos;

  Profile_Pos.x = D_LENGTH( Object_Pos, m_Position );
  Profile_Pos.y = m_Position.z - Object_Pos.z;

#undef	D_LENGTH
#undef	D_SQ

  float	phi = atan2f( Profile_Pos.y, Profile_Pos.x );

  //// APPLY

  glTranslatef( Object_Pos.x, Object_Pos.y, Object_Pos.z );

#define	D_RADIAN_TO_DEGREE(rad) ((rad) * 180.0f / 3.14f)

  glRotatef( D_RADIAN_TO_DEGREE(theta), 0,0,1 );
  glRotatef( D_RADIAN_TO_DEGREE(phi), 0,1,0 );

#undef	D_RADIAN_TO_DEGREE

}
