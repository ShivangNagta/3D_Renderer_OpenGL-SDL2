//Third Party
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


//C++ STL
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

//My Custom Libraries
#include "Camera.hpp"
#include "ShaderProgram.hpp"
#include "Texture2D.hpp"
#include "Mesh.hpp"

// Global variables
const int gScreenWidth = 1024;
const int gScreenHeight = 768;
SDL_Window* gGraphicsApplicationWindow = nullptr;
SDL_GLContext gOpenGLContext = nullptr; 
bool gQuit = false;

const int numOfModels = 6;
Mesh mesh[numOfModels];
Mesh lightMesh[2];
Texture2D texture[numOfModels];

float angle = 0.0f;

// Global Camera;
Camera g_camera;

// FPS
void ShowFPS(SDL_Window*){
    static double previousSeconds = 0.0;
    static int frameCount = 0;
    double elapsedSeconds;

    double currentSeconds = SDL_GetTicks() / 1000.0;

    elapsedSeconds = currentSeconds - previousSeconds;

    //Update 4 times a second
    if (elapsedSeconds > 0.25){
        previousSeconds = currentSeconds;
        double fps = (double)frameCount / elapsedSeconds;
        double msPerFrame = 1000 / fps;

        std::ostringstream outs;
        outs.precision(3);
        outs << std::fixed << "  "
             << "FPS: " << fps << "  "
             << "Frame Time: " << msPerFrame << " (ms)";
        SDL_SetWindowTitle(gGraphicsApplicationWindow, outs.str().c_str());
        frameCount = 0;   
    }
    frameCount++;
}


void Init(){
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    gGraphicsApplicationWindow = SDL_CreateWindow("OpenGL",
                            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                            gScreenWidth, gScreenHeight,
                            SDL_WINDOW_OPENGL);

    if(gGraphicsApplicationWindow == nullptr){
        std::cerr << "SDL_Window was not created" << std::endl;
        exit(1);
    }

    gOpenGLContext = SDL_GL_CreateContext(gGraphicsApplicationWindow);
    
    if(gOpenGLContext == nullptr){
        std::cerr << "OpenGL context not available\n";
        exit(1);
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        exit(1);
    }

    // V-Sync Enabled for FPS restriced with monitor fresh rate
    if (SDL_GL_SetSwapInterval(0) < 0) {
        std::cerr << "Warning: Unable to disable VSync! SDL Error: " << SDL_GetError() << std::endl;
    }



	mesh[0].loadOBJ("./assets/models/bunny.obj");
	// mesh[1].loadOBJ("./assets/models/floor.obj");
	// mesh[2].loadOBJ("./assets/models/barrel.obj");
	// mesh[3].loadOBJ("./assets/models/woodcrate.obj");
	// mesh[4].loadOBJ("./assets/models/robot.obj");
	// mesh[5].loadOBJ("./assets/models/bowling_pin.obj");

    lightMesh[0].loadOBJ("./assets/models/light.obj");
    lightMesh[1].loadOBJ("./assets/models/light.obj");
    


    texture[0].loadTexture("./assets/textures/bunny_diffuse.jpg", true);
    // texture[1].loadTexture("./assets/textures/tile_floor.jpg", true);
    // texture[2].loadTexture("./assets/textures/crate.jpg", true);
    // texture[3].loadTexture("./assets/textures/woodcrate_diffuse.jpg", true);
    // texture[4].loadTexture("./assets/textures/robot_diffuse.jpg", true);
    // texture[5].loadTexture("./assets/textures/AMF.tga", true);
}


