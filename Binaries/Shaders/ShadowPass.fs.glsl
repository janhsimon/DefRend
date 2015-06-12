#version 330

layout(location = 0) out float color;

in vec4 vs_fs_posWS;

uniform vec3 pointLightPosition;
uniform float cameraFarClip;

void main()
{
	color = length(pointLightPosition - vs_fs_posWS.xyz) / cameraFarClip;
}