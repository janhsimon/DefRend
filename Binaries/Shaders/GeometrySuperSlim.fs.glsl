#version 330

in vec2 vs_fs_uv;
in vec3 vs_fs_normal;
in vec3 vs_fs_tangent;
in vec3 vs_fs_bitangent;
in vec4 vs_fs_posWS;
in vec4 vs_fs_posVS;

layout(location = 0) out uvec4 outGBufferMRT0;
layout(location = 1) out uvec4 outGBufferMRT1;

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform sampler2D normalMap;
uniform sampler2D opacityMap;

uniform float cameraFarClip;

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

	uint r = uint(texture(diffuseMap, vs_fs_uv).r * 255.0);
	uint g = uint(texture(diffuseMap, vs_fs_uv).g * 255.0);
	uint b = uint(texture(diffuseMap, vs_fs_uv).b * 255.0);
	uint a = uint(texture(specularMap, vs_fs_uv).r * 255.0);

	outGBufferMRT0 = uvec4(r, g, b, a);

	vec3 normal = calcTangentSpaceNormal();
	normal = 0.5 * normal + vec3(0.5); // from [-1, 1] to [0, 1]

	uint nx = uint(normal.x * 31.0); // 5-bit
	uint ny = uint(normal.y * 63.0); // 6-bit
	uint nz = uint(normal.z * 31.0); // 5-bit

	uint nl = ny >> 3; // left half of the y component, 3-bit
	uint nr = ny & 7u; // right half of the y component, 3-bit

	// construct the first 8-bit
	uint nred = nx << 3;
	nred = nred | nl;

	// and the second 8-bit
	uint ngreen = nz;
	ngreen = ngreen | (nr << 5);

	uint depth = uint((length(vs_fs_posVS.xyz) / cameraFarClip) * 65535.0);
	uint depthUpperBits = depth >> 8;
	uint depthLowerBits = depth & 255u;

	outGBufferMRT1 = uvec4(nred, ngreen, depthUpperBits, depthLowerBits);
}