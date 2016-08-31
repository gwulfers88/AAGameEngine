#ifndef GL_SHADER_H
#define GL_SHADER_H

#include "GLcommon.h"

enum SHADER_TYPE
{
	ST_VERTEX = GL_VERTEX_SHADER,
	ST_FRAGMENT = GL_FRAGMENT_SHADER,
};

enum ATTRIB_TYPE
{
	AT_POS,
	AT_COLOR,
	AT_UV,
	AT_NORMALS,
};

enum UNIFORM_TYPE
{
	UT_MODEL_VIEW,
	UT_PROJECTION,
};

class GLShader
{
public:
	GLShader();
	~GLShader();

	void LoadShaderFromFile(char* filename, int shaderType);
	void CreateProgram();
	void Bind();
	void Unbind();
	void Destroy();

	void SetAttribLocation(char* name, int attribType);
	void SetUniformLocation(char* name, int uniformType);
	
	int GetAttribLocation(int attribType);
	int GetUniformLocation(int uniformType);
	
private:
	bool isCreated;
	
	GLuint program;
	GLuint vs;
	GLuint fs;
	
	GLuint pos_loc;
	GLuint color_loc;
	GLuint uv_loc;
	GLuint normals_loc;

	GLuint modelView_loc;
	GLuint projection_loc;

};

#endif