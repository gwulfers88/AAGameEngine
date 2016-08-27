#include"RenderEngine.h"
#include"constants.h"
#include<xnamath.h>
#include<fstream>
#include<array>
#include"RendererManager.h"
#include"Math\AAMath.h"
#include"MeshManager.h"
#include"Shader.h"

RenderEngine* RenderEngine::instance = nullptr;

namespace AAEngine {
	namespace {
		struct VertexPos
		{
			XMFLOAT3 pos;
			XMFLOAT2 tex0;
		};

		struct COLORMOD
		{
			float RedLevel, BlueLevel;
		};
	}

	RenderEngine::RenderEngine()
	{
		Time = 0.0f;
	}

	RenderEngine::~RenderEngine() {
	}

	bool RenderEngine::MakeSingleTon() {
		if (instance == nullptr) {
			instance = new RenderEngine();
		}
		return true;
	}

	void RenderEngine::Update(float dt) {

	}

	void RenderEngine::Render(float dt)
	{
		CleanScreen();

		    std::map<std::string, std::vector<Renderer*> >::iterator it = RendererManager::instance->RendererTypeByMeshType.begin();
		

			ID3D11Buffer* vertexBuffer = MeshManager::instance->GetAMesh(it->first)->vertexBuffer; //get instance buffer
			
			Shader* theShader= it->second[0]->material->shader;
			

			UINT stride = sizeof(VERTEX);
			UINT offset = 0;
			 
			//Set up all Render PipleLine then Render.......
			m_pD3dContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
			m_pD3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			m_pD3dContext->IASetInputLayout(theShader->inputLayout);
			m_pD3dContext->VSSetShader(theShader->vs_Shader, 0, 0);
			m_pD3dContext->PSSetShader(theShader->ps_Shader, 0, 0);
			m_pD3dContext->VSSetConstantBuffers(0, 1, &theShader->constantBuffer);

			Time += 0.01f;
			// calculate the world matrices
			XMMATRIX matRotate[4];
			matRotate[0] = XMMatrixRotationY(Time);
			matRotate[1] = XMMatrixRotationY(Time + 3.14159f);
			matRotate[2] = XMMatrixRotationY(Time);
			matRotate[3] = XMMatrixRotationY(Time + 3.14159f);
			XMMATRIX matTranslate[4];
			matTranslate[0] = XMMatrixTranslation(0.0f, 0.0f, 0.5f);
			matTranslate[1] = XMMatrixTranslation(0.0f, 0.0f, 0.5f);
			matTranslate[2] = XMMatrixTranslation(0.0f, 0.0f, -0.5f);
			matTranslate[3] = XMMatrixTranslation(0.0f, 0.0f, -0.5f);

			float moveCam = Time * 0.1;

			XMVECTOR vecCamPosition = XMVectorSet(1.5f, 0.5f , 1.5f + moveCam, 0);
			XMVECTOR vecCamLookAt = XMVectorSet(0, 0, 0, 0);
			XMVECTOR vecCamUp = XMVectorSet(0, 1, 0, 0);

			XMMATRIX matView = XMMatrixLookAtLH(vecCamPosition, vecCamLookAt, vecCamUp);
			

			XMMATRIX matProjection = XMMatrixPerspectiveFovLH(
				XMConvertToRadians(45),                                      // the field of view
				(FLOAT)GAME_WIDTH / (FLOAT)GAME_HEIGHT,                      // aspect ratio
				1,                                                           // the near view-plane
				100);                                                        // the far view-plane
																			 // calculate the final matrix
																			 // calculate the final matrix
			XMMATRIX matFinal[4];
			matFinal[0] = matTranslate[0] * matRotate[0] * matView * matProjection;
			matFinal[1] = matTranslate[1] * matRotate[1] * matView * matProjection;
			matFinal[2] = matTranslate[2] * matRotate[2] * matView * matProjection;
			matFinal[3] = matTranslate[3] * matRotate[3] * matView * matProjection;

			
			m_pD3dContext->UpdateSubresource(theShader->constantBuffer, 0, 0, &matFinal[0], 0, 0);
			m_pD3dContext->Draw(3, 0);
			m_pD3dContext->UpdateSubresource(theShader->constantBuffer, 0, 0, &matFinal[1], 0, 0);
			m_pD3dContext->Draw(3, 0);
			m_pD3dContext->UpdateSubresource(theShader->constantBuffer, 0, 0, &matFinal[2], 0, 0);
			m_pD3dContext->Draw(3, 0);
			m_pD3dContext->UpdateSubresource(theShader->constantBuffer, 0, 0, &matFinal[3], 0, 0);
			m_pD3dContext->Draw(3, 0);

			m_pSwapChain->Present(1, 0);
	}

