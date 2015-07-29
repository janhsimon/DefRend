#version 330

layout(location = 0) out vec4 color;

in vec3 vs_fs_viewRay;
in vec3 vs_fs_eyePosition;

uniform sampler2D inGBufferMRT0;
uniform sampler2D inGBufferMRT1;
uniform samplerCube inShadowMap;

uniform vec2 screenSize;

struct LightInfo
{
	int type; // 0 = directional, 1 = point, 2 = spot
	vec3 position;
	vec3 direction;
	vec3 diffuseColor;
	float diffuseIntensity;
	float specularIntensity;
	float specularPower;
	bool castShadows;
	float shadowBias;
	float cutoffCosine;
};

uniform LightInfo light;

vec3 reconstructFromDepth(float depth)
{
	vec3 viewRay = normalize(vs_fs_viewRay);
	return vs_fs_eyePosition + viewRay * depth;
}

vec3 unpackNormal(vec2 uv)
{
	// from [0, 1] to [-1, 1]
	vec3 normal = 2.0 * texture(inGBufferMRT1, uv).rgb - vec3(1.0);
	return normalize(normal);
}

void main()
{
	vec2 uv = gl_FragCoord.xy / screenSize;

	vec3 diffuseTextureColor = texture(inGBufferMRT0, uv).rgb;
	vec3 normal = unpackNormal(uv);

	vec3 lightDirection = normalize(light.direction);

	if (light.type == 0)
	// directional light
		color = vec4(diffuseTextureColor * light.diffuseColor, 1.0) * (dot(-lightDirection, normal) * light.diffuseIntensity);
	else
	// point or spot light
	{
		vec3 worldPosition = reconstructFromDepth(texture(inGBufferMRT0, uv).a);
		
		vec3 lightToFragment = worldPosition - light.position;
		float distance = length(lightToFragment);
		lightToFragment = normalize(lightToFragment);

		float diffuseFactor = dot(normal, -lightToFragment);
		float SpecularFactor = 0.0;

		vec3 DiffuseColor = vec3(0.0, 0.0, 0.0);
		vec3 SpecularColor = vec3(0.0, 0.0, 0.0);

		if (diffuseFactor > 0.0)
		{
			DiffuseColor = light.diffuseColor * diffuseFactor;

			vec3 VertexToEye = normalize(vs_fs_eyePosition - worldPosition);
			vec3 LightReflect = normalize(reflect(lightToFragment, normal));
			SpecularFactor = pow(dot(VertexToEye, LightReflect), light.specularPower);

			float materialSpecularReflectivity = texture(inGBufferMRT1, uv).a;

			if (SpecularFactor > 0.0)
				SpecularColor = light.diffuseColor * SpecularFactor * materialSpecularReflectivity;
		}

		float diffuseAttenuationFactor = 1.0 - sqrt(distance / light.diffuseIntensity);
		float specularAttenuationFactor = 1.0 - sqrt(distance / light.specularIntensity);

		color = vec4(diffuseTextureColor * (DiffuseColor * clamp(diffuseAttenuationFactor, 0.0, 1.0) + SpecularColor * clamp(specularAttenuationFactor, 0.0, 1.0)), 1.0);

		if (light.type == 1)
		// point light
		{
			if (light.castShadows)
			{
				vec3  light_to_pixel = worldPosition - light.position;
				float lightRadius = max(light.diffuseIntensity, light.specularIntensity);
				float fDepth = length(light_to_pixel) / lightRadius;
				float vShadowSample = texture(inShadowMap, light_to_pixel).r;

				float shadow_factor = 0.0;

				if (vShadowSample > fDepth * light.shadowBias)
					shadow_factor = 1.0;

				color *= shadow_factor;
			}
		}
		else
		// spot light
		{
			float SpotFactor = dot(lightToFragment, lightDirection);

			if (SpotFactor > light.cutoffCosine)
				color *= (1.0 - ((1.0 - SpotFactor) * (1.0 / (1.0 - light.cutoffCosine))));
			else
				color = vec4(0.0);
		}
	}
}