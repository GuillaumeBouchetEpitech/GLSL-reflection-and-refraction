

#include "GL_BoundingBox.hpp"


#include "GL_Light.hpp"


GL_BoundingBox::GL_BoundingBox()
  // : m_Geom(8)
  : m_Min(0,0,0), m_Max(0,0,0)
{
}

///////////////////////////////////

void	GL_BoundingBox::Reset(const sf::Vector3f& center)
{
  m_Min.x = m_Max.x = center.x;
  m_Min.y = m_Max.y = center.y;
  m_Min.z = m_Max.z = center.z;
}

void	GL_BoundingBox::ExpandBy(float x, float y, float z)
{
  // bool	x_updated = true;
  // bool	y_updated = true;
  // bool	z_updated = true;

  if (x > m_Max.x)
    m_Max.x = x;
  else if (x < m_Min.x)
    m_Min.x = x;
  // else
  //   x_updated = false;

  if (y > m_Max.y)
    m_Max.y = y;
  else if (y < m_Min.y)
    m_Min.y = y;
  // else
  //   y_updated = false;

  if (z > m_Max.z)
    m_Max.z = z;
  else if (z < m_Min.z)
    m_Min.z = z;
  // else
  //   z_updated = false;

  // if (x_updated || y_updated || z_updated)
  //   {
  //     m_mat.Make_Translate( -m_Min.x, -m_Min.y, -m_Min.z );
  //     // m_mat.PreMultScale( sf::Vector3f(1.2, 1.2, 1.2) );

  //     m_Geom.MakeBox( m_Max.x - m_Min.x,
  // 		      m_Max.y - m_Min.y,
  // 		      m_Max.z - m_Min.z,
  // 		      0, 0, 0 );
  //   }

}

void	GL_BoundingBox::Render()
{
  // glPushMatrix();
  // {
  // m_mat.Use_Mult();

  // std::cerr << std::endl;
  // std::cerr << m_Min.x << " / " << m_Min.y << " / " << m_Min.z << std::endl;
  // std::cerr << m_Max.x << " / " << m_Max.y << " / " << m_Max.z << std::endl;

  // glDisable(GL_CULL_FACE);

  GL_Light0::Disable_Lighting();

  glEnable(GL_CULL_FACE);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  glLineWidth(3.0f);

  // m_Geom.Render(false, false);

  glColor4f(1,1,1, 1);

  glCullFace(GL_FRONT);

  glEnable(GL_LINE_STIPPLE);
  glLineStipple(1, 0x00FF);

  glBegin(GL_QUADS);
  {

    /// +X
    glVertex3f( m_Max.x, m_Min.y, m_Max.z );
    glVertex3f( m_Max.x, m_Min.y, m_Min.z );
    glVertex3f( m_Max.x, m_Max.y, m_Min.z );
    glVertex3f( m_Max.x, m_Max.y, m_Max.z );

    /// -X
    glVertex3f( m_Min.x, m_Min.y, m_Min.z );
    glVertex3f( m_Min.x, m_Min.y, m_Max.z );
    glVertex3f( m_Min.x, m_Max.y, m_Max.z );
    glVertex3f( m_Min.x, m_Max.y, m_Min.z );


    /// +Y
    glVertex3f( m_Min.x, m_Max.y, m_Min.z );
    glVertex3f( m_Min.x, m_Max.y, m_Max.z );
    glVertex3f( m_Max.x, m_Max.y, m_Max.z );
    glVertex3f( m_Max.x, m_Max.y, m_Min.z );

    /// -Y
    glVertex3f( m_Min.x, m_Min.y, m_Max.z );
    glVertex3f( m_Min.x, m_Min.y, m_Min.z );
    glVertex3f( m_Max.x, m_Min.y, m_Min.z );
    glVertex3f( m_Max.x, m_Min.y, m_Max.z );


    /// +Z
    glVertex3f( m_Min.x, m_Max.y, m_Max.z );
    glVertex3f( m_Min.x, m_Min.y, m_Max.z );
    glVertex3f( m_Max.x, m_Min.y, m_Max.z );
    glVertex3f( m_Max.x, m_Max.y, m_Max.z );

    /// -Z
    glVertex3f( m_Min.x, m_Min.y, m_Min.z );
    glVertex3f( m_Min.x, m_Max.y, m_Min.z );
    glVertex3f( m_Max.x, m_Max.y, m_Min.z );
    glVertex3f( m_Max.x, m_Min.y, m_Min.z );

  }
  glEnd();

  glDisable(GL_LINE_STIPPLE);

  glCullFace(GL_BACK);

  glBegin(GL_QUADS);
  {

    /// +X
    glVertex3f( m_Max.x, m_Min.y, m_Max.z );
    glVertex3f( m_Max.x, m_Min.y, m_Min.z );
    glVertex3f( m_Max.x, m_Max.y, m_Min.z );
    glVertex3f( m_Max.x, m_Max.y, m_Max.z );

    /// -X
    glVertex3f( m_Min.x, m_Min.y, m_Min.z );
    glVertex3f( m_Min.x, m_Min.y, m_Max.z );
    glVertex3f( m_Min.x, m_Max.y, m_Max.z );
    glVertex3f( m_Min.x, m_Max.y, m_Min.z );


    /// +Y
    glVertex3f( m_Min.x, m_Max.y, m_Min.z );
    glVertex3f( m_Min.x, m_Max.y, m_Max.z );
    glVertex3f( m_Max.x, m_Max.y, m_Max.z );
    glVertex3f( m_Max.x, m_Max.y, m_Min.z );

    /// -Y
    glVertex3f( m_Min.x, m_Min.y, m_Max.z );
    glVertex3f( m_Min.x, m_Min.y, m_Min.z );
    glVertex3f( m_Max.x, m_Min.y, m_Min.z );
    glVertex3f( m_Max.x, m_Min.y, m_Max.z );


    /// +Z
    glVertex3f( m_Min.x, m_Max.y, m_Max.z );
    glVertex3f( m_Min.x, m_Min.y, m_Max.z );
    glVertex3f( m_Max.x, m_Min.y, m_Max.z );
    glVertex3f( m_Max.x, m_Max.y, m_Max.z );

    /// -Z
    glVertex3f( m_Min.x, m_Min.y, m_Min.z );
    glVertex3f( m_Min.x, m_Max.y, m_Min.z );
    glVertex3f( m_Max.x, m_Max.y, m_Min.z );
    glVertex3f( m_Max.x, m_Min.y, m_Min.z );

  }
  glEnd();

  glLineWidth(1.0f);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glDisable(GL_CULL_FACE);

  GL_Light0::Enable_Lighting();

  // }
  // glPopMatrix();
}

