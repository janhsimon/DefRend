#version 330

layout(location = 0) out vec4 color;

//in vec4 vs_fs_positionVS;
//in mat4 vs_fs_invWorldMatrix;
in vec3 vs_fs_viewRay;
in vec3 vs_fs_eyePosition;

uniform sampler2D inGBufferMRT0;
uniform sampler2D inGBufferMRT1;

uniform vec3 lightPosition;
uniform vec3 lightDiffuseColor;
uniform float lightDiffuseIntensity;
uniform float lightSpecularIntensity;
uniform float lightSpecularPower;
//uniform vec3 lightAttenuation;

uniform vec2 screenSize;

uniform float cameraFarClip;

vec3 reconstructFromDepth(float depth)
{
	//vec3 frustumRay = vs_fs_positionVS.xyz * (cameraFarClip / -vs_fs_positionVS.z);
	//frustumRay = vec3(vs_fs_invWorldMatrix * vec4(frustumRay, 0.0));
	//return frustumRay * depth;

	vec3 viewRay = normalize(vs_fs_viewRay);
	return vs_fs_eyePosition + viewRay * depth;
}

vec4 calcLightInternal(vec3 LightDirection, vec3 WorldPos, vec3 Normal, vec2 uv, float Distance)
{
	float diffuseFactor = dot(Normal, -LightDirection);
	float SpecularFactor = 0.0;

	vec4 DiffuseColor = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 SpecularColor = vec4(0.0, 0.0, 0.0, 1.0);

	if (diffuseFactor > 0.0)
	{
		DiffuseColor = vec4(lightDiffuseColor, 1.0) /** lightDiffuseIntensity*/ * diffuseFactor;

		vec3 VertexToEye = normalize(vs_fs_eyePosition - WorldPos);
		vec3 LightReflect = normalize(reflect(LightDirection, Normal));
		SpecularFactor = pow(dot(VertexToEye, LightReflect), lightSpecularPower);

		if (SpecularFactor > 0.0)
		{
			SpecularColor = vec4(1.0, 1.0, 1.0, 1.0) * SpecularFactor /** lightSpecularIntensity*/ * texture(inGBufferMRT1, uv).a;
		}
	}

	float diffuseAttenuationFactor = 1.0 - sqrt(Distance / lightDiffuseIntensity);
	float specularAttenuationFactor = 1.0 - sqrt(Distance / lightSpecularIntensity);

	return DiffuseColor * clamp(diffuseAttenuationFactor, 0.0, 1.0) + SpecularColor * clamp(specularAttenuationFactor, 0.0, 1.0);
}

vec4 calcPointLight(vec3 WorldPos, vec3 Normal, vec2 uv)
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

	color = vec4(diffuse, 1.0) * calcPointLight(worldPosition, normal, uv);
}