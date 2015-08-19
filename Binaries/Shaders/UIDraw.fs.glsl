#version 330

in vec2 vs_fs_uv;

layout(location = 0) out vec4 outColor;

uniform sampler2D textureMap;
uniform usampler2D uintTextureMap;

uniform int mode; // 0 = custom texture, 1 = single color, 2 = gbuffer diffuse, 3 = gbuffer specular, 4 = gbuffer normal, 5 = gbuffer depth
uniform int gBufferLayout; // 0 = super slim, 1 = slim, 2 = fat
uniform vec4 color;
uniform float farClip;

vec3 unpackNormal(vec2 uv)
{
	vec3 normal;

	uint nr = texture(uintTextureMap, uv).r;
	uint ng = texture(uintTextureMap, uv).g;

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
	uint l = texture(uintTextureMap, uv).z << 8;
	uint r = texture(uintTextureMap, uv).a;
	uint depth = l | r;
	return float(depth) / 65535.0;
}

void main()
{
	if (mode == 0)
	// custom texture
		outColor = texture(textureMap, vs_fs_uv);
	else if (mode == 1)
	// single color
		outColor = color;
	else if (mode == 2)
	// gbuffer diffuse
	{
		if (gBufferLayout == 0)
		// super slim
		{
			float r = float(texture(uintTextureMap, vs_fs_uv).r) / 255.0;
			float g = float(texture(uintTextureMap, vs_fs_uv).g) / 255.0;
			float b = float(texture(uintTextureMap, vs_fs_uv).b) / 255.0;

			outColor = vec4(r, g, b, 1.0);
		}
		else if (gBufferLayout == 1 || gBufferLayout == 2)
		// slim or fat
			outColor = vec4(texture(textureMap, vs_fs_uv).rgb, 1.0);
	}
	else if (mode == 3)
	// gbuffer specular
	{
		if (gBufferLayout == 0)
		// super slim
		{
			float a = float(texture(uintTextureMap, vs_fs_uv).a) / 255.0;
			outColor = vec4(a, a, a, 1.0);
		}
		else if (gBufferLayout == 1)
		// slim
		{
			float a = texture(textureMap, vs_fs_uv).a;
			outColor = vec4(a, a, a, 1.0);
		}
		else if (gBufferLayout == 2)
		// fat
		{
			float r = texture(textureMap, vs_fs_uv).r;
			outColor = vec4(r, r, r, 1.0);
		}
		
	}
	else if (mode == 4)
	// gbuffer normal
	{
		if (gBufferLayout == 0)
		// super slim
			outColor = vec4(unpackNormal(vs_fs_uv), 1.0);
		else if (gBufferLayout == 1 || gBufferLayout == 2)
		// slim or fat
			outColor = vec4(texture(textureMap, vs_fs_uv).rgb, 1.0);
	}
	else if (mode == 5)
	// gbuffer depth
	{
		if (gBufferLayout == 0)
		// super slim
		{
			float a = unpackDepth(vs_fs_uv);
			outColor = vec4(a, a, a, 1.0);
		}
		else if (gBufferLayout == 1)
		// slim
		{
			float a = texture(textureMap, vs_fs_uv).a / farClip;
			outColor = vec4(a, a, a, 1.0);
		}
		else if (gBufferLayout == 2)
		// fat
		{
			// this is actually worldspace position
			outColor = vec4(texture(textureMap, vs_fs_uv).rgb, 1.0);
		}
	}
}