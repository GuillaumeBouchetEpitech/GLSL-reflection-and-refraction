

#ifndef	READOBJFILE_HH
#define	READOBJFILE_HH


#include <string>

class GL_Geometry;

class	GL_OBJLoader
{
public : // step

  static GL_Geometry*	Load(const std::string& FileName);

  static bool		Save(const std::string& FileName, const GL_Geometry& Geom);

};

#endif

