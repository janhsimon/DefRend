#version 330

layout(location = 0) out vec4 color;

uniform sampler2D gbuffer_worldPosition;
uniform sampler2D gbuffer_diffuse;
uniform sampler2D gbuffer_specular;
uniform sampler2D gbuffer_normal;

uniform vec3 lightDirection;
uniform vec3 lightDiffuseColor;
uniform float lightDiffuseIntensity;

uniform vec2 screenSize;

void main()
{
	vec2 uv = gl_FragCoord.xy / screenSize;

	vec3 worldPos = texture(gbuffer_worldPosition, uv).rgb;
	vec3 diffuse = texture(gbuffer_diffuse, uv).rgb;
	float specular = texture(gbuffer_specular, uv).r;
	vec3 normal = normalize(texture(gbuffer_normal, uv).rgb);

	color = vec4(diffuse + worldPos * 0.00001 + specular * 0.00001, 1.0) * (dot(-lightDirection, normal) * vec4(lightDiffuseColor, 1.0) * lightDiffuseIntensity);
}