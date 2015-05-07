#version 330

layout(location = 0) out vec4 color;

uniform sampler2D gbuffer_worldPosition;
uniform sampler2D gbuffer_diffuse;
uniform sampler2D gbuffer_specular;
uniform sampler2D gbuffer_normal;

uniform vec3 lightPosition;
uniform vec3 lightDiffuseColor;
uniform float lightDiffuseIntensity;
uniform float lightSpecularIntensity;
uniform float lightSpecularPower;
uniform vec3 lightAttenuation;
uniform vec3 lightDirection;
uniform float lightCutoffCosine;

uniform vec3 eyePosition;

uniform vec2 screenSize;

vec4 calcLightInternal(vec3 LightDirection, vec3 WorldPos, vec3 Normal, vec2 uv)
{
	float diffuseFactor = dot(Normal, -LightDirection);
	float SpecularFactor = 0.0;

	vec4 DiffuseColor = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 SpecularColor = vec4(0.0, 0.0, 0.0, 1.0);
	
	if (diffuseFactor > 0.0)
	{
		DiffuseColor = vec4(lightDiffuseColor, 1.0) * lightDiffuseIntensity * diffuseFactor;

		vec3 VertexToEye = normalize(eyePosition - WorldPos);
		vec3 LightReflect = normalize(reflect(LightDirection, Normal));
		SpecularFactor = pow(dot(VertexToEye, LightReflect), lightSpecularPower);

		if (SpecularFactor > 0.0)
		{
			SpecularColor = vec4(1.0, 1.0, 1.0, 1.0) * SpecularFactor * lightSpecularIntensity * texture(gbuffer_specular, uv).r;
		}
	}

	return DiffuseColor + SpecularColor;
}

vec4 calcPointLight(vec3 WorldPos, vec3 Normal, vec2 uv)
{
	vec3 LightDirection = WorldPos - lightPosition;
	float Distance = length(LightDirection);
	LightDirection = normalize(LightDirection);

	vec4 Color = calcLightInternal(LightDirection, WorldPos, Normal, uv);

	// x = constant, y = linear, z = exponent
	float Attenuation = lightAttenuation.x + lightAttenuation.y * Distance + lightAttenuation.z * Distance * Distance;

	Attenuation = max(1.0, Attenuation);

	return Color / Attenuation;
}

vec4 calcSpotLight(vec3 WorldPos, vec3 Normal, vec2 uv)
{
	vec3 LightToPixel = normalize(WorldPos - lightPosition);
	float SpotFactor = dot(LightToPixel, lightDirection);

	if (SpotFactor > lightCutoffCosine) {
		vec4 Color = calcPointLight(WorldPos, Normal, uv);
		return Color * (1.0 - (1.0 - SpotFactor) * 1.0 / (1.0 - lightCutoffCosine));
	}
	else {
		return vec4(0, 0, 0, 0);
	}
}

void main()
{
	vec2 uv = gl_FragCoord.xy / screenSize;

	vec3 worldPos = texture(gbuffer_worldPosition, uv).rgb;
	vec3 diffuse = texture(gbuffer_diffuse, uv).rgb;
	float specular = texture(gbuffer_specular, uv).r;
	vec3 normal = normalize(texture(gbuffer_normal, uv).rgb);

	color = vec4(diffuse, 1.0) * calcSpotLight(worldPos, normal, uv) + specular * 0.00001;
}