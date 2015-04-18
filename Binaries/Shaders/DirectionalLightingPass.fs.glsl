#version 330

layout(location = 0) out vec4 color;

uniform sampler2D gbuffer_worldPosition;
uniform sampler2D gbuffer_diffuse;
uniform sampler2D gbuffer_normal;

uniform vec2 screenSize;

float calcDirectionalLight(vec3 normal)
{
	return dot(vec3(-1.0, 1.0, -1.0), normal);
}

void main()
{
	vec2 uv = gl_FragCoord.xy / screenSize;

	vec3 worldPos = texture(gbuffer_worldPosition, uv).rgb;
	vec3 diffuse = texture(gbuffer_diffuse, uv).rgb;
	vec3 normal = normalize(texture(gbuffer_normal, uv).rgb);

	color = vec4(0.05, 0.05, 0.05, 1.0) + vec4(diffuse + worldPos * 0.00001, 1.0) * calcDirectionalLight(normal);
}