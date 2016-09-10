#pragma once

#include"Math\AAMath.h"
#include<D3D11.h>

#include "constants.h"
#include "GLcommon.h"
#include "GLShader.h"
#include "GLVertexFormat.h"

#include <vector>

using namespace AAEngine;

// TODO: Find a way to make this work with two different libraries.
// If it is not worth making a single Mesh then we can just make two different ones.
namespace AAEngine
{
	struct Mesh
	{
		int vertexCount;
		ID3D11Buffer* vertexBuffer;
		Mesh();
	};

	// TODO(George): Move this to a more appropriate place
	struct GLMesh
	{
		vmath::vec3* vertices;
		vmath::vec2* uvs;
		vmath::vec3* normals;
		
		u32* indices;
		u32* tIndices;
		u32* nIndices;
		
		i32 vertCount;
		i32 uvCount;
		i32 normCount;
		i32 indexCount;

		GLuint vao;
		GLuint vertexBuffers[VB_SIZE];
		GLuint texture;
		
		GLShader* shader;
	};
}

