#version 330

layout (location = 0) in vec3 inPosition;

out vec4 vs_fs_posWS;

uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main(void)
{
	vs_fs_posWS = worldMatrix * vec4(inPosition, 1.0);
	gl_Position = projectionMatrix * viewMatrix * worldMatrix * vec4(inPosition, 1.0);
}