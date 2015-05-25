#pragma once

#include "PointLight.hpp"

class SpotLight : public PointLight
{
public:
	float cutoffAngle;
};