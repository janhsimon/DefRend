#version 330

in vec2 vs_fs_uv;
in vec3 vs_fs_normal;
in vec3 vs_fs_tangent;
in vec3 vs_fs_bitangent;
in vec4 vs_fs_posWS;
in vec4 vs_fs_posVS;

layout(location = 0) out vec4 outGBufferMRT0;
layout(location = 1) out vec4 outGBufferMRT1;

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

	vec3 nPacked = normalize(tbn * normal);
	
	// from [-1, 1] to [0, 1]
	nPacked = 0.5 * nPacked + vec3(0.5);

	return nPacked;
}

void main()
{
	if (texture(opacityMap, vs_fs_uv).r < 0.5)
		discard;

	outGBufferMRT0 = vec4(texture(diffuseMap, vs_fs_uv).rgb, length(vs_fs_posVS.xyz));
	outGBufferMRT1 = vec4(calcTangentSpaceNormal(), texture(specularMap, vs_fs_uv).r);
}