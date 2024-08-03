#define GLM_ENABLE_EXPERIMENTAL

#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtx/rotate_vector.hpp"
#include <glm/gtx/quaternion.hpp>

Camera::Camera(){
    mEye = glm::vec3(0.0f, 6.0f, 20.0f);
    mViewDirection = glm::vec3(0.0f, 0.0f, -1.0f);
    mUpVector = glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::mat4 Camera::GetViewMatrix() const{
    return glm::lookAt(mEye, mEye + mViewDirection, mUpVector);
}

glm::vec3 Camera::GetEyePos() const{
    return mEye;
}

void Camera::MouseLook(int mouseX, int mouseY) {
    glm::vec2 currentMouse = glm::vec2(mouseX, mouseY);
    
    static bool firstLook = true;
    if(firstLook) {
        mOldMousePosition = currentMouse;
        firstLook = false;
    }
    
    glm::vec2 mouseDelta = currentMouse - mOldMousePosition;
    
    // Apply sensitivity
    float sensitivity = 0.1f;
    mouseDelta *= sensitivity;
    
    // Yaw rotation around the up vector
    glm::quat yawQuat = glm::angleAxis(glm::radians(-mouseDelta.x), mUpVector);
    mViewDirection = glm::normalize(yawQuat * mViewDirection);
    
    // Pitch rotation around the right vector
    glm::vec3 rightVector = glm::normalize(glm::cross(mViewDirection, mUpVector));
    glm::quat pitchQuat = glm::angleAxis(glm::radians(-mouseDelta.y), rightVector);
    glm::vec3 newViewDirection = glm::normalize(pitchQuat * mViewDirection);
    
    // Prevent flipping by limiting the pitch angle
    float pitchAngle = glm::degrees(glm::asin(newViewDirection.y));
    if (pitchAngle > -89.0f && pitchAngle < 89.0f) {
        mViewDirection = newViewDirection;
    }
    
    mOldMousePosition = currentMouse;
}

void Camera::MoveForward(float speed){
    mEye += (mViewDirection*speed);
};
void Camera::MoveBackward(float speed){
    mEye -= (mViewDirection*speed);
};
void Camera::MoveLeft(float speed){
    glm::vec3 rightVector = glm::normalize(glm::cross(mViewDirection, mUpVector));
    mEye -= (rightVector * speed);
}

void Camera::MoveRight(float speed){
    glm::vec3 rightVector = glm::normalize(glm::cross(mViewDirection, mUpVector));
    mEye += (rightVector * speed);
}
void Camera::MoveUp(float speed){
    mEye += (mUpVector * speed);
}

void Camera::MoveDown(float speed){
    mEye -= (mUpVector * speed);
}




