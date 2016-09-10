#ifndef GL_GRAPHICS_ENGINE_H
#define GL_GRAPHICS_ENGINE_H
/* ========================================================================
$File: GLGraphicsEngine.h $
$Date: 07-28-16 $
$Revision: 08-6-16 $
$Creator: George Wulfers $
$Notice: (C) Copyright 2016 by WulfersGames, Inc. All Rights Reserved. $
======================================================================== */

#include "GLVertexFormat.h"
#include "GLShader.h"
#include "SysFileHandling.h"
#include "Transform.h"
#include "Mesh.h"

#include <string>

GLMesh OBJLoader(char* filename);

void DrawMesh(GLMesh* mesh, vmath::mat4 transform, vmath::mat4 projection, vmath::vec3 lightPos = vmath::vec3(0), vmath::vec3 camPos = vmath::vec3(0), vmath::vec3 ambientLight = vmath::vec3(0));
void AttachShader(GLMesh* mesh, GLShader* shader);
void InitMesh(GLMesh* mesh);
void UnloadMesh(GLMesh* mesh);

class GLGraphicsEngine
{
	friend class GLRenderEngine;

public:
	GLGraphicsEngine();
	~GLGraphicsEngine();
	
	void Initialize(HWND hwnd);
	void ReleaseAll();
	void LoadContent();
	void UnloadContent();

private:
	HGLRC m_renderContext;
	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;

	GLShader simpleLightingShader;
	GLShader simpleTextureShader;
	GLShader simpleShader;

	GLMesh cube;
	GLMesh tank;
	GLMesh walker;
};

#endif