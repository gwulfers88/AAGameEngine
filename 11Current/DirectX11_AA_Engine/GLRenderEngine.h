#ifndef GL_RENDER_ENGINE_H
#define GL_RENDER_ENGINE_H

#include "GLGraphicsEngine.h"
#include "Transform.h"

class GLRenderEngine : public GLGraphicsEngine
{
public:
	GLRenderEngine();
	~GLRenderEngine();

	void Update(float dt);
	void Render(float dt);
};

#endif