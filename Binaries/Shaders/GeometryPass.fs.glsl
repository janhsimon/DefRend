#version 330

in vec2 vs_fs_uv;
in vec3 vs_fs_normal;
in vec3 vs_fs_worldPosition;
in vec3 vs_fs_tangent;

layout(location = 0) out vec3 outWorldPosition;
layout(location = 1) out vec3 outDiffuse;
layout(location = 2) out vec3 outNormal;
//layout(location = 3) out vec3 outUV;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D opacityMap;

vec3 calcTangentSpaceNormal()
{
	vec3 n = normalize(vs_fs_normal);
	vec3 t = normalize(vs_fs_tangent);
	vec3 b = normalize(cross(t, n)); // normalize() shouldn't be necessary but let's be safe
	mat3 tbn = mat3(t, b, n);

	vec3 normal = texture(normalMap, vs_fs_uv).rgb;
	normal = 2.0 * normal - vec3(1.0, 1.0, 1.0);
	return normalize(tbn * normal);
}

void main()
{
	outWorldPosition = vs_fs_worldPosition;
	outDiffuse = texture(diffuseMap, vs_fs_uv).rgb + texture(opacityMap, vs_fs_uv).rgb * 0.0001;
	outNormal = calcTangentSpaceNormal();
	//outUV = vec3(vs_fs_uv, 0.0);
}