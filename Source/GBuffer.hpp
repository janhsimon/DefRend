#pragma once

#define GBUFFER_POSITION_TEXTURE_UNIT 0
#define GBUFFER_DIFFUSE_TEXTURE_UNIT  1
#define GBUFFER_SPECULAR_TEXTURE_UNIT  2
#define GBUFFER_NORMAL_TEXTURE_UNIT   3
//#define GBUFFER_TEXCOORD_TEXTURE_UNIT 3

enum GBUFFER_TEXTURE_TYPE
{
	GBUFFER_TEXTURE_TYPE_POSITION,
	GBUFFER_TEXTURE_TYPE_DIFFUSE,
	GBUFFER_TEXTURE_TYPE_SPECULAR,
	GBUFFER_TEXTURE_TYPE_NORMAL
	//GBUFFER_TEXTURE_TYPE_TEXCOORD
};

class GBuffer
{
private:
	GLuint FBO;
	GLuint textures[4];
	GLuint depthTexture;

public:
	~GBuffer();

	bool load(unsigned int windowWidth, unsigned int windowHeight);
	void bindForWriting();
	void bindForReading(bool debug);
	void setReadBuffer(GBUFFER_TEXTURE_TYPE textureType);
};