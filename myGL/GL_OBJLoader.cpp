

#include "GL_OBJLoader.hpp"

#include "GL_Geometry.hpp"

#include <sstream>
#include <fstream>
#include <vector>
#include <iostream>


GL_Geometry*	GL_OBJLoader::Load(const std::string& FileName)
{
  GL_Geometry*	pGeom = NULL;

  std::ifstream	FileReader(FileName.c_str());

  if (FileReader.fail())
    std::cerr << "Fail to open the file \"" << FileName << "\"." << std::endl;

  else
    {
      pGeom = new GL_Geometry;

      while (FileReader)
	{
	  std::string	str_line;
	  getline(FileReader, str_line);

	  std::stringstream	sstr(str_line.c_str());

	  char c = sstr.get();
	  if (!(c == '#' || c == '$') && str_line.size() > 2)
	    {
	      char c2 = sstr.get();
	      if (c == 'v')
		{
		  float x, y, z, w;

		  if (c2 == 'n' && sstr.get() != ' ')
		    c2 = '\0';

		  sstr >> x;
		  if (sstr.fail())
		    {
		      sstr.clear();
		      x = 0;
		    }

		  sstr >> y;
		  if (sstr.fail())
		    {
		      sstr.clear();
		      y = 0;
		    }

		  sstr >> z;
		  if (sstr.fail())
		    {
		      sstr.clear();
		      z = 0;
		    }

		  sstr >> w;
		  if (sstr.fail())
		    {
		      sstr.clear();
		      w = 1;
		    }

		  if (c2 == ' ')
		    pGeom->m_Vertices.push_back( sf::Vector3f(x / w, y / w, z / w) );
		  else if (c2 == 'n')
		    pGeom->m_Normales.push_back( sf::Vector3f(x / w, y / w, z / w) );
		  else if (c2 == 't')
		    pGeom->m_TexCoords.push_back( sf::Vector2f(x, y) );

		} // if (c == 'v')

	      else if (c2 == ' ' && (c == 'l' || c == 'p' || c == 'f'))
		{
		  unsigned int	prim = ( (str_line[0] == 'p')
					 ? (GL_POINTS)
					 : ( (str_line[0] == 'l')
					     ? (GL_LINE_STRIP)
					     : (GL_POLYGON) ) );

		  int vi, ti, ni;
		  while (!sstr.eof())
		    {
		      sstr >> vi;
		      if (sstr.fail())
			sstr.clear();
		      else
			pGeom->m_Prim_index.push_back(vi - 1);

		      if (sstr.peek() == '/')
			{
			  sstr.get();

			  sstr >> ti;
			  if (sstr.fail())
			    sstr.clear();
			  else
			    pGeom->m_TexC_index.push_back(ti - 1);
			  // element->texCoordIndices.push_back(remapTexCoordIndex(ti));

			  if (sstr.peek() == '/')
			    {
			      sstr.get();

			      sstr >> ni;
			      if (sstr.fail())
				sstr.clear();
			      else
				pGeom->m_Norm_index.push_back(ni - 1);
			    }

			} // if (sstr.peek() == '/')

		    } // while (!sstr.eof())

		  pGeom->m_Primitives.push_back(prim);

		} // else if (c2 == ' ' && (c == 'l' || c == 'p' || c == 'f'))

	    } // if (!(c == '#' || c == '$') && str_line.size() > 2)

	} // while (FileReader)

      FileReader.close();

      pGeom->Setup();


      { // Print

	// std::cerr << std::endl
	// 	  << "indices : " << pGeom->m_Primitives.size() << " (" << pGeom->m_Primitives.size() / 3 << ")" << std::endl
	// 	  << "prim indices : " << pGeom->m_Prim_index.size() << " (" << pGeom->m_Prim_index.size() / 3 << ")" << std::endl
	// 	  << "norm indices : " << pGeom->m_Norm_index.size() << " (" << pGeom->m_Norm_index.size() / 3 << ")" << std::endl
	// 	  << "vertices : " << pGeom->m_Vertices.size() << std::endl
	// 	  << "normals : " << pGeom->m_Normales.size() << std::endl
	// 	  << std::endl;

      } // /Print


      { // Setup

	// std::vector< sf::Vector3f >	Normales_tmp;

	// for (unsigned int i = 0; i < pGeom->m_Primitives.size(); ++i)
	//   {

	//     for (unsigned int k = 0; k < 3; ++k)
	//       {

	// 	unsigned int	index = i * 3 + k;

	// 	unsigned int	norm = pGeom->m_Norm_index[ index ];

	// 	Normales_tmp.push_back( pGeom->m_Normales[ norm ] );

	//       }

	//   }

	// pGeom->m_Normales.swap( Normales_tmp );

      } // /Setup


      { // Setup

	/**

	std::vector< sf::Vector3f >	Vertices_tmp;
	std::vector< sf::Vector3f >	Normales_tmp;

	unsigned int	prim_nbr = pGeom->m_Primitives.size() * 3;

	Vertices_tmp.reserve( prim_nbr );
	Normales_tmp.reserve( prim_nbr );

	// for (unsigned int i = 0; i < pGeom->m_Primitives.size(); ++i)
	for (unsigned int i = 0; i < prim_nbr; ++i)
	  {
	    // for (unsigned int k = 0; k < 3; ++k)
	    //   {
	    // 	unsigned int	index = i * 3 + k;

	    	unsigned int	index = i;

		unsigned int	vert = pGeom->m_Prim_index[ index ];
		unsigned int	norm = pGeom->m_Norm_index[ index ];


		Vertices_tmp.push_back( (pGeom->m_Vertices)[ vert ] );
		Normales_tmp.push_back( (pGeom->m_Normales)[ norm ] );



	      // 	unsigned int	texc = 0;

	      // 	if (!m_TexC_index.empty())
	      // 	  texc = m_TexC_index[ index ];


	      // 	t_Vertex*	curr = &(m_vbo_Vertices[ index ]);

	      // 	curr->x = (m_Vertices)[ vert ].x;
	      // 	curr->y = (m_Vertices)[ vert ].y;
	      // 	curr->z = (m_Vertices)[ vert ].z;

	      // 	curr->nx = (m_Normales)[ norm ].x;
	      // 	curr->ny = (m_Normales)[ norm ].y;
	      // 	curr->nz = (m_Normales)[ norm ].z;

	      // 	curr->r = 1.0f;
	      // 	curr->g = 1.0f;
	      // 	curr->b = 1.0f;

	      // 	if (!m_TexC_index.empty())
	      // 	  {
	      // 	    curr->tx = (m_TexCoords)[ texc ].x;
	      // 	    curr->ty = (m_TexCoords)[ texc ].y;
	      // 	  }

	      // 	else if (!m_TexCoords.empty())
	      // 	  {
	      // 	    curr->tx = (m_TexCoords)[ vert ].x;
	      // 	    curr->ty = (m_TexCoords)[ vert ].y;
	      // 	  }

	      // 	// // float	color = 0.66f + (k + 1) * 0.11f;
	      // 	// float	color = (k + 1) * 0.33f;
	      // 	// curr->r = color;
	      // 	// curr->g = color;
	      // 	// curr->b = color;
	      // 	// // curr->g = 0;
	      // 	// // curr->b = 0;
	      // }

	  }

	pGeom->m_Vertices.swap( Vertices_tmp );
	pGeom->m_Normales.swap( Normales_tmp );

	//*/

      } // /Setup


      { // Print

	// std::cerr << std::endl
	// 	  << "indices : " << pGeom->m_Primitives.size() << " (" << pGeom->m_Primitives.size() / 3 << ")" << std::endl
	// 	  << "prim indices : " << pGeom->m_Prim_index.size() << " (" << pGeom->m_Prim_index.size() / 3 << ")" << std::endl
	// 	  << "norm indices : " << pGeom->m_Norm_index.size() << " (" << pGeom->m_Norm_index.size() / 3 << ")" << std::endl
	// 	  << "vertices : " << pGeom->m_Vertices.size() << std::endl
	// 	  << "normals : " << pGeom->m_Normales.size() << std::endl
	// 	  << std::endl;

      } // /Print


    } // else

  return (pGeom);

} // Load( ... )


