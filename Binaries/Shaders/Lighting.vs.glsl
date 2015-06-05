#version 330

layout(location = 0) in vec3 inPosition;

//out vec4 vs_fs_positionVS;
//out mat4 vs_fs_invWorldMatrix;
out vec3 vs_fs_viewRay;
out vec3 vs_fs_eyePosition;

uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform vec3 eyePosition;

//uniform mat4 invWorldMatrix;

void main()
{
	vec4 posWS = worldMatrix * vec4(inPosition, 1.0);

	vs_fs_viewRay = posWS.xyz - eyePosition;

	//vs_fs_positionVS = viewMatrix * worldMatrix * vec4(inPosition, 1.0);
	gl_Position = projectionMatrix * viewMatrix * worldMatrix * vec4(inPosition, 1.0);

	vs_fs_eyePosition = eyePosition;

	//vs_fs_invWorldMatrix = invWorldMatrix;
}