#include "PointLight.hpp"
#include "Util.hpp"

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

void PointLight::beginDrawingToShadowMap()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, shadowMap->getFBO());
}