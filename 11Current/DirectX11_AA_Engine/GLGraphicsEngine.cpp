#include "GLGraphicsEngine.h"
#include "constants.h"
#include "BMPFileLoader.h"

#include <vector>

void CalcNormals(GL::Mesh& mesh)
{
	for (unsigned int i = 0; i < mesh.indexCount; i += 3)
	{
		int i0 = mesh.indices[i];
		int i1 = mesh.indices[i + 1];
		int i2 = mesh.indices[i + 2];

		vmath::vec3 v1 = mesh.vertices[i1] - mesh.vertices[i0];
		vmath::vec3 v2 = mesh.vertices[i2] - mesh.vertices[i0];

		vmath::vec3 normal = vmath::normalize(vmath::cross(v1, v2));

		mesh.normals[i0] += normal;
		mesh.normals[i1] += normal;
		mesh.normals[i2] += normal;
	}

	for (unsigned int i = 0; i < mesh.vertCount; i++)
		mesh.normals[i] = vmath::normalize(mesh.normals[i]);
}

GL::Mesh OBJLoader(char* filename)
{
	FileResult fileResult = ReadEntireFile(filename);
	
	GL::Mesh result = {};

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

		while (*buffer)
		{
			if (*buffer == 'v' && *(buffer + 1) == ' ')
			{
				vertCount++;
				float x, y, z;
				sscanf(buffer, "v %f %f %f", &x, &y, &z);
				vertices.push_back(vmath::vec3(x, y, z));
			}
			if (*buffer == 'v' && *(buffer + 1) == 'n')
			{
				normCount++;
				float x, y, z;
				sscanf(buffer, "vn %f %f %f", &x, &y, &z);
				normals.push_back(vmath::vec3(x, y, z));
			}
			if (*buffer == 'v' && *(buffer + 1) == 't')
			{
				uvCount++;
				float x, y, z;
				sscanf(buffer, "vt %f %f", &x, &y);
				uvs.push_back(vmath::vec2(x, y));
			}
			else if (*buffer == 'f' && *(buffer + 1) == ' ')
			{
				faceCount++;
				int v1, v2, v3;
				int n1, n2, n3;
				int t1, t2, t3;
				sscanf(buffer, "f %d/%d/%d %d/%d/%d %d/%d/%d", &v1, &n1, &t1, &v2, &n2, &t2, &v3, &n3, &t3);
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

			buffer++;
		}

		result.vertices = (vmath::vec3*)AllocMemory(sizeof(vmath::vec3) * vertCount);
		result.vertCount = vertCount;

		result.uvs = (vmath::vec2*)AllocMemory(sizeof(vmath::vec2) * uvCount);
		result.uvCount = uvCount;

		result.normals = (vmath::vec3*)AllocMemory(sizeof(vmath::vec3) * normCount);
		result.normCount = normCount;

		result.indices = (unsigned int*)AllocMemory(sizeof(u32) * (faceCount * 3));
		result.indexCount = faceCount * 3;
		
		OutputDebugString("\n");
		for (int i = 0; i < vertices.size(); i++)
		{
			result.vertices[i] = vertices[i];
			char buffer[256];
			sprintf(buffer, "Vertex: %d (%.03f, %.03f, %.03f)\n", i, result.vertices[0], result.vertices[1], result.vertices[2]);
			OutputDebugString(buffer);
		}
		vertices.clear();
		
		OutputDebugString("\n");
		for (int i = 0; i < normals.size(); i++)
		{
			result.normals[i] = normals[i];
			char buffer[256];
			sprintf(buffer, "Normals: %d (%.03f, %.03f, %.03f)\n", i, result.normals[0], result.normals[1], result.normals[2]);
			OutputDebugString(buffer);
		}
		normals.clear();

		for (int i = 0; i < uvCount; i++)
		{
			result.uvs[i] = uvs[i];
			char buffer[256];
			sprintf(buffer, "UVs: %d (%.03f, %.03f)\n", i, result.uvs[0], result.uvs[1]);
			OutputDebugString(buffer);
		}

		OutputDebugString("\n");
		for (int i = 0; i < indices.size(); i++)
		{
			result.indices[i] = indices[i];
			char buffer[256];
			sprintf(buffer, "Indices: %d %d\n", i, result.indices[i]);
			OutputDebugString(buffer);
		}
		indices.clear();
		
		CalcNormals(result);

		FreeMemory(fileResult.data);
	}

	return result;
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

	OutputDebugString((char*)glGetString(GL_VERSION));

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);

	LoadContent();
}

void GLGraphicsEngine::ReleaseAll()
{
	//DISABLE OPGL
	wglMakeCurrent(0, 0);
	wglDeleteContext(m_renderContext);
}

void GLGraphicsEngine::LoadContent()
{
	cube = OBJLoader("Models/tank_body_test.obj");
	
	shader.LoadShaderFromFile("Shaders/GLSL/simple.vs", ST_VERTEX);
	shader.LoadShaderFromFile("Shaders/GLSL/simple.fs", ST_FRAGMENT);
	
	shader.CreateProgram();

	shader.SetAttribLocation("pos", AT_POS);
	shader.SetAttribLocation("uv", AT_UV);
	shader.SetAttribLocation("normals", AT_NORMALS);

	shader.SetUniformLocation("modelView", UT_MODEL_VIEW);
	shader.SetUniformLocation("projection", UT_PROJECTION);

	glCreateVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(VB_SIZE, vertexBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[VB_POSITION]);
	glBufferData(GL_ARRAY_BUFFER, cube.vertCount * sizeof(vmath::vec3), cube.vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(shader.GetAttribLocation(AT_POS), 3, GL_FLOAT, GL_FALSE, 0, 0);//sizeof(VERTEX_FORMAT), 0);
	glEnableVertexAttribArray(shader.GetAttribLocation(AT_POS));

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[VB_UV]);
	glBufferData(GL_ARRAY_BUFFER, cube.uvCount * sizeof(vmath::vec2), cube.uvs, GL_STATIC_DRAW);

	glVertexAttribPointer(shader.GetAttribLocation(AT_UV), 2, GL_FLOAT, GL_FALSE, 0, 0);//sizeof(vmath::vec3), 0);
	glEnableVertexAttribArray(shader.GetAttribLocation(AT_UV));

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[VB_NORMALS]);
	glBufferData(GL_ARRAY_BUFFER, cube.normCount * sizeof(vmath::vec3), cube.normals, GL_STATIC_DRAW);

	glVertexAttribPointer(shader.GetAttribLocation(AT_NORMALS), 3, GL_FLOAT, GL_FALSE, 0, 0);//sizeof(vmath::vec3) + sizeof(vmath::vec2), 0);
	glEnableVertexAttribArray(shader.GetAttribLocation(AT_NORMALS));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers[VB_INDICES]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, cube.indexCount * sizeof(cube.indices[0]), &cube.indices[0], GL_STATIC_DRAW);

	BitmapImage image = LoadBMP("Textures/stones.bmp");

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width, image.height, 0, GL_BGR, GL_UNSIGNED_BYTE, image.pixels);

	FreeMemory(image.pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void GLGraphicsEngine::UnloadContent()
{
	FreeMemory(cube.vertices);
	FreeMemory(cube.uvs);
	FreeMemory(cube.normals);
	FreeMemory(cube.indices);

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(VB_SIZE, vertexBuffers);

	glDeleteTextures(1, &texture);

	shader.Destroy();
}