#version 330

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec2 inUV;

out vec2 vs_fs_uv;

uniform mat4 worldMatrix;
uniform vec2 uvScale;

void main(void)
{
	// pass on vertex position
	gl_Position = worldMatrix * vec4(inPosition, 0.0, 1.0);

	// pass on tex coord uvs and normal
	vs_fs_uv = inUV * uvScale;
}