void Input(){
    SDL_Event e;
    static int mouseX = gScreenWidth / 2;
    static int mouseY = gScreenHeight / 2;
    static bool wireframeMode = false;
   
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
        case (SDL_QUIT):
            gQuit = true;
            break;
        case SDL_KEYDOWN:
            if (e.key.keysym.sym == SDLK_ESCAPE) {
                gQuit = true;
            }
            if (e.key.keysym.sym == SDLK_1) {
                wireframeMode = !wireframeMode;
                if (wireframeMode) {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                } else {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }
            }
            break;
        case (SDL_MOUSEMOTION):
            mouseX += e.motion.xrel;
            mouseY += e.motion.yrel;
            g_camera.MouseLook(mouseX, mouseY );
            break;
        
        }
    }
    
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    float speed = 0.01f;
    if(state[SDL_SCANCODE_W]){
        g_camera.MoveForward(speed);
    }
    if(state[SDL_SCANCODE_S]){
        g_camera.MoveBackward(speed);
    }
    if(state[SDL_SCANCODE_A]){
        g_camera.MoveLeft(speed);
    }
    if(state[SDL_SCANCODE_D]){
        g_camera.MoveRight(speed);
    }
    if(state[SDL_SCANCODE_SPACE]){
        g_camera.MoveUp(speed);
    }
    if(state[SDL_SCANCODE_LCTRL]){
        g_camera.MoveDown(speed);
    }
}


