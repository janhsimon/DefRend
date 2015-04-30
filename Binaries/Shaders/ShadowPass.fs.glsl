#version 330

layout (location = 0) out vec4 color;

in vec3 vs_fs_position;

uniform vec3 pointLightPosition;

void main()
{
	float distance = length(pointLightPosition - vs_fs_position);
	color = vec4(distance, distance, distance, 1.0);
}