#version 330

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inUV;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec3 inTangent;
layout(location = 4) in vec3 inBitangent;

out vec2 vs_fs_uv;
out vec3 vs_fs_normal;
out vec3 vs_fs_tangent;
out vec3 vs_fs_bitangent;
out vec4 vs_fs_posWS;
out vec4 vs_fs_posVS;

uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
	// bring vertex position from model- to screenspace
	vs_fs_posWS = worldMatrix * vec4(inPosition, 1.0);
	vs_fs_posVS = viewMatrix * vs_fs_posWS;
	gl_Position = projectionMatrix * vs_fs_posVS;
	
	// pass on tex coord uvs
	vs_fs_uv = inUV;

	// bring normal, tangent, bitangent and world-position from model- to worldspace and pass them on
	vs_fs_normal = (worldMatrix * vec4(inNormal, 0.0)).xyz;
	vs_fs_tangent = (worldMatrix * vec4(inTangent, 0.0)).xyz;
	vs_fs_bitangent = (worldMatrix * vec4(inBitangent, 0.0)).xyz;
}