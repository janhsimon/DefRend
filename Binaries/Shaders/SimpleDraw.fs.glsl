#version 330

in vec2 vs_fs_uv;
in vec3 vs_fs_normal;

layout (location = 0) out vec4 color;

uniform sampler2D diffuseTexture;

void main(void)
{
	vec4 diffuseTexture = texture2D(diffuseTexture, vs_fs_uv);
	color = diffuseTexture;
}
