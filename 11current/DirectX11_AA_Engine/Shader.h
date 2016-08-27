#pragma once
#include<string>
#include<D3D11.h>

namespace AAEngine {

	class Shader
	{
	public:
		Shader();
		~Shader();

		//variable
		unsigned int renderQueue;

		char* fileName;

		ID3D11VertexShader* vs_Shader;
		ID3D11PixelShader* ps_Shader;
		ID3D11InputLayout* inputLayout;
		ID3D11Buffer*      constantBuffer;

		void CompileVertexShader();
		void CompilePixelShader();
		void CreateConstantBuffer();

	};
}

