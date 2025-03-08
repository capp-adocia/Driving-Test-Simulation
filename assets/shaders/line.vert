#version 460 core
layout(location = 0) in vec3 aPos;

uniform mat4 viewProj; // 视图投影矩阵

void main() {
    gl_Position = viewProj * vec4(aPos, 1.0);
}