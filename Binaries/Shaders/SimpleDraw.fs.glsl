#version 330

in vec2 vs_fs_uv;
in vec3 vs_fs_normal;

layout (location = 0) out vec4 color;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D opacityMap;

void main()
{
	vec3 diffuseColor = texture2D(diffuseMap, vs_fs_uv).rgb + texture2D(normalMap, vs_fs_uv).rgb * 0.000001;
	color = vec4(diffuseColor, texture2D(opacityMap, vs_fs_uv).r);
}