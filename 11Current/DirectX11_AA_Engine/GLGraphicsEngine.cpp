/* ========================================================================
$File: GLGraphicsEngine.cpp $
$Date: 07-28-16 $
$Revision: 08-6-16 $
$Creator: George Wulfers $
$Notice: (C) Copyright 2016 by WulfersGames, Inc. All Rights Reserved. $
======================================================================== */

#include "GLGraphicsEngine.h"
#include "constants.h"
#include "BMPFileLoader.h"

void CalcNormals(GLMesh& mesh)
{
	//Resize normal container for calculation of each vert normal
	FreeMemory(mesh.normals);
	vmath::vec3* normals = (vmath::vec3*)AllocMemory(sizeof(vmath::vec3) * mesh.vertCount);
	mesh.normals = normals;

	// used to store all the vertices that have been seen.
	std::vector<u32> seen;
	seen.resize(mesh.vertCount, 0);

	for (unsigned int i = 0; i < mesh.indexCount; i += 3)
	{
		int i0 = mesh.indices[i];
		int i1 = mesh.indices[i + 1];
		int i2 = mesh.indices[i + 2];
		vmath::vec3 normal = vmath::normalize(vmath::cross(
			vmath::vec3(mesh.vertices[i1]) - vmath::vec3(mesh.vertices[i0]),
			vmath::vec3(mesh.vertices[i2]) - vmath::vec3(mesh.vertices[i0])));

		u32 v[3]; v[0] = i0; v[1] = i1; v[2] = i2;

		for (int j = 0; j < 3; j++)
		{
			u32 curV = v[j];
			seen[curV]++;

			if (seen[curV] == 1)
			{
				mesh.normals[curV] = normal;
			}
			else
			{
				mesh.normals[curV][0] = mesh.normals[curV][0] * (1.0f - 1.0f / seen[curV]) +
					normal[0] * 1.0f / seen[curV];
				mesh.normals[curV][1] = mesh.normals[curV][1] * (1.0f - 1.0f / seen[curV]) +
					normal[1] * 1.0f / seen[curV];
				mesh.normals[curV][2] = mesh.normals[curV][2] * (1.0f - 1.0f / seen[curV]) +
					normal[2] * 1.0f / seen[curV];
				mesh.normals[curV] = vmath::normalize(mesh.normals[curV]);
			}
		}
	}
}

