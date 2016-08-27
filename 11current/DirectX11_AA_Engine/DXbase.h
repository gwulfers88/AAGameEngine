#ifndef _DXBASE_H_
#define _DXBASE_H_

#ifdef _DEBUG
#define D3d_DEBUG_INFO
#endif

#include <d3d11.h>
#include <D3DX11.h>
#include <xnamath.h>
#include <DxErr.h>


class DXbase 
{
public:
	//variables
	HRESULT m_hresult;
	HWND    m_hwnd;
	HINSTANCE m_hInstance;

	D3D_DRIVER_TYPE     m_driverType;
	D3D_FEATURE_LEVEL   m_featureLevel;

	ID3D11Device*           m_pD3dDevice;
	ID3D11DeviceContext*    m_pD3dContext;
	//
	/*		Microsoft DirectX Graphics Infrastructure (DXGI) recognizes that some parts of graphics evolve more slowly than others. 
	  The primary goal of DXGI is to manage low-level tasks that can be independent of the DirectX graphics runtime. DXGI provides a
	  common framework for future graphics components; the first component that takes advantage of DXGI is Microsoft Direct3D 10.*/
	//
	IDXGISwapChain*         m_pSwapChain;
	
	ID3D11RenderTargetView* m_pBackBufferTarget;
	ID3D11Texture2D*        m_pBackBufferTexture;

	ID3D11Texture2D*        depthBufferTexture;
	ID3D11DepthStencilView* depthBuffer;

	unsigned int width;
	unsigned int height;


public:
	DXbase();
	virtual ~DXbase();
	void ReleaseAll();

	bool Initialize(HINSTANCE hinstance, HWND hw);
	void DXbase::DxBaseClearRenderTargetView(ID3D11RenderTargetView* m_pRenderTargetView, const FLOAT ColorRGBA[4]);
	
	HRESULT showBackBuffer();
	HRESULT getDeviceState();
	HRESULT reset();

	bool CreateDeviceAndSwapChain(HWND& hw);
	bool SetUpRenderTargetView();
	void SetViewportAndDepthBuffer();

	static bool CompileD3DShader(char* filePath, char* entry,char* shaderModel, ID3DBlob** buffer);
	virtual bool LoadContent();
	virtual void UnloadContent();

	virtual void Update(float dt) = 0;
	virtual void Render(float dt) = 0;


};

#endif
