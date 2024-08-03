#version 410 core

in vec3 v_lightColor;
out vec4 color;


void main()
{
    color = vec4(v_lightColor, 1.0);
}
