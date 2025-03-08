#version 460 core
out vec4 FragColor;

uniform vec3 lineColor; // 线框颜色

void main() {
    FragColor = vec4(lineColor, 1.0);
}