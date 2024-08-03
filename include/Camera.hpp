#pragma once

#include <glm/glm.hpp>

class Camera{
    public:

    Camera();
    glm::mat4 GetViewMatrix() const;
    glm::vec3 GetEyePos() const;

    void MouseLook(int mouseX, int mouseY);
    void MoveForward(float speed);
    void MoveBackward(float speed);
    void MoveLeft(float speed);
    void MoveRight(float speed);
    void MoveUp(float speed);
    void MoveDown(float speed);
    

    private:
    glm::vec3 mEye;
    glm::vec3 mViewDirection;
    glm::vec3 mUpVector;

    glm::vec2 mOldMousePosition;
};