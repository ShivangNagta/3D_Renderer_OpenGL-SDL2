#version 410 core

layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 textCoord;

uniform vec3 lightColor;
uniform mat4 u_ModelMatrixLight;
uniform mat4 u_PerspectiveLight;
uniform mat4 u_ViewLight;

out vec3 v_lightColor;
 
void main()
{
    v_lightColor = lightColor;

    vec4 newPosition = u_PerspectiveLight * u_ViewLight * u_ModelMatrixLight * vec4(position, 1.0f);
    gl_Position = vec4(newPosition.x, newPosition.y, newPosition.z, newPosition.w);
}

