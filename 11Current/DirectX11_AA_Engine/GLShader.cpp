/* ========================================================================
$File: GLShader.cpp $
$Date: 07-28-16 $
$Revision: 08-6-16 $
$Creator: George Wulfers $
$Notice: (C) Copyright 2016 by WulfersGames, Inc. All Rights Reserved. $
======================================================================== */

#include "GLShader.h"
#include "SysFileHandling.h"

GLShader::GLShader()
{
	isCreated = false;
	
	program = 0;
	vs = 0;
	fs = 0;
	
	pos_loc = 0;
	color_loc = 0;
	uv_loc = 0;

	light0_loc = 0;
	ambient_loc = 0;
	camPso_loc = 0;

	modelView_loc = 0;
	projection_loc = 0;
}

GLShader::~GLShader()
{

}

void GLShader::LoadShaderFromFile(char* filename, int shaderType)
{
	FileResult shaderFile = ReadEntireFile(filename);

	if (shaderFile.data)
	{
		char* shaderSource = (char*)shaderFile.data;
		GLuint shader = glCreateShader(shaderType);
		glShaderSource(shader, 1, &shaderSource, 0);
		glCompileShader(shader);

		GLsizei size = 0;
		GLchar buffer[MAX_PATH_SIZE];
		buffer[0] = 0;

		glGetShaderInfoLog(vs,
			MAX_PATH_SIZE,
			&size,
			buffer);

		if (buffer[0])
		{
			OutputDebugString(filename);
			OutputDebugString("\n");
			OutputDebugString(buffer);
			OutputDebugString("\n");
		}

		FreeMemory(shaderFile.data);
		shaderFile = {0};

		(shaderType == ST_VERTEX) ? vs = shader : fs = shader;
	}
	else
	{
		//ERROR LOG
	}
}

void GLShader::CreateProgram()
{
	program = glCreateProgram();

	isCreated = true;

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);
}

void GLShader::Bind()
{
	if (isCreated)
	{
		glUseProgram(program);
	}
}

void GLShader::Unbind()
{
	glUseProgram(0);
}

void GLShader::Destroy()
{
	if (isCreated)
	{
		glDeleteProgram(program);
		isCreated = false;
	}
}

void GLShader::SetAttribLocation(char* name, int attribType)
{
	if (isCreated)
	{
		if (attribType == AT_POS)
		{
			pos_loc = glGetAttribLocation(program, name);
		}
		else if (attribType == AT_COLOR)
		{
			color_loc = glGetAttribLocation(program, name);
		}
		else if (attribType == AT_UV)
		{
			uv_loc = glGetAttribLocation(program, name);
		}
		else if (attribType == AT_NORMALS)
		{
			normals_loc = glGetAttribLocation(program, name);
		}
	}
}

void GLShader::SetUniformLocation(char* name, int uniformType)
{
	if (isCreated)
	{
		if (uniformType == UT_MODEL_VIEW)
		{
			modelView_loc = glGetUniformLocation(program, name);
		}
		else if (uniformType == UT_PROJECTION)
		{
			projection_loc = glGetUniformLocation(program, name);
		}
		else if (uniformType == UT_LIGHT0)
		{
			light0_loc = glGetUniformLocation(program, name);
		}
		else if (uniformType == UT_AMBIENT)
		{
			ambient_loc = glGetUniformLocation(program, name);
		}
		else if (uniformType == UT_CAMPOS)
		{
			camPso_loc = glGetUniformLocation(program, name);
		}
	}
}

int GLShader::GetAttribLocation(int attribType)
{
	if (isCreated)
	{
		if (attribType == AT_POS)
		{
			return pos_loc;
		}
		else if (attribType == AT_COLOR)
		{
			return color_loc;
		}
		else if (attribType == AT_UV)
		{
			return uv_loc;
		}
		else if (attribType == AT_NORMALS)
		{
			return normals_loc;
		}
	}

	return 0;
}

int GLShader::GetUniformLocation(int uniformType)
{
	if (isCreated)
	{
		if (uniformType == UT_MODEL_VIEW)
		{
			return modelView_loc;
		}
		else if (uniformType == UT_PROJECTION)
		{
			return projection_loc;
		}
		else if (uniformType == UT_LIGHT0)
		{
			return light0_loc;
		}
		else if (uniformType == UT_AMBIENT)
		{
			return ambient_loc;
		}
		else if (uniformType == UT_CAMPOS)
		{
			return camPso_loc;
		}
	}

	return 0;
}