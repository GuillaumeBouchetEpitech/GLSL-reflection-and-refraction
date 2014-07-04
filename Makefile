

NAME=	exec


####


SRC_MAIN_PATH=	.
SRC_MAIN=	$(SRC_MAIN_PATH)/main.cpp		\
		$(SRC_MAIN_PATH)/Exception.cpp		\
		$(SRC_MAIN_PATH)/Program.cpp

SRC_GL_PATH=	$(SRC_MAIN_PATH)/myGL
SRC_GL=		$(SRC_GL_PATH)/GL_BoundingBox.cpp		\
		$(SRC_GL_PATH)/GL_Camera_FreeFly.cpp		\
		$(SRC_GL_PATH)/GL_CubeMap.cpp			\
		$(SRC_GL_PATH)/GL_FrustumCulling.cpp		\
		$(SRC_GL_PATH)/GL_Geometry_Box.cpp		\
		$(SRC_GL_PATH)/GL_Geometry.cpp			\
		$(SRC_GL_PATH)/GL_Geometry_Cylinder.cpp		\
		$(SRC_GL_PATH)/GL_Geometry_PartialCylinder.cpp	\
		$(SRC_GL_PATH)/GL_Geometry_Sphere.cpp		\
		$(SRC_GL_PATH)/GL_Geometry_Torus.cpp		\
		$(SRC_GL_PATH)/GL_Light.cpp			\
		$(SRC_GL_PATH)/GL_Matrix.cpp			\
		$(SRC_GL_PATH)/GL_OBJLoader.cpp			\
		$(SRC_GL_PATH)/GL_RawRenderer.cpp		\
		$(SRC_GL_PATH)/GL_Shader.cpp			\
		$(SRC_GL_PATH)/GL_Shadow.cpp			\
		$(SRC_GL_PATH)/GL_Texture.cpp


####

SRC=		$(SRC_MAIN) $(SRC_GL)

OBJ=		$(SRC:.cpp=.o)

####

# SFML_PATH=	./lib/SFML-2.1
SFML_PATH=	/home/barbie/Programmation/Library/SFML/SFML-2.1_git/SFML/
SFML_INC_PATH=	$(SFML_PATH)/include
SFML_LIB_PATH=	$(SFML_PATH)/lib
SFML_LIB=	-Wl,--rpath=$(SFML_LIB_PATH)			\
		$(SFML_LIB_PATH)/libsfml-graphics.so.2.1	\
		$(SFML_LIB_PATH)/libsfml-window.so.2.1		\
		$(SFML_LIB_PATH)/libsfml-system.so.2.1


#######


CXXFLAGS=	-Wall -W -Wextra -Wunused -O3	\
		-I./				\
		-I$(SFML_INC_PATH)

# CXXFLAGS=	-Wall -W -Wextra -Wunused -g3	\
# 		-I./				\
# 		-I$(SFML_INC_PATH)

LDFLAGS=	$(SFML_LIB) -lGL -lGLU -lGLEW


#######


CXX=		g++
RM=		rm -f


#######


$(NAME):	$(OBJ)
		$(CXX) $(OBJ) -o $(NAME) $(LDFLAGS)

all:		$(NAME)

clean:
		$(RM) $(OBJ)

fclean:		clean
		$(RM) $(NAME)

re:		fclean $(NAME)

.PHONY:		all clean fclean re