// TODO(George): Find a way to extract the object groups into sub meshes
GLMesh OBJLoader(char* filename)
{
	FileResult fileResult = ReadEntireFile(filename);
	
	GLMesh result = {};

	if (fileResult.fileSize != 0)
	{
		char* buffer = (char*)fileResult.data;

		int vertCount = 0;
		int uvCount = 0;
		int normCount = 0;
		int faceCount = 0;
		
		std::vector<vmath::vec3> vertices;
		std::vector<vmath::vec3> normals;
		std::vector<vmath::vec2> uvs;
		std::vector<int> indices;
		std::vector<int> tindices;
		std::vector<int> nindices;

		b32 hasNormals = false;
		b32 hasUvs = false;
		b32 parsingSuccessful = true;

		while (*buffer)
		{
			if (*buffer == 'v' && *(buffer + 1) == ' ')
			{
				vertCount++;
				float x, y, z;
				i32 itemsRead = sscanf(buffer, "v %f %f %f", &x, &y, &z);
				if (itemsRead == 3)
				{
					vertices.push_back(vmath::vec3(x, y, z));
				}
				else
				{
					parsingSuccessful = false;
					break;
				}
			}
			if (*buffer == 'v' && *(buffer + 1) == 'n')
			{
				hasNormals = true;

				normCount++;
				float x, y, z;
				i32 itemsRead = sscanf(buffer, "vn %f %f %f", &x, &y, &z);
				if (itemsRead == 3)
				{
					normals.push_back(vmath::vec3(x, y, z));
				}
				else
				{
					parsingSuccessful = false;
					break;
				}
			}
			if (*buffer == 'v' && *(buffer + 1) == 't')
			{
				hasUvs = true;

				uvCount++;
				float x, y, z;
				i32 itemsRead = sscanf(buffer, "vt %f %f", &x, &y);
				if (itemsRead == 2 || itemsRead == 3)
				{
					uvs.push_back(vmath::vec2(x, y));
				}
				else
				{
					parsingSuccessful = false;
					break;
				}
			}
			else if (*buffer == 'f' && *(buffer + 1) == ' ')
			{
				faceCount++;
				int v1, v2, v3;
				int n1, n2, n3;
				int t1, t2, t3;
				i32 itemsRead = sscanf(buffer, "f %d/%d/%d %d/%d/%d %d/%d/%d", &v1, &t1, &n1, &v2, &t2, &n2, &v3, &t3, &n3);
				if (itemsRead == 9)
				{
					indices.push_back(v1 - 1);
					nindices.push_back(n1 - 1);
					tindices.push_back(t1 - 1);

					indices.push_back(v2 - 1);
					nindices.push_back(n2 - 1);
					tindices.push_back(t2 - 1);

					indices.push_back(v3 - 1);
					nindices.push_back(n3 - 1);
					tindices.push_back(t3 - 1);
				}
				else
				{
					parsingSuccessful = false;
					break;
				}
			}

			buffer++;
		}

		if (parsingSuccessful)
		{
			result.vertCount = vertCount;
			result.vertices = (vmath::vec3*)AllocMemory(sizeof(vmath::vec3) * result.vertCount);

			if (hasUvs)
			{
				result.uvCount = vertCount;
				result.uvs = (vmath::vec2*)AllocMemory(sizeof(vmath::vec2) * result.uvCount);
			}
			if (hasNormals)
			{
				result.normCount = normCount;
				result.normals = (vmath::vec3*)AllocMemory(sizeof(vmath::vec3) * result.normCount);
			}

			result.indexCount = faceCount * 3;
			result.indices = (unsigned int*)AllocMemory(sizeof(u32) * (result.indexCount));
			result.tIndices = (unsigned int*)AllocMemory(sizeof(u32) * (result.indexCount));
			result.nIndices = (unsigned int*)AllocMemory(sizeof(u32) * (result.indexCount));

			for (int i = 0; i < normals.size(); i++)
			{
				result.normals[i] = normals[i];
			}
			normals.clear();

			for (int i = 0; i < vertices.size(); i++)
			{
				result.vertices[i] = vertices[i];
				result.uvs[i] = uvs[tindices[i]];
			}
			vertices.clear();
			uvs.clear();

			for (int i = 0; i < indices.size(); i++)
			{
				result.indices[i] = indices[i];
				result.tIndices[i] = tindices[i];
				result.nIndices[i] = nindices[i];
			}
			indices.clear();
			tindices.clear();
			nindices.clear();

			CalcNormals(result);
		}
		else
		{
			MessageBox(0, "File could not be parsed correctly.", "ERROR: Invalid Format", 0);
		}

		FreeMemory(fileResult.data);
	}

	return result;
}

void DrawMesh(GLMesh* mesh, vmath::mat4 transform, vmath::mat4 projection, vmath::vec3 lightPos, vmath::vec3 camPos, vmath::vec3 ambientLight)
{
	glBindVertexArray(mesh->vao);

	mesh->shader->Bind();

	glUniform3fv(mesh->shader->GetUniformLocation(UT_LIGHT0), 1, lightPos);
	glUniform3fv(mesh->shader->GetUniformLocation(UT_CAMPOS), 1, camPos);
	glUniform3fv(mesh->shader->GetUniformLocation(UT_AMBIENT), 1, ambientLight);
	glUniformMatrix4fv(mesh->shader->GetUniformLocation(UT_PROJECTION), 1, GL_FALSE, projection);
	glUniformMatrix4fv(mesh->shader->GetUniformLocation(UT_MODEL_VIEW), 1, GL_FALSE, transform);
	glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, 0);
	
	mesh->shader->Unbind();

	glBindVertexArray(0);
}

