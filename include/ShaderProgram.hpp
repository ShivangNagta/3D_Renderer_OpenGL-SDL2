#ifndef SHADERPROGRAM_HPP
#define SHADERPROGRAM_HPP

#include <glad/glad.h>
#include <string>

class ShaderProgram {
public:
    ShaderProgram();
    ~ShaderProgram();

    void LoadShader(const std::string& vertexShaderFile, const std::string& fragmentShaderFile);
    void Use() const;
    void Unuse() const;

    GLuint GetProgramID() const;

private:
    GLuint programID;

    GLuint CompileShader(GLuint type, const std::string& source);
    GLuint CreateShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
    std::string LoadShaderAsString(const std::string& filename);
};

#endif // SHADERPROGRAM_HPP
