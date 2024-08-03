#include "ShaderProgram.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

ShaderProgram::ShaderProgram() : programID(0) {}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(programID);
}

void ShaderProgram::LoadShader(const std::string& vertexShaderFile, const std::string& fragmentShaderFile) {
    std::string vertexShaderSource = LoadShaderAsString(vertexShaderFile);
    std::string fragmentShaderSource = LoadShaderAsString(fragmentShaderFile);
    programID = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);
}

void ShaderProgram::Use() const {
    glUseProgram(programID);
}

void ShaderProgram::Unuse() const {
    glUseProgram(0);
}

GLuint ShaderProgram::GetProgramID() const {
    return programID;
}

GLuint ShaderProgram::CompileShader(GLuint type, const std::string& source) {
    GLuint shaderObject = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shaderObject, 1, &src, nullptr);
    glCompileShader(shaderObject);

    // Check for compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shaderObject, 512, nullptr, infoLog);
        std::cerr << "Shader compilation error: " << infoLog << std::endl;
        exit(EXIT_FAILURE);
    }

    return shaderObject;
}

GLuint ShaderProgram::CreateShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) {
    GLuint programObject = glCreateProgram();

    GLuint myVertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint myFragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    glAttachShader(programObject, myVertexShader);
    glAttachShader(programObject, myFragmentShader);
    glLinkProgram(programObject);

    // Check for linking errors
    int success;
    char infoLog[512];
    glGetProgramiv(programObject, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(programObject, 512, nullptr, infoLog);
        std::cerr << "Shader linking error: " << infoLog << std::endl;
        exit(EXIT_FAILURE);
    }

    glValidateProgram(programObject);

    // Clean up shaders
    glDeleteShader(myVertexShader);
    glDeleteShader(myFragmentShader);

    return programObject;
}

std::string ShaderProgram::LoadShaderAsString(const std::string& filename) {
    std::string result = "";
    std::string line = "";
    std::ifstream myFile(filename.c_str());

    if (myFile.is_open()) {
        while (std::getline(myFile, line)) {
            result += line + "\n";
        };
        myFile.close();
    }

    return result;
}