void AttachShader(GLMesh* GLMesh, GLShader* shader)
{
	GLMesh->shader = shader;
}

void InitGLMesh(GLMesh* mesh)
{
	glCreateVertexArrays(1, &mesh->vao);
	glBindVertexArray(mesh->vao);

	glGenBuffers(VB_SIZE, mesh->vertexBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffers[VB_POSITION]);
	glBufferData(GL_ARRAY_BUFFER, mesh->vertCount * sizeof(vmath::vec3), mesh->vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(mesh->shader->GetAttribLocation(AT_POS), 3, GL_FLOAT, GL_FALSE, 0, 0);//sizeof(VERTEX_FORMAT), 0);
	glEnableVertexAttribArray(mesh->shader->GetAttribLocation(AT_POS));

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffers[VB_UV]);
	glBufferData(GL_ARRAY_BUFFER, mesh->uvCount * sizeof(vmath::vec2), mesh->uvs, GL_STATIC_DRAW);

	glVertexAttribPointer(mesh->shader->GetAttribLocation(AT_UV), 2, GL_FLOAT, GL_FALSE, 0, 0);//sizeof(vmath::vec3), 0);
	glEnableVertexAttribArray(mesh->shader->GetAttribLocation(AT_UV));

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffers[VB_NORMALS]);
	glBufferData(GL_ARRAY_BUFFER, mesh->normCount * sizeof(vmath::vec3), mesh->normals, GL_STATIC_DRAW);

	glVertexAttribPointer(mesh->shader->GetAttribLocation(AT_NORMALS), 3, GL_FLOAT, GL_FALSE, 0, 0);//sizeof(vmath::vec3) + sizeof(vmath::vec2), 0);
	glEnableVertexAttribArray(mesh->shader->GetAttribLocation(AT_NORMALS));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->vertexBuffers[VB_INDICES]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indexCount * sizeof(mesh->indices[0]), &mesh->indices[0], GL_STATIC_DRAW);

	BitmapImage image = LoadBMP("Textures/stones.bmp");

	glGenTextures(1, &mesh->texture);
	glBindTexture(GL_TEXTURE_2D, mesh->texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width, image.height, 0, GL_BGR, GL_UNSIGNED_BYTE, image.pixels);

	FreeMemory(image.pixels);
}

void UnloadGLMesh(GLMesh* mesh)
{
	FreeMemory(mesh->vertices);
	FreeMemory(mesh->uvs);
	FreeMemory(mesh->normals);
	FreeMemory(mesh->indices);
	FreeMemory(mesh->tIndices);
	FreeMemory(mesh->nIndices);
	
	glDeleteVertexArrays(1, &mesh->vao);
	glDeleteBuffers(VB_SIZE, mesh->vertexBuffers);

	glDeleteTextures(1, &mesh->texture);
}

GLGraphicsEngine::GLGraphicsEngine()
	: m_renderContext(0)
	, wglCreateContextAttribsARB(0)
{
	
}

GLGraphicsEngine::~GLGraphicsEngine()
{
	
}

void GLGraphicsEngine::Initialize(HWND hwnd)
{
	HDC deviceContext = GetDC(hwnd);

	i32 Attribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 0, 0,
	};
	
	HGLRC temp = wglCreateContext(deviceContext);
	wglMakeCurrent(deviceContext, temp);

	wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

	if (!wglCreateContextAttribsARB)
	{
		MessageBox(hwnd, "ERROR: Your system does not support OpenGL version 3.0!", "ERROR!", MB_OK | MB_ICONERROR);

		m_renderContext = wglCreateContext(deviceContext);
		wglMakeCurrent(deviceContext, m_renderContext);
	}
	else
	{
		m_renderContext = wglCreateContextAttribsARB(deviceContext, 0, Attribs);

		wglDeleteContext(temp);
		wglMakeCurrent(deviceContext, m_renderContext);
	}

	if (glewInit() != GLEW_OK)
	{
		MessageBox(hwnd, "Glew was unable to be initialzed!\n", "ERROR!", MB_OK | MB_ICONERROR);
		return;
	}
	char* glewVersion = (char*)glewGetString(GLEW_VERSION);
	OutputDebugString(glewVersion);
	OutputDebugString("\n");
	OutputDebugString((char*)glGetString(GL_VERSION));

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);

	LoadContent();
}

