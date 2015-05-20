#version 330

in vec2 vs_fs_uv;

layout(location = 0) out vec4 color;

uniform sampler2D diffuseMap;

void main()
{
	color = texture2D(diffuseMap, vs_fs_uv);
}