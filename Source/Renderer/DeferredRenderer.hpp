#include "GBuffer.hpp"
#include "IRenderer.hpp"
#include "..\Model\Model.hpp"
#include "..\Shader\DirectionalLightingShader.hpp"
#include "..\Shader\GeometryShader.hpp"
#include "..\Shader\PointLightingShader.hpp"
#include "..\Shader\SpotLightingShader.hpp"

class DeferredRenderer : public IRenderer
{
private:
	GBuffer *gBuffer;
	
	GeometryShader *geometryShader;
	DirectionalLightingShader *directionalLightingShader;
	PointLightingShader *pointLightingShader;
	SpotLightingShader *spotLightingShader;

	Model *fullscreenQuadModel, *unitSphereModel;

	bool loadShaders();
	bool loadModels();

	void renderGeometryPass(Camera *camera);
	void renderGBufferDebug();
	void beginLightPass();
	void doDirectionalLightPass(Camera *camera);
	void doPointLightPass(Camera *camera);
	void doSpotLightPass(Camera *camera);

public:
	~DeferredRenderer();

	bool init(Window *window);
	virtual void render(Camera *camera);
};