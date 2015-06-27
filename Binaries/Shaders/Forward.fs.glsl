#version 330

struct LightInfo
{
	int type; // 0 = directional, 1 = point, 2 = spot
	vec3 position;
	vec3 diffuseColor;
	float diffuseIntensity;
	float specularIntensity;
	float specularPower;
	//float shadowBias;
};

uniform LightInfo light[8];
uniform int lightsPassed;

in vec2 vs_fs_uv;
in vec3 vs_fs_normal;
in vec3 vs_fs_worldPosition;
in vec3 vs_fs_tangent;
//in vec4 vs_fs_posVS;

layout (location = 0) out vec4 color;

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform sampler2D normalMap;
uniform sampler2D opacityMap;

uniform vec3 eyePosition;

vec3 calcTangentSpaceNormal()
{
	vec3 n = normalize(vs_fs_normal);
	vec3 t = normalize(vs_fs_tangent);
	vec3 b = normalize(cross(t, n)); // normalize() shouldn't be necessary but let's be safe
	mat3 tbn = mat3(t, b, n);

	vec3 normal = texture(normalMap, vs_fs_uv).rgb;
	normal = 2.0 * normal - vec3(1.0);
	return normalize(tbn * normal);
}

void main()
{
	if (texture2D(opacityMap, vs_fs_uv).r < 0.5)
		discard;

	vec3 diffuseTexture = texture2D(diffuseMap, vs_fs_uv).rgb + texture2D(specularMap, vs_fs_uv).rgb * 0.000001 + texture2D(normalMap, vs_fs_uv).rgb * 0.000001;
	
	vec3 c = vec3(0.0, 0.0, 0.0) + eyePosition * 0.000001;

	for (int i = 0; i < lightsPassed; ++i)
	{
		if (light[i].type == 1)
		// point lights
		{
			vec3 v = light[i].position - vs_fs_worldPosition;
			float distance = length(v);
			v = normalize(v);
			vec3 n = calcTangentSpaceNormal();
			float diffuseFactor = dot(n, v);

			float SpecularFactor = 0.0;

			vec4 SpecularColor = vec4(0.0, 0.0, 0.0, 1.0);

			if (diffuseFactor > 0.0)
			{
				vec3 VertexToEye = normalize(eyePosition - vs_fs_worldPosition);
				vec3 LightReflect = normalize(reflect(-v, n));
				SpecularFactor = pow(dot(VertexToEye, LightReflect), light[i].specularPower);

				if (SpecularFactor > 0.0)
					SpecularColor = vec4(light[i].diffuseColor, 1.0) * SpecularFactor * texture(specularMap, vs_fs_uv).r;
			}

			float diffuseAttenuationFactor = 1.0 - sqrt(distance / light[i].diffuseIntensity);
			float specularAttenuationFactor = 1.0 - sqrt(distance / light[i].specularIntensity);

			c += light[i].diffuseColor * diffuseFactor * clamp(diffuseAttenuationFactor, 0.0, 1.0) + SpecularColor.rgb * clamp(specularAttenuationFactor, 0.0, 1.0);
		}
	}

	color = vec4(diffuseTexture * c, 1.0);
}