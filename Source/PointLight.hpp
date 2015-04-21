#pragma once

struct PointLight
{
	float position[3];
	float color[3];
	float intensity;
	float attenuation[3]; // constant, linear, exponent
};