#version 330

in vec2 vs_fs_uv;
in vec3 vs_fs_normal;
in vec3 vs_fs_tangent;
in vec3 vs_fs_bitangent;
in vec4 vs_fs_posWS;
in vec4 vs_fs_posVS;

layout(location = 0) out vec3 outGBufferMRT0;
layout(location = 1) out vec3 outGBufferMRT1;
layout(location = 2) out vec3 outGBufferMRT2;
layout(location = 3) out vec3 outGBufferMRT3;

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform sampler2D normalMap;
uniform sampler2D opacityMap;

vec3 calcTangentSpaceNormal()
{
	vec3 n = normalize(vs_fs_normal);
	vec3 t = normalize(vs_fs_tangent);
	vec3 b = normalize(vs_fs_bitangent);
	mat3 tbn = mat3(t, b, n);

	vec3 normal = texture(normalMap, vs_fs_uv).rgb;

	// from [0, 1] to [-1, 1]
	normal = 2.0 * normal - vec3(1.0);

	return normalize(tbn * normal);
}

void main()
{
	if (texture(opacityMap, vs_fs_uv).r < 0.5)
		discard;

	outGBufferMRT0 = texture(diffuseMap, vs_fs_uv).rgb;
	outGBufferMRT1 = 0.5 * calcTangentSpaceNormal() + vec3(0.5);
	outGBufferMRT2 = vec3(texture(specularMap, vs_fs_uv).r, 0.0, 0.0);
	outGBufferMRT3 = vs_fs_posWS.xyz;
}