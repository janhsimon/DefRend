#version 330

layout(location = 0) out vec4 color;

uniform sampler2D gbuffer_worldPosition;
uniform sampler2D gbuffer_diffuse;
uniform sampler2D gbuffer_normal;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform float lightIntensity;
uniform vec3 lightAttenuation;
uniform vec3 lightDirection;
uniform float lightCutoffCosine;

uniform vec2 screenSize;

vec4 calcLightInternal(vec3 LightDirection, vec3 WorldPos, vec3 Normal)
{
	float diffuseFactor = dot(Normal, -LightDirection);
	return vec4(lightColor, 1.0) * lightIntensity * diffuseFactor;
}

vec4 calcPointLight(vec3 WorldPos, vec3 Normal)
{
	vec3 LightDirection = WorldPos - lightPosition;
	float Distance = length(LightDirection);
	LightDirection = normalize(LightDirection);

	vec4 Color = calcLightInternal(LightDirection, WorldPos, Normal);

	float Attenuation = lightAttenuation.x + lightAttenuation.y * Distance + lightAttenuation.z * Distance * Distance;

	Attenuation = max(1.0, Attenuation);

	return Color / Attenuation;
}

vec4 calcSpotLight(vec3 WorldPos, vec3 Normal)
{
	vec3 LightToPixel = normalize(WorldPos - lightPosition);
	float SpotFactor = dot(LightToPixel, lightDirection);

	if (SpotFactor > lightCutoffCosine) {
		vec4 Color = calcPointLight(WorldPos, Normal);
		return Color * (1.0 - (1.0 - SpotFactor) * 1.0 / (1.0 - lightCutoffCosine));
	}
	else {
		return vec4(0, 0, 0, 0);
	}
}

void main()
{
	vec2 uv = gl_FragCoord.xy / screenSize;

	vec3 worldPos = texture(gbuffer_worldPosition, uv).xyz;
	vec3 diffuse = texture(gbuffer_diffuse, uv).xyz;
	vec3 normal = normalize(texture(gbuffer_normal, uv).xyz);

	color = vec4(diffuse, 1.0) * calcSpotLight(worldPos, normal);
}