bool	GL_OBJLoader::Save(const std::string& FileName, const GL_Geometry& Geom)
{
  std::ofstream	FileWriter( FileName.c_str() );

  bool	ret_val = false;

  if ( !Geom.m_Vertices.empty() &&
       !Geom.m_Normales.empty() &&
       !Geom.m_Prim_index.empty() &&
       !Geom.m_Norm_index.empty() &&

       FileWriter.fail() )
    {
      std::cerr << "Fail to open the file \"" << FileName << "\"." << std::endl;
    }

  else
    {
      ret_val = true;

      FileWriter << std::endl;
      for (unsigned int i = 0; i < Geom.m_Vertices.size(); ++i)
	{
	  // FileWriter << std::fixed << "v "
	  FileWriter << "v "
		     << Geom.m_Vertices[i].x << " "
		     << Geom.m_Vertices[i].y << " "
		     << Geom.m_Vertices[i].z << std::endl;
	}

      FileWriter << std::endl;
      for (unsigned int i = 0; i < Geom.m_Normales.size(); ++i)
	{
	  // FileWriter << std::fixed << "vn "
	  FileWriter << "vn "
		     << Geom.m_Normales[i].x << " "
		     << Geom.m_Normales[i].y << " "
		     << Geom.m_Normales[i].z << std::endl;
	}

      FileWriter << std::endl;
      for (unsigned int i = 0; i < Geom.m_Prim_index.size() / 3; ++i)
	{
#define	D_PRIM(index)	(Geom.m_Prim_index[ i * 3 + (index) ] + 1)
#define	D_NORM(index)	(Geom.m_Norm_index[ i * 3 + (index) ] + 1)

	  FileWriter << "f "
		     << D_PRIM(0) << "//" << D_NORM(0) << " "
		     << D_PRIM(1) << "//" << D_NORM(1) << " "
		     << D_PRIM(2) << "//" << D_NORM(2) << std::endl;

#undef	D_PRIM
#undef	D_NORM
	}

      FileWriter << std::endl;

      FileWriter.close();
    }

  return (ret_val);
}

