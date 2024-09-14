# OpenGL Renderer
It is a 3D renderer, which can read external obj files, map the textures according to the UV data and calculate lighting/shadows from the surface normal data. It is the part of the C++ Engine I have been working on (https://github.com/ShivangNagta/PhysicsEngine_OpenGL_SDL2). It uses the Blinn Phong Reflection Model for now, which includes ambient, diffuse and specular lighting, all calculated on the Fragment Shader. <br/>
Currently not for deployment, but you can use it in windows (keep the Mingw setup). <br/>
Makefile is configured for compilation. <br/>

Libraries used - <br/>
SDL2<br/>
glad<br/>
glm<br/>
imgui<br/>
stb_image<br/>

Compile command - make <br/>
Run the executable - bin\main <br/>

Camera Movements : <br/>
w - move forward  <br/>
s - move backwards <br/>
a - move left <br/>
d - move right <br/>
space - move up <br/>
left ctrl - move down <br/>

![Screenshot (3)](https://github.com/user-attachments/assets/987a7681-30ab-41a2-a671-5bca3411ba60)
![Screenshot (5)](https://github.com/user-attachments/assets/5f3d33ac-ddef-4ed8-811a-349e184a9335)
![Screenshot (7)](https://github.com/user-attachments/assets/7d259c2e-6efd-4832-b355-ce3b6aaf611f)
