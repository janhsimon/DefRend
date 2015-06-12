#include "PointLight.hpp"
#include "..\Util\Util.hpp"

bool PointLight::create()
{
	if (!Util::checkMemory(shadowMap = new Cubemap()))
		return false;

	return true;
}

PointLight::~PointLight()
{
	delete shadowMap;
}