CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -Iglad/include -Isrc
GLFWFLAGS = -lglfw -ldl -lGL -lpthread
IMGUIFLAGS = -limgui
ASSIMPFLAGS = -lassimp

BIN = ./bin
OUT = composer.out
MAIN = ./src/*.cpp ./src/core/*.cpp ./src/renderer/*.cpp ./src/input/*.cpp ./src/utils/*.cpp glad/src/gl.c
SHADERS = ./shaders/fragment_core.glsl ./shaders/vertex_core.glsl

$(BIN)/$(OUT): $(MAIN)
	mkdir -p $(BIN)
	$(CXX) $(MAIN) $(CXXFLAGS) -o $(BIN)/$(OUT) $(GLFWFLAGS) $(IMGUIFLAGS) $(ASSIMPFLAGS)

.PHONY: clean
clean:
	rm $(BIN)/$(OUT)