void PreDraw(){
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    ShaderProgram shaderProgram;
    shaderProgram.LoadShader("./shaders/vert.glsl", "./shaders/frag.glsl");
    GLuint gGraphicsPipelineShaderProgram = shaderProgram.GetProgramID();

    shaderProgram.Use();

    // Model positions
    glm::vec3 modelPos[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),    // bunny
        glm::vec3(0.0f, 0.0f, 0.0f),    // floor
        glm::vec3(-3.5f, 0.0f, 0.0f),   // barrel
        glm::vec3(3.5f, 0.0f, 0.0f),    // crate
        glm::vec3(0.0f, 0.0f, -2.0f),   // robot
        glm::vec3(0.0f, 0.0f, 2.0f),    // pin
    };

    // Model scale
    glm::vec3 modelScale[] = {
        glm::vec3(1.5f, 1.5f, 1.5f),    // bunny
        glm::vec3(4.0f, 1.0f, 4.0f),    // floor
        glm::vec3(1.0f, 1.0f, 1.0f),    // barrel
        glm::vec3(1.0f, 1.0f, 1.0f),    // crate
        glm::vec3(1.0f, 1.0f, 1.0f),    // robot
        glm::vec3(0.1f, 0.1f, 0.1f),    // pin
    };

    glm::vec3 lightPos = {0.0f, 3.0f, 5.0f};
    glm::vec3 lightColor = {1.0f, 1.0f, 1.0f};

    glm::vec3 lightPos2 = {3.0f, 5.0f, 0.0f};
    glm::vec3 lightColor2 = {1.0f, 0.0f, 0.0f};

    angle = SDL_GetTicks64() * 0.1f;
    lightPos.x = cosf(glm::radians(angle)) * 5.0f;
    lightPos.z = sinf(glm::radians(angle)) * 5.0f;

    glm::vec3 cameraPos = g_camera.GetEyePos();

    glViewport(0, 0, gScreenWidth, gScreenHeight);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set light properties for the shader
    glUniform3f(glGetUniformLocation(gGraphicsPipelineShaderProgram, "lightColor"), lightColor.x, lightColor.y, lightColor.z);
    glUniform3f(glGetUniformLocation(gGraphicsPipelineShaderProgram, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    glUniform3f(glGetUniformLocation(gGraphicsPipelineShaderProgram, "lightColor2"), lightColor2.x, lightColor2.y, lightColor2.z);
    glUniform3f(glGetUniformLocation(gGraphicsPipelineShaderProgram, "lightPos2"), lightPos2.x, lightPos2.y, lightPos2.z);
    glUniform3f(glGetUniformLocation(gGraphicsPipelineShaderProgram, "cameraPos"), cameraPos.x, cameraPos.y, cameraPos.z);

    // View Matrix
    glm::mat4 view = g_camera.GetViewMatrix();
    GLint u_ViewLocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, "u_View");
    if(u_ViewLocation >= 0){
        glUniformMatrix4fv(u_ViewLocation, 1, GL_FALSE, &view[0][0]);
    }
    else{
        std::cout << "u_View errrrrrr!!!" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Perspective Projection
    glm::mat4 perspective = glm::perspective(glm::radians(45.0f), (float)gScreenWidth / (float)gScreenHeight, 0.1f, 100.0f);
    GLint u_PerspectiveLocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, "u_Perspective");
    glUniformMatrix4fv(u_PerspectiveLocation, 1, GL_FALSE, &perspective[0][0]);

    for (int i = 0; i < numOfModels; i++) {
        // Set model matrix
        glm::mat4 model = glm::translate(glm::mat4(1.0f), modelPos[i]);
        model = glm::scale(model, modelScale[i]);
        GLint u_ModelMatrixLocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, "u_ModelMatrix");
        glUniformMatrix4fv(u_ModelMatrixLocation, 1, GL_FALSE, &model[0][0]);

        // Bind texture
        texture[i].bind(0);
        GLint u_Texture1Location = glGetUniformLocation(gGraphicsPipelineShaderProgram, "myTexture1");
        glUniform1i(u_Texture1Location, 0);

        // Draw the mesh
        mesh[i].draw();

        // Unbind texture
        texture[i].unbind(0);
    }
    shaderProgram.Unuse();

    ShaderProgram lightShader;
    lightShader.LoadShader("./shaders/light.vert", "./shaders/light.frag");
    GLuint gGraphicsPipelineLightShader = lightShader.GetProgramID();

    lightShader.Use();

    // View Matrix
    view = g_camera.GetViewMatrix();
    glUniformMatrix4fv(glGetUniformLocation(gGraphicsPipelineLightShader, "u_ViewLight"), 1, GL_FALSE, &view[0][0]);

    // Perspective Projection
    perspective = glm::perspective(glm::radians(45.0f), (float)gScreenWidth / (float)gScreenHeight, 0.1f, 100.0f);
    glUniformMatrix4fv(glGetUniformLocation(gGraphicsPipelineLightShader, "u_PerspectiveLight"), 1, GL_FALSE, &perspective[0][0]);

    // Draw first light
    glm::mat4 model = glm::translate(glm::mat4(1.0f), lightPos);
    glUniformMatrix4fv(glGetUniformLocation(gGraphicsPipelineLightShader, "u_ModelMatrixLight"), 1, GL_FALSE, &model[0][0]);
    GLint u_lightCol = glGetUniformLocation(gGraphicsPipelineLightShader, "lightColor");
    glUniform3f(u_lightCol, lightColor.x, lightColor.y, lightColor.z);
    lightMesh[0].draw();

    // Draw second light
    glm::mat4 model2 = glm::translate(glm::mat4(1.0f), lightPos2);
    glUniformMatrix4fv(glGetUniformLocation(gGraphicsPipelineLightShader, "u_ModelMatrixLight"), 1, GL_FALSE, &model2[0][0]);
    glUniform3f(u_lightCol, lightColor2.x, lightColor2.y, lightColor2.z);
    lightMesh[1].draw();

    SDL_GL_SwapWindow(gGraphicsApplicationWindow);
}




void Draw() {   
    // glBindVertexArray(gVertexArrayObject);
    // glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    // SDL_GL_SwapWindow(gGraphicsApplicationWindow);
}

void MainLoop(){

    SDL_WarpMouseInWindow(gGraphicsApplicationWindow, gScreenWidth/2, gScreenHeight/2);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    while (!gQuit) {
        
        ShowFPS(gGraphicsApplicationWindow);
        Input();
        PreDraw(); 
        Draw();
        
    }
}

void CleanUp(){
    SDL_GL_DeleteContext(gOpenGLContext);
    SDL_DestroyWindow(gGraphicsApplicationWindow);
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Init();

    MainLoop();

    CleanUp();
    return 0;
}


