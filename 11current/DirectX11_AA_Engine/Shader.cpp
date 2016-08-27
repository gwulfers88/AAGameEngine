#include "Shader.h"
#include"RenderEngine.h"

namespace AAEngine {

	Shader::Shader()
	{
		vs_Shader = nullptr;
		ps_Shader = nullptr;
		fileName = "WorkingWithConstBuffer.txt";
		CompileVertexShader();
		CompilePixelShader();
		CreateConstantBuffer();
		
	}

	Shader::~Shader()
	{
	}

	void Shader::CreateConstantBuffer() { 
		// this can be part of the InitPipeline() function
		D3D11_BUFFER_DESC bd = { 0 };

		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = 64;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		RenderEngine::instance->m_pD3dDevice->CreateBuffer(&bd, nullptr, &constantBuffer);
	}

	void Shader::CompileVertexShader() {
		ID3DBlob* vsBuffer = 0;//This interface is used to return data of arbitrary length.
							   //More detail at P101
		bool compileResult = RenderEngine::instance->CompileD3DShader(fileName, "VS_Main", "vs_4_0", &vsBuffer);

		if (compileResult == false) {
			MessageBox(0, "Error loading vertex shader!", "Compile Error", MB_OK);
		}
		//P103
		HRESULT d3dResult = RenderEngine::instance->m_pD3dDevice->CreateVertexShader(vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), 0, &vs_Shader);

		if (FAILED(d3dResult)) {
			if (vsBuffer)
				vsBuffer->Release();
		}
		//InputLayout
		//
		D3D11_INPUT_ELEMENT_DESC solidColorLayout[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		unsigned int totalLayoutElements = ARRAYSIZE(solidColorLayout);

		d3dResult = RenderEngine::instance->m_pD3dDevice->CreateInputLayout(solidColorLayout, totalLayoutElements, vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), &inputLayout);
		vsBuffer->Release();

	}

	void Shader::CompilePixelShader() {
		ID3DBlob* psBuffer = 0;

		bool compileResult = RenderEngine::instance->CompileD3DShader(fileName, "PS_Main", "ps_4_0", &psBuffer);

		if (compileResult == false) {
			MessageBox(0, "Error loading pixel shader!", "Compile Error", MB_OK);
		}

		HRESULT d3dResult = RenderEngine::instance->m_pD3dDevice->CreatePixelShader(psBuffer->GetBufferPointer(), psBuffer->GetBufferSize(), 0, &ps_Shader);
		psBuffer->Release();

		//RenderEngine::instance->m_pD3dContext->PSSetShader(ps_Shader, 0, 0);
	}
}
