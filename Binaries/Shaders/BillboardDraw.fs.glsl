#version 330

in vec2 vs_fs_uv;

layout(location = 0) out vec4 color;

uniform sampler2D diffuseMap;

uniform vec3 tintColor;

void main()
{
	vec4 tex = texture2D(diffuseMap, vs_fs_uv);
	color = vec4(tex.rgb * tintColor, tex.a);
}