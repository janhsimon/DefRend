#include "PointLight.hpp"

PointLight::PointLight()
{
	shadowMap = new Cubemap();
}

PointLight::~PointLight()
{
	delete shadowMap;
}

void PointLight::beginDrawingToShadowMap()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, shadowMap->getFBO());
}