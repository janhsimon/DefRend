#version 330

layout(location = 0) in vec3 inPosition;

out vec3 vs_fs_viewRay;
out vec3 vs_fs_eyePosition;

uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform vec3 eyePosition;

void main()
{
	vec4 posWS = worldMatrix * vec4(inPosition, 1.0);
	vs_fs_viewRay = posWS.xyz - eyePosition;
	gl_Position = projectionMatrix * viewMatrix * posWS;
	vs_fs_eyePosition = eyePosition;
}