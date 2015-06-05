#version 330

layout(location = 0) out vec4 color;

uniform sampler2D inGBufferMRT0;
uniform sampler2D inGBufferMRT1;

uniform vec3 lightDirection;
uniform vec3 lightDiffuseColor;
uniform float lightDiffuseIntensity;

uniform vec2 screenSize;

void main()
{
	vec2 uv = gl_FragCoord.xy / screenSize;

	vec3 diffuse = texture(inGBufferMRT0, uv).rgb;
	vec3 normal = normalize(texture(inGBufferMRT1, uv).rgb);

	color = vec4(diffuse, 1.0) * (dot(-lightDirection, normal) * vec4(lightDiffuseColor, 1.0) * lightDiffuseIntensity);
}