#version 330

layout(location = 0) out vec4 color;

in vec3 vs_fs_viewRay;
in vec3 vs_fs_eyePosition;

uniform sampler2D inGBufferMRT0;
uniform sampler2D inGBufferMRT1;
uniform samplerCube inShadowMap;

uniform vec3 lightPosition;
uniform vec3 lightDiffuseColor;
uniform float lightDiffuseIntensity;
uniform float lightSpecularIntensity;
uniform float lightSpecularPower;
uniform bool lightCastShadows;

uniform float lightShadowBias;

uniform vec2 screenSize;

uniform float cameraFarClip;

vec3 reconstructFromDepth(float depth)
{
	vec3 viewRay = normalize(vs_fs_viewRay);
	return vs_fs_eyePosition + viewRay * depth;
}

vec3 calcLightInternal(vec3 LightDirection, vec3 WorldPos, vec3 Normal, vec2 uv, float Distance)
{
	float diffuseFactor = dot(Normal, -LightDirection);
	float SpecularFactor = 0.0;

	vec3 DiffuseColor = vec3(0.0, 0.0, 0.0);
	vec3 SpecularColor = vec3(0.0, 0.0, 0.0);

	if (diffuseFactor > 0.0)
	{
		DiffuseColor = lightDiffuseColor * diffuseFactor;

		vec3 VertexToEye = normalize(vs_fs_eyePosition - WorldPos);
		vec3 LightReflect = normalize(reflect(LightDirection, Normal));
		SpecularFactor = pow(dot(VertexToEye, LightReflect), lightSpecularPower);

		if (SpecularFactor > 0.0)
			SpecularColor = lightDiffuseColor * SpecularFactor * texture(inGBufferMRT1, uv).a;
	}

	float diffuseAttenuationFactor = 1.0 - sqrt(Distance / lightDiffuseIntensity);
	float specularAttenuationFactor = 1.0 - sqrt(Distance / lightSpecularIntensity);

	return DiffuseColor * clamp(diffuseAttenuationFactor, 0.0, 1.0) + SpecularColor * clamp(specularAttenuationFactor, 0.0, 1.0);
}

vec3 calcPointLight(vec3 WorldPos, vec3 Normal, vec2 uv)
{
	vec3 LightDirection = WorldPos - lightPosition;
	float Distance = length(LightDirection);
	LightDirection = normalize(LightDirection);

	return calcLightInternal(LightDirection, WorldPos, Normal, uv, Distance);
}

void main()
{
	vec2 uv = gl_FragCoord.xy / screenSize;

	vec3 diffuse = texture(inGBufferMRT0, uv).rgb;
	vec3 worldPosition = reconstructFromDepth(texture(inGBufferMRT0, uv).a);
	vec3 normal = normalize(texture(inGBufferMRT1, uv).rgb);

	if (lightCastShadows)
	{
		vec3  light_to_pixel = worldPosition - lightPosition;
		float lightRadius = max(lightDiffuseIntensity, lightSpecularIntensity);
		float fDepth = length(light_to_pixel) / lightRadius;
		float vShadowSample = texture(inShadowMap, light_to_pixel).r;

		float shadow_factor = 0.0;

		if (vShadowSample > fDepth * lightShadowBias)
			shadow_factor = 1.0;

		color = vec4(diffuse * calcPointLight(worldPosition, normal, uv) * shadow_factor, 1.0);
	}
	else
		color = vec4(diffuse * calcPointLight(worldPosition, normal, uv), 1.0);
}