VERTEX_FORMAT* CreateVertexFormat(u32 size, GLMesh& data)
{
	VERTEX_FORMAT* result = (VERTEX_FORMAT*)AllocMemory(sizeof(VERTEX_FORMAT) * size);

	for (int i = 0; i < data.vertCount; ++i)
	{
		result[i].position = data.vertices[i];
		result[i].uv = data.uvs[data.tIndices[i]];
		result[i].normals = data.normals[data.nIndices[i]];
	}

	return result;
}

void GLGraphicsEngine::ReleaseAll()
{
	//DISABLE OPGL
	wglMakeCurrent(0, 0);
	wglDeleteContext(m_renderContext);
}

void GLGraphicsEngine::LoadContent()
{
	simpleLightingShader.LoadShaderFromFile("Shaders/GLSL/simpleLighting.vs", ST_VERTEX);
	simpleLightingShader.LoadShaderFromFile("Shaders/GLSL/simpleLighting.fs", ST_FRAGMENT);
	simpleLightingShader.CreateProgram();
	simpleLightingShader.SetAttribLocation("pos", AT_POS);
	simpleLightingShader.SetAttribLocation("uv", AT_UV);
	simpleLightingShader.SetAttribLocation("normals", AT_NORMALS);
	simpleLightingShader.SetUniformLocation("modelView", UT_MODEL_VIEW);
	simpleLightingShader.SetUniformLocation("projection", UT_PROJECTION);
	simpleLightingShader.SetUniformLocation("lightPosition", UT_LIGHT0);
	simpleLightingShader.SetUniformLocation("ambientLight", UT_AMBIENT);
	simpleLightingShader.SetUniformLocation("cameraPos", UT_CAMPOS);

	simpleShader.LoadShaderFromFile("Shaders/GLSL/simple.vs", ST_VERTEX);
	simpleShader.LoadShaderFromFile("Shaders/GLSL/simple.fs", ST_FRAGMENT);
	simpleShader.CreateProgram();
	simpleShader.SetAttribLocation("pos", AT_POS);
	simpleShader.SetAttribLocation("uv", AT_UV);
	simpleShader.SetAttribLocation("normals", AT_NORMALS);
	simpleShader.SetUniformLocation("modelView", UT_MODEL_VIEW);
	simpleShader.SetUniformLocation("projection", UT_PROJECTION);

	simpleTextureShader.LoadShaderFromFile("Shaders/GLSL/simpleTexture.vs", ST_VERTEX);
	simpleTextureShader.LoadShaderFromFile("Shaders/GLSL/simpleTexture.fs", ST_FRAGMENT);
	simpleTextureShader.CreateProgram();
	simpleTextureShader.SetAttribLocation("pos", AT_POS);
	simpleTextureShader.SetAttribLocation("uv", AT_UV);
	simpleTextureShader.SetAttribLocation("normals", AT_NORMALS);
	simpleTextureShader.SetUniformLocation("modelView", UT_MODEL_VIEW);
	simpleTextureShader.SetUniformLocation("projection", UT_PROJECTION);

	cube = OBJLoader("Models/Cube.obj");
	AttachShader(&cube, &simpleShader);
	InitGLMesh(&cube);

	tank = OBJLoader("Models/tank_test.obj");
	AttachShader(&tank, &simpleLightingShader);
	InitGLMesh(&tank);

	walker = OBJLoader("Models/walker_test.obj");
	AttachShader(&walker, &simpleLightingShader);
	InitGLMesh(&walker);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void GLGraphicsEngine::UnloadContent()
{
	UnloadGLMesh(&cube);
	UnloadGLMesh(&tank);
	UnloadGLMesh(&walker);

	simpleLightingShader.Destroy();
	simpleShader.Destroy();
	simpleTextureShader.Destroy();
}