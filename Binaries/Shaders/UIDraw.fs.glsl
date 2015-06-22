#version 330

in vec2 vs_fs_uv;

layout(location = 0) out vec4 outColor;

uniform sampler2D diffuseMap;

uniform bool colorOverride;
uniform bool mrtRGBReinterpretOverride;
uniform bool mrtAReinterpretOverride;
uniform float mrtScale;
uniform vec4 color;

void main()
{
	if (colorOverride)
		outColor = color;
	else
	{
		if (mrtRGBReinterpretOverride)
			outColor = vec4(texture2D(diffuseMap, vs_fs_uv).rgb, 1.0);
		else if (mrtAReinterpretOverride)
		{
			float a = texture2D(diffuseMap, vs_fs_uv).a * mrtScale;
			outColor = vec4(a, a, a, 1.0);
		}
		else
			outColor = texture2D(diffuseMap, vs_fs_uv);
	}
}