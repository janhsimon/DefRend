#version 330

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inUV;

out vec4 vs_fs_posWS;
out vec2 vs_fs_uv;

uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main(void)
{
	vs_fs_posWS = worldMatrix * vec4(inPosition, 1.0);
	gl_Position = projectionMatrix * viewMatrix * worldMatrix * vec4(inPosition, 1.0);

	vs_fs_uv = inUV;
}