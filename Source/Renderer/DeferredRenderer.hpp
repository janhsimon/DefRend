#include "IRenderer.hpp"
#include "..\GBuffer\IGBuffer.hpp"
#include "..\Light\PointLight.hpp"
#include "..\Model\Model.hpp"
#include "..\Shader\DeferredFatShader.hpp"
#include "..\Shader\DeferredSlimShader.hpp"
#include "..\Shader\DeferredSuperSlimShader.hpp"
#include "..\Shader\GeometryFatShader.hpp"
#include "..\Shader\GeometrySlimShader.hpp"
#include "..\Shader\GeometrySuperSlimShader.hpp"
#include "..\Shader\ShadowPassShader.hpp"

class DeferredRenderer : public IRenderer
{
private:
	
	ShadowPassShader *shadowPassShader;

	GeometryFatShader *geometryFatShader;
	DeferredFatShader *deferredFatShader;

	GeometrySlimShader *geometrySlimShader;
	DeferredSlimShader *deferredSlimShader;

	GeometrySuperSlimShader *geometrySuperSlimShader;
	DeferredSuperSlimShader *deferredSuperSlimShader;

	Model *unitSphereModel;

	bool loadFatShaders();
	bool loadSlimShaders();
	bool loadSuperSlimShaders();

	bool loadModels();

	void doShadowPass(PointLight *pointLight);

public:
	IGBuffer *gBuffer;

	~DeferredRenderer();

	bool init();
	virtual void render(Camera *camera);

	void changeGBufferLayout(GBufferType type);
};