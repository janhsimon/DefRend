#version 330

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inNormal;

out vec2 vs_fs_uv;
out vec3 vs_fs_normal;

uniform mat4 worldViewMatrix;
uniform mat4 projectionMatrix;

void main(void)
{
	// bring vertex position from model- to screenspace
	gl_Position = projectionMatrix * worldViewMatrix * vec4(inPosition, 1.0);

	// pass on tex coord uvs and normal
	vs_fs_uv = inUV;
	vs_fs_normal = inNormal;
}