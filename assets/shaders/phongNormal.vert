#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec3 aNormal;

out vec2 uv;
out vec3 normal;
out vec3 worldPosition;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

//uniform mat3 normalMatrix;

//aPos作为attribute（属性）传入shader
//不允许更改的
void main()
{
	vec4 transformPosition = vec4(aPos, 1.0);
	transformPosition = modelMatrix * transformPosition;
	worldPosition = transformPosition.xyz;
	gl_Position = projectionMatrix * viewMatrix * transformPosition;
	uv = aUV;
//	normal =  normalMatrix * aNormal;
}