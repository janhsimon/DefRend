#include "GBuffer.hpp"
#include "IRenderer.hpp"
#include "..\Model\Model.hpp"
#include "..\Shader\DirectionalLightingShader.hpp"
#include "..\Shader\GeometryShader.hpp"
#include "..\Shader\PointLightingShader.hpp"
#include "..\Shader\ShadowPassShader.hpp"
#include "..\Shader\SpotLightingShader.hpp"

class DeferredRenderer : public IRenderer
{
private:
	
	ShadowPassShader *shadowPassShader;
	GeometryShader *geometryShader;
	DirectionalLightingShader *directionalLightingShader;
	PointLightingShader *pointLightingShader;
	SpotLightingShader *spotLightingShader;

	Model *unitSphereModel;

	bool loadShaders();
	bool loadModels();

	void doShadowPass(PointLight *pointLight);
	void doDirectionalLightPass(Camera *camera);
	void doPointLightPass(Camera *camera);
	void doSpotLightPass(Camera *camera);

public:
	GBuffer *gBuffer;

	~DeferredRenderer();

	bool init();
	virtual void render(Camera *camera);
};