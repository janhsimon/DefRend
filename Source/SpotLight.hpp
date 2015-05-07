#pragma once

#include "PointLight.hpp"

struct SpotLight : public PointLight
{
	float direction[3];
	float cutoffAngle;
};