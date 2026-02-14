CXX = g++
CXXFLAGS = -std=c++17 -Iglad/include -Isrc
LINKFLAGS = -lglfw -ldl -lGL -lpthread -lglfw -ldl -lGL -lX11 -lpthread -lopenxr_loader -limgui -lassimp -lnfd

BIN = ./bin
OUT = composer.out
MAIN = ./src/*.cpp ./src/core/*.cpp ./src/renderer/*.cpp ./src/input/*.cpp ./src/utils/*.cpp glad/src/gl.c
SHADERS = ./shaders/fragment_core.glsl ./shaders/vertex_core.glsl

$(BIN)/$(OUT): $(MAIN)
	mkdir -p $(BIN)
	$(CXX) $(MAIN) $(CXXFLAGS) -o $(BIN)/$(OUT) $(LINKFLAGS)

.PHONY: clean
clean:
	rm $(BIN)/$(OUT)