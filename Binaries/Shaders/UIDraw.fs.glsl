#version 330

in vec2 vs_fs_uv;

layout(location = 0) out vec4 outColor;

uniform /*u*/sampler2D diffuseMap;

uniform int mode; // 0 = custom texture, 1 = single color, 2 = gbuffer diffuse, 3 = gbuffer specular, 4 = gbuffer normal, 5 = gbuffer depth
uniform vec4 color;

/*
vec3 unpackNormal(vec2 uv)
{
	vec3 normal;

	uint nr = texture(diffuseMap, uv).r;
	uint ng = texture(diffuseMap, uv).g;

	uint nx = nr >> 3;
	uint nz = ng & 31u;

	uint ny = (nr & 7u) << 3;
	ny = ny | (ng >> 5);

	// [0, 1]
	normal.x = float(nx) / 31.0; // 5-bit
	normal.y = float(ny) / 63.0; // 6-bit
	normal.z = float(nz) / 31.0; // 5-bit

	return normalize(normal);
}

float unpackDepth(vec2 uv)
{
	uint l = texture(diffuseMap, uv).z << 8;
	uint r = texture(diffuseMap, uv).a;
	uint depth = l | r;
	return float(depth) / 65535.0;
}
*/

void main()
{
	if (mode == 0)
	// custom texture
		outColor = texture(diffuseMap, vs_fs_uv);
	else if (mode == 1)
	// single color
		outColor = color;
	else if (mode == 2)
	// gbuffer diffuse
	{
		/*
		float r = float(texture(diffuseMap, vs_fs_uv).r) / 255.0;
		float g = float(texture(diffuseMap, vs_fs_uv).g) / 255.0;
		float b = float(texture(diffuseMap, vs_fs_uv).b) / 255.0;

		outColor = vec4(r, g, b, 1.0);
		*/

		outColor = vec4(texture(diffuseMap, vs_fs_uv).rgb, 1.0);
	}
	else if (mode == 3)
	// gbuffer specular
	{
		/*
		float a = float(texture(diffuseMap, vs_fs_uv).a) / 255.0;
		outColor = vec4(a, a, a, 1.0);
		*/

		float a = texture(diffuseMap, vs_fs_uv).r;
		outColor = vec4(a, a, a, 1.0);
	}
	else if (mode == 4)
	// gbuffer normal
	{
		//outColor = vec4(unpackNormal(vs_fs_uv), 1.0);

		outColor = vec4(texture(diffuseMap, vs_fs_uv).rgb, 1.0);
	}
	else if (mode == 5)
	// gbuffer depth
	{
		/*
		float a = unpackDepth(vs_fs_uv);
		outColor = vec4(a, a, a, 1.0);
		*/

		outColor = vec4(texture(diffuseMap, vs_fs_uv).rgb, 1.0);
	}
}