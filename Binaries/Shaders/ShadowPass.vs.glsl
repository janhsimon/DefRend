#version 330

layout (location = 0) in vec3 inPosition;

out vec3 vs_fs_position;

uniform mat4 worldMatrix;
uniform mat4 viewProjectionMatrix;

void main(void)
{
	// bring vertex position from model- to screenspace
	gl_Position = viewProjectionMatrix * worldMatrix * vec4(inPosition, 1.0);

	// store world position
	vs_fs_position = (worldMatrix * vec4(inPosition, 1.0)).xyz;
}