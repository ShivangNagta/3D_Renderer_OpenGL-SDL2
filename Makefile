all:
	g++ -Iinclude -Iinclude/imgui -Iinclude/SDL2 -Iinclude/glad -Llib -o bin/main \
	    src/main.cpp src/Camera.cpp src/ShaderProgram.cpp src/Mesh.cpp src/Texture2D.cpp include/glad/glad.c -Iinclude/glm \
	    -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