	void RenderEngine::CleanScreen() {
		/*Bind one or more render targets atomically and the depth-stencil buffer to the output-merger stage.*/
		// If we use multiple swapChain, then we need to Set the RenderTargets ever frame...
		
		float clearColor[4] = { 0.0f, 0.25f, 0.5f, 1.0f };
		m_pD3dContext->ClearRenderTargetView(m_pBackBufferTarget, clearColor);
		m_pD3dContext->ClearDepthStencilView(depthBuffer, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	bool RenderEngine::DrawString(char* _message, float _startX, float _startY, ID3D11Buffer* vertexBuffer_) {
		const int maxLetters = 24;
		int length = strlen(_message);

		if (length > maxLetters) {
			length = maxLetters;
		}

		float charWidth = 32.0f / 800.0f;//Char's width on screem.
		float charHeight = 32.0f / 640.0f;
		float texelWidth = 32.0f / 864.0f; //32 * 24 = 864
		const int verticesPerLetter = 6;
		D3D11_MAPPED_SUBRESOURCE mapResource;
		//Detail at p167
		HRESULT d3dResult = m_pD3dContext->Map(vertexBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource);

		if (FAILED(d3dResult)) {
			DXTRACE_MSG("Faild to map resource!");
			return false;
		}
		VertexPos* spritePtr = (VertexPos*)mapResource.pData;
		const int indexA = static_cast<char>('A');
		const int indexZ = static_cast<char>('Z');

		for (int i = 0; i < length; ++i) {
			float thisStartX = _startX + (charWidth * static_cast<float>(i));
			float thisEndX = thisStartX + charWidth;
			float thisEndY = _startY + charHeight;

			spritePtr[0].pos = XMFLOAT3(thisEndX, thisEndY, 1.0f);
			spritePtr[1].pos = XMFLOAT3(thisEndX, _startY, 1.0f);
			spritePtr[2].pos = XMFLOAT3(thisStartX, _startY, 1.0f);
			spritePtr[3].pos = XMFLOAT3(thisStartX, _startY, 1.0f);
			spritePtr[4].pos = XMFLOAT3(thisStartX, thisEndY, 1.0f);
			spritePtr[5].pos = XMFLOAT3(thisEndX, thisEndY, 1.0f);

			int texLookup = 0;
			int letter = static_cast<char>(_message[i]);

			if (letter < indexA || letter > indexZ) {
				texLookup = (indexZ - indexA) + 1;// Grab one index past Z, which is a blank space in the texture.
			}
			else {
				// A = 0, B = 1, Z = 25, etc.
				texLookup = (letter - indexA);
			}

			float tuStart = 0.0f + (texelWidth * static_cast<float>(texLookup));
			float tuEnd = tuStart + texelWidth;

			spritePtr[0].tex0 = XMFLOAT2(tuEnd, 0.0f);
			spritePtr[1].tex0 = XMFLOAT2(tuEnd, 1.0f);
			spritePtr[2].tex0 = XMFLOAT2(tuStart, 1.0f);
			spritePtr[3].tex0 = XMFLOAT2(tuStart, 1.0f);
			spritePtr[4].tex0 = XMFLOAT2(tuStart, 0.0f);
			spritePtr[5].tex0 = XMFLOAT2(tuEnd, 0.0f);

			spritePtr += 6;
		}

		m_pD3dContext->Unmap(vertexBuffer_, 0);
		m_pD3dContext->Draw(6 * length, 0);

		return true;
	}
} 