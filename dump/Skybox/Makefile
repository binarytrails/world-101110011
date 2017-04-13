# VARIABLES

CXX = g++
CXXFLAGS = -std=c++11 -w
OPENGL_MAC=-framework Cocoa -framework OpenGL \
		   -framework IOKit -framework CoreVideo

LIBS = -lGLEW -lglfw3 -lSOIL

SRC = Skybox.cpp SkyboxMain.cpp ../Camera.cpp ../Window.cpp ../Shader.cpp

BIN = -o Skybox.out

# RULES

all: $(SRC)
	$(CXX) $(CXXFLAGS) $(OPENGL_MAC) $(LIBS) $(SRC) $(BIN);

clean:
	rm *.out 
