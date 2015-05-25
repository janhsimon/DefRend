#pragma once

#include <SDL_ttf.h>
#include <string>

#include "UnitArrow.hpp"
#include "UnitQuad.hpp"
#include "..\Camera\Camera.hpp"
#include "..\Light\DirectionalLight.hpp"
#include "..\Material\Texture.hpp"
#include "..\Shader\BillboardDrawShader.hpp"
#include "..\Shader\LineDrawShader.hpp"

class BillboardRenderer
{
private:
	static const std::string DIRECTIONAL_LIGHT_BILLBOARD_FILENAME;
	static const std::string POINT_LIGHT_BILLBOARD_FILENAME;
	static const std::string SPOT_LIGHT_BILLBOARD_FILENAME;

	BillboardDrawShader *billboardDrawShader;
	LineDrawShader *lineDrawShader;
	Texture *directionalLightBillboard, *pointLightBillboard, *spotLightBillboard;
	UnitArrow *unitArrow;
	UnitQuad *unitQuad;

	glm::mat4 calculateBillboardMatrix(Camera *camera, DirectionalLight *light);

public:
	bool create();
	~BillboardRenderer();

	void render(Camera *camera);
};