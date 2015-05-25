#version 330

layout(location = 0) in vec3 inPosition;

uniform mat4 worldViewProjectionMatrix;

void main(void)
{
	// pass on vertex position
	gl_Position = worldViewProjectionMatrix * vec4(inPosition, 1.0);
}