#ifndef GL_RENDER_ENGINE_H
#define GL_RENDER_ENGINE_H
/* ========================================================================
$File: GLRenderEngine.h $
$Date: 07-28-16 $
$Revision: 08-6-16 $
$Creator: George Wulfers $
$Notice: (C) Copyright 2016 by WulfersGames, Inc. All Rights Reserved. $
======================================================================== */

#include "GLGraphicsEngine.h"

class GLRenderEngine : public GLGraphicsEngine
{
public:
	GLRenderEngine();
	~GLRenderEngine();

	void Update(float dt);
	void Render(float dt);
};

#endif