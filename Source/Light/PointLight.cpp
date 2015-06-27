#include "PointLight.hpp"
#include "..\Util\Util.hpp"

bool PointLight::create()
{
	type = LightType::POINT_LIGHT;

	shadowMap = nullptr;
	setCastShadows(false);

	return true;
}

PointLight::~PointLight()
{
	delete shadowMap;
}

void PointLight::setCastShadows(bool castShadows)
{
	this->castShadows = castShadows;

	if (shadowMap)
	{
		delete shadowMap;
		shadowMap = nullptr;
	}

	if (castShadows)
	{
		//if (!Util::checkMemory(shadowMap = new Cubemap()))
			//return false;

		shadowMap = new Cubemap(512);
	}
}