#version 330

in vec2 vs_fs_uv;

layout(location = 0) out vec4 outColor;

uniform sampler2D diffuseMap;
uniform bool colorOverride;
uniform vec4 color;

void main()
{
	if (colorOverride)
		outColor = color;
	else
		outColor = texture2D(diffuseMap, vs_fs_uv);
}