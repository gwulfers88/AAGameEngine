/* ========================================================================
$File: GLRenderEngine.cpp $
$Date: 07-28-16 $
$Revision: 08-6-16 $
$Creator: George Wulfers $
$Notice: (C) Copyright 2016 by WulfersGames, Inc. All Rights Reserved. $
======================================================================== */

#include "GLRenderEngine.h"
#include "constants.h"
#include "GLCamera.h"

// TODO(George): Add these to mesh class
Transform cubeTransform;
Transform tankTransform;
Transform walkerTransform;

GLCamera* camera = 0;

GLRenderEngine::GLRenderEngine() : GLGraphicsEngine()
{
	cubeTransform.SetPosition(vmath::vec3(0.0f, 0.0f, -0.0f));
	cubeTransform.SetRotation(vmath::vec3(-0.0f, 0.0f, 0.0f));

	tankTransform.SetPosition(vmath::vec3(-50.0f, -5.0f, -0.0f));
	tankTransform.SetRotation(vmath::vec3(-90.0f, 135.0f, 0.0f));
	
	walkerTransform.SetPosition(vmath::vec3(50.0f, -0.0f, 400.0f));
	walkerTransform.SetRotation(vmath::vec3(-00.0f, 225.0f, 0.0f));

	camera = new GLCamera(vmath::vec3(0, 0, -100.0f), 50.0f, (float)config->windowWidth / (float)config->windowHeight, 0.1f, 1000.0f);
}

GLRenderEngine::~GLRenderEngine()
{
	delete camera;
}

void GLRenderEngine::Update(float dt)
{
	float aspect = (float)config->windowWidth / (float)config->windowHeight;
	camera->UpdateAspectRatio(aspect);

	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		cubeTransform.GetRotation()[0] -= 5.0f * 0.16f;
	}
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		cubeTransform.GetRotation()[0] += 5.0f * 0.16f;
	}

	if (GetAsyncKeyState('Q') & 0x8000)
	{
		cubeTransform.GetPosition()[1] -= 5.0f * 0.16f;
	}
	else if (GetAsyncKeyState('E') & 0x8000)
	{
		cubeTransform.GetPosition()[1] += 5.0f * 0.16f;
	}

	if (GetAsyncKeyState('W') & 0x8000)
	{
		cubeTransform.GetPosition()[2] += 5.0f * 0.16f;
	}
	else if (GetAsyncKeyState('S') & 0x8000)
	{
		cubeTransform.GetPosition()[2] -= 5.0f * 0.16f;
	}

	if (GetAsyncKeyState('A') & 0x8000)
	{
		cubeTransform.GetRotation()[1] += 5.0f * 0.16f;
	}
	else if (GetAsyncKeyState('D') & 0x8000)
	{
		cubeTransform.GetRotation()[1] -= 5.0f * 0.16f;
	}
}

void GLRenderEngine::Render(float dt)
{
	glViewport(0, 0, config->windowWidth, config->windowHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);

	DrawMesh(&cube, cubeTransform.ToMatrix(), camera->ToMatrix());

	//tankTransform.GetRotation()[1] += 0.16f * 5.0f;
	DrawMesh(&tank, tankTransform.ToMatrix(), camera->ToMatrix(), cubeTransform.GetPosition(), camera->GetPosition(), vmath::vec3(0.1f));

	//walkerTransform.GetRotation()[1] -= 0.16f * 5.0f;
	DrawMesh(&walker, walkerTransform.ToMatrix(), camera->ToMatrix(), cubeTransform.GetPosition(), camera->GetPosition(), vmath::vec3(0.1f));
}