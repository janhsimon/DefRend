#version 330

layout(location = 0) out float color;

in vec4 vs_fs_posWS;
in vec2 vs_fs_uv;

uniform sampler2D opacityMap;

uniform vec3 pointLightPosition;
uniform float cameraFarClip;

void main()
{
	if (texture(opacityMap, vs_fs_uv).r < 0.5)
		discard;

	color = length(pointLightPosition - vs_fs_posWS.xyz) / cameraFarClip;
}