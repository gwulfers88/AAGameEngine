#ifndef GL_GRAPHICS_ENGINE_H
#define GL_GRAPHICS_ENGINE_H

#include "GLVertexFormat.h"
#include "GLShader.h"
#include "SysFileHandling.h"
#include <string>
#include <algorithm>
#include <fstream>

namespace GL
{
	struct Mesh
	{
		vmath::vec3* vertices;
		vmath::vec2* uvs;
		vmath::vec3* normals;
		unsigned int* indices;
		int vertCount;
		int uvCount;
		int normCount;
		int indexCount;
	};
}

GL::Mesh OBJLoader(char* filename);

#define RED		vmath::vec3(1.0f, 0.0f, 0.0f)
#define GREEN	vmath::vec3(0.0f, 1.0f, 0.0f)
#define BLUE	vmath::vec3(0.0f, 0.0f, 1.0f)
#define YELLOW	vmath::vec3(0.5f, 0.5f, 0.0f)
#define CYAN	vmath::vec3(0.0f, 0.5f, 0.5f)
#define PURPLE	vmath::vec3(0.5f, 0.0f, 0.5f)

#define ArrayCount(a) (sizeof(a) / sizeof(a[0]))

static VERTEX_FORMAT vertices[] =
{
	vmath::vec3(-0.5f, -0.5f, 0.0f), vmath::vec2(1.0f, 0.0f), vmath::vec3(0.0f, 0.0f, -1.0f),
	vmath::vec3(-0.5f, 0.5f, 0.0f), vmath::vec2(1.0f, 1.0f), vmath::vec3(0.0f, 0.0f, 1.0f),
	vmath::vec3(0.5f, 0.5f, 0.0f), vmath::vec2(0.0f, 1.0f), vmath::vec3(0.0f, -1.0f, 0.0f),
	vmath::vec3(0.5f, -0.5f, 0.0), vmath::vec2(0.0f, 0.0f), vmath::vec3(1.0f, 0.0f, 0.0f),

	vmath::vec3(-0.5f, -0.5f, 1.0f), vmath::vec2(1.0f, 0.0f), vmath::vec3(0.0f, 1.0f, 0.0f),
	vmath::vec3(0.5f, -0.5f, 1.0f), vmath::vec2(1.0f, 1.0f), vmath::vec3(-1.0f, 0.0f, 0.0f),
	vmath::vec3(0.5f, 0.5f, 1.0f), vmath::vec2(0.0f, 1.0f), vmath::vec3(0.0f, 0.0f, 0.0f),
	vmath::vec3(-0.5f, 0.5f, 1.0f), vmath::vec2(0.0f, 0.0f), vmath::vec3(0.0f, 0.0f, 0.0f),
};

static unsigned int indices[] =
{
	0, 1, 2,
	2, 3, 0,
	4, 5, 6,
	6, 7, 4,
	0, 3, 5,
	5, 4, 0,
	3, 2, 6,
	6, 5, 3,
	2, 1, 7,
	7, 6, 2,
	1, 0, 4,
	4, 7, 1,
};

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

	//TEMPORARY PLACE FOR NOW
	GLuint vao;
	GLuint vertexBuffers[VB_SIZE];

	GLuint texture;

	GLShader shader;
	GL::Mesh cube;
};

#endif