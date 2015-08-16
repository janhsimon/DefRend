#version 330

struct LightInfo
{
	int type; // 0 = directional, 1 = point, 2 = spot
	vec3 position;
	vec3 direction;
	vec3 diffuseColor;
	float diffuseIntensity;
	float specularIntensity;
	float specularPower;
	//float shadowBias;
	float cutoffCosine;
};

uniform LightInfo light[33];
uniform int lightsPassed;

in vec2 vs_fs_uv;
in vec3 vs_fs_normal;
in vec3 vs_fs_worldPosition;
in vec3 vs_fs_tangent;
in vec3 vs_fs_bitangent;
//in vec4 vs_fs_posVS;

layout (location = 0) out vec4 color;

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform sampler2D normalMap;
uniform sampler2D opacityMap;

uniform vec3 eyePosition;

vec3 calcTangentSpaceNormal()
{
	vec3 t = normalize(vs_fs_tangent);
	vec3 b = normalize(vs_fs_bitangent);
	vec3 n = normalize(vs_fs_normal);
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

	vec3 c = vec3(0.0);

	for (int i = 0; i < lightsPassed; ++i)
	{
		if (light[i].type == 0)
		// directional light
		{
			vec3 v = -light[i].direction;
			v = normalize(v);
			vec3 n = calcTangentSpaceNormal();
			float diffuseTerm = clamp(dot(v, n), 0.0, 1.0);
			c += diffuseTerm * light[i].diffuseColor;
		}
		else
		// point or spot light
		{
			vec3 v = light[i].position - vs_fs_worldPosition;
			float distance = length(v);
			v = normalize(v);
			vec3 n = calcTangentSpaceNormal();
			float diffuseFactor = clamp(dot(v, n), 0.0, 1.0);

			float specularFactor = 0.0;
			
			if (diffuseFactor > 0.0)
			{
				vec3 VertexToEye = normalize(eyePosition - vs_fs_worldPosition);
				vec3 LightReflect = normalize(reflect(-v, n));
				float spec = pow(dot(VertexToEye, LightReflect), light[i].specularPower);

				if (spec > 0.0)
					specularFactor = spec * texture(specularMap, vs_fs_uv).r;
			}

			float diffuseAttenuationFactor = 0.0;

			if (light[i].diffuseIntensity > 0.0)
				diffuseAttenuationFactor = 1.0 - sqrt(distance / light[i].diffuseIntensity);
				
			float specularAttenuationFactor = 0.0;

			if (light[i].specularIntensity > 0.0)
				specularAttenuationFactor = 1.0 - sqrt(distance / light[i].specularIntensity);
				
			float diffuseTerm = diffuseFactor * clamp(diffuseAttenuationFactor, 0.0, 1.0);
			float specularTerm = specularFactor * clamp(specularAttenuationFactor, 0.0, 1.0);
			
			if (light[i].type == 2)
			// spot lighting
			{
				vec3 LightToPixel = normalize(-v);
				float SpotFactor = dot(LightToPixel, light[i].direction);

				float spot = 0.0;

				if (SpotFactor > light[i].cutoffCosine)
					spot = (1.0 - (1.0 - SpotFactor) * 1.0 / (1.0 - light[i].cutoffCosine));

				c += (diffuseTerm + specularTerm) * light[i].diffuseColor * spot;
			}
			else if (light[i].type == 1)
			// point lighting
				c += (diffuseTerm + specularTerm) * light[i].diffuseColor;
		}
	}

	vec3 diffuseTexture = texture(diffuseMap, vs_fs_uv).rgb;
	color = vec4(diffuseTexture * c, 1.0);
}