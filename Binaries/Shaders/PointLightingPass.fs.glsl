#version 330

layout(location = 0) out vec4 color;

uniform sampler2D gbuffer_worldPosition;
uniform sampler2D gbuffer_diffuse;
uniform sampler2D gbuffer_specular;
uniform sampler2D gbuffer_normal;

uniform vec3 lightPosition;
uniform vec3 lightDiffuseColor;
uniform float lightDiffuseIntensity;
uniform vec3 lightSpecularColor;
uniform float lightSpecularIntensity;
uniform float lightSpecularPower;
uniform vec3 lightAttenuation;

uniform vec3 eyeWorldPosition;

uniform vec2 screenSize;

vec4 calcLightInternal(vec3 LightDirection, vec3 WorldPos, vec3 Normal, vec2 uv)
{
	float diffuseFactor = dot(Normal, -LightDirection);

	vec4 DiffuseColor = vec4(0, 0, 0, 0);
	vec4 SpecularColor = vec4(0, 0, 0, 0);

	if (diffuseFactor > 0.0)
	{
		DiffuseColor = vec4(lightDiffuseColor, 1.0) * lightDiffuseIntensity * diffuseFactor;

		vec3 VertexToEye = normalize(eyeWorldPosition - WorldPos);
		vec3 LightReflect = normalize(reflect(LightDirection, Normal));
		float SpecularFactor = dot(VertexToEye, LightReflect);
		SpecularFactor = pow(SpecularFactor, lightSpecularPower);
		
		if (SpecularFactor > 0.0)
		{
			SpecularColor = vec4(lightSpecularColor, 1.0) * lightSpecularIntensity * texture(gbuffer_specular, uv).r * SpecularFactor;
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

	float Attenuation = lightAttenuation.x + lightAttenuation.y * Distance + lightAttenuation.z * Distance * Distance;

	Attenuation = max(1.0, Attenuation);

	return Color / Attenuation;
}

void main()
{
	vec2 uv = gl_FragCoord.xy / screenSize;

	vec3 worldPos = texture(gbuffer_worldPosition, uv).rgb;
	vec3 diffuse = texture(gbuffer_diffuse, uv).rgb;
	vec3 normal = normalize(texture(gbuffer_normal, uv).rgb);

	color = vec4(diffuse, 1.0) * calcPointLight(worldPos, normal, uv);
}