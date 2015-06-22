
#include "IRenderer.hpp"
#include "..\Shader\ForwardShader.hpp"

class ForwardRenderer : public IRenderer
{
private:
	ForwardShader *forwardShader;

	bool loadShaders();

public:
	~ForwardRenderer();

	bool init();
	virtual void render(Camera *camera);
};