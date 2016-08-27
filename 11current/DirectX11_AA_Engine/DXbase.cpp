#include "DXbase.h"
#include<D3Dcompiler.h>
#include<iostream>
#include"DXbaseConst.h"
#include"constants.h"
//Most commonly, DXGI formats are used to describe the layout of the buffers in the swap chain.
// 8 bits = 1byte;  bits used to describe the rate of deliver. byte use to describe the size of a file
DXbase::DXbase()
{
	m_driverType = D3D_DRIVER_TYPE_NULL;
	m_featureLevel = D3D_FEATURE_LEVEL_11_0;
	m_pD3dDevice = nullptr;
	m_pD3dContext = nullptr;
	m_pSwapChain = nullptr;
	m_pBackBufferTarget = nullptr;
	height = 0;
	width = 0;
}

DXbase::~DXbase()
{
	ReleaseAll();
}

void DXbase::ReleaseAll()
{
	UnloadContent();

	if (m_pBackBufferTarget)
		m_pBackBufferTarget->Release();
	if (m_pSwapChain)
		m_pSwapChain->Release();
	if (m_pD3dContext)
		m_pD3dContext->Release();

	m_pBackBufferTarget = nullptr;
	m_pSwapChain = nullptr;
	m_pD3dContext = nullptr;
	m_pD3dDevice = nullptr;
}

bool DXbase::CreateDeviceAndSwapChain(HWND& hw) {
	//
	/*		The front buffer have the data which current displayer shows. We draw every thing on back buffer
		and then swap it ti the front buffer.*/
	//
	DXGI_SWAP_CHAIN_DESC swapChainDesc;//this is not a pointer.
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));			  // ZeroMemory(* Destination,int Length)
	swapChainDesc.BufferCount = 2;								  // a front buffer and a back buffer
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;          // specifying a 60 Hertz refresh rate
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;  // how the swap chain should be used
	swapChainDesc.OutputWindow = hw;
	swapChainDesc.Windowed = true;                                // that is used to specify whether Direct3D should stay windowed or resize into full - screen mode.
	swapChainDesc.SampleDesc.Count = 1;                           // disable anti-aliasing
	swapChainDesc.SampleDesc.Quality = 0;

	unsigned int creationFlags = 0;

#ifdef _DEBUG 
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG; //C |= 2 is same as C = C | 2
#endif

	HRESULT result;
	unsigned int driver = 0;

	for (driver = 0; driver < totalDriverTypes; ++driver)
	{
		result = D3D11CreateDeviceAndSwapChain(nullptr,//A pointer to the video adapter to use to create the device.Passing null causes Direct3D to use the default device.This can be useful if there is more than one device installed in the machine.
			driverTypes[driver], // change to D3D_DRIVER_TYPE_WARP.....
			nullptr,
			0,
			featureLevels,
			creationFlags, //change to 1
			D3D11_SDK_VERSION,
			&swapChainDesc,
			&m_pSwapChain,
			&m_pD3dDevice,
			&m_featureLevel,
			&m_pD3dContext);

		if (SUCCEEDED(result))//if reseult >=0;
		{
			m_driverType = driverTypes[driver];
			break;
		}
	}
	if (FAILED(result))
	{
		DXTRACE_MSG("Failed to create the Direct3D device!");
		return false;
	}
	return true;
}

bool DXbase::SetUpRenderTargetView() {
	HRESULT result;

	// get a pointer directly to the back buffer
	result = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&m_pBackBufferTexture);
	if (FAILED(result))
	{
		DXTRACE_MSG("Failed to get the swap chain back buffer!");
		return false;
	}

	// create a render target pointing to the back buffer
	result = m_pD3dDevice->CreateRenderTargetView(m_pBackBufferTexture, nullptr, &m_pBackBufferTarget);

	if (m_pBackBufferTexture)
		m_pBackBufferTexture->Release();

	if (FAILED(result))
	{
		DXTRACE_MSG("Failed to create the render target view!");
		return false;
	}


	/*Bind one or more render targets atomically and the depth-stencil buffer to the output-merger stage.*/

	// If we use multiple swapChain, then we need to Set the RenderTargets ever frame...

	//m_pD3dContext->OMSetRenderTargets(1, &m_pBackBufferTarget, nullptr);
	return true;
}

void DXbase::SetViewportAndDepthBuffer() {

	//======================================================================================
	//1.Create a depth buffer object and initialize it with the texture.
	//======================================================================================
	D3D11_TEXTURE2D_DESC texd = { 0 };

	texd.Width = GAME_WIDTH;
	texd.Height = GAME_HEIGHT;
	texd.ArraySize = 1;
	texd.MipLevels = 1;
	texd.SampleDesc.Count = 1;
	texd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	texd.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	
	m_pD3dDevice->CreateTexture2D(&texd, nullptr, &depthBufferTexture);

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));

	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	m_pD3dDevice->CreateDepthStencilView(depthBufferTexture, &dsvd, &depthBuffer);

	//=====================================================================
	//2. Set the Output-Merger Stage to write to the depth buffer.
	//=====================================================================
	m_pD3dContext->OMSetRenderTargets(1, &m_pBackBufferTarget, depthBuffer);

	//=====================================================================
	//3. Set the Rasterizer Stage to check the depth of each pixel before drawing.
	//=====================================================================
	D3D11_VIEWPORT viewport;
	viewport.Width = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.MinDepth = 0;  // the closest an object can be on the depth buffer is 0.0
	viewport.MaxDepth = 1;  // the farthest an object can be on the depth buffer is 1.0

	//RSSetViewports: Bind an array of viewports to the rasterizer stage of the pipeline.
	m_pD3dContext->RSSetViewports(1, &viewport);
}

bool DXbase::Initialize(HINSTANCE hinstance, HWND hw)
{
	m_hInstance = hinstance;
	m_hwnd = hw;

	RECT dimensions;
	GetClientRect(hw, &dimensions);// the upper-left corner are (0,0).
	width = dimensions.right - dimensions.left;
	height = dimensions.bottom - dimensions.top;
//===========================================================
// Specifying the driver type and feature levels. we do it at DXbaseConst.h
//===========================================================
	if (!CreateDeviceAndSwapChain(hw)){return false;}
	if (!SetUpRenderTargetView()) {return false;}
	SetViewportAndDepthBuffer();
	//======================================================================
	return LoadContent();
}

void DXbase::DxBaseClearRenderTargetView(ID3D11RenderTargetView* m_pRenderTargetView, const FLOAT ColorRGBA[4])
{
	//float clearColor[4] = { 0.0f, 0.0f, 0.25f, 1.0f };
	m_pD3dContext->ClearRenderTargetView(m_pBackBufferTarget, ColorRGBA);
}

bool DXbase::CompileD3DShader(char* filePath,char* entry,char* shaderModel, ID3DBlob** buffer) {
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined( DEBUG ) || defined( _DEBUG )
	shaderFlags |= D3DCOMPILE_DEBUG;
#endif
	//ID3DBlob interface
	//
	//This interface is used to return data of arbitrary length.
	//Methods:
	//		 ---GetBufferPointer :Retrieves a pointer to the blob's data.
	//       ---GetBufferSize: Retrieves the size, in bytes, of the blob's data.
	ID3DBlob* errorBuffer = 0;
	HRESULT result;
	//more detail at P101;
	result = D3DX11CompileFromFile(filePath, 0, 0, entry, shaderModel,
		shaderFlags, 0, 0, buffer, &errorBuffer, 0);

	if (FAILED(result))
	{
		if (errorBuffer != 0)
		{
			OutputDebugStringA((char*)errorBuffer->GetBufferPointer());
			errorBuffer->Release();
		}

		return false;
	}

	if (errorBuffer != 0)
		errorBuffer->Release();

	return true;
}

HRESULT DXbase::showBackBuffer()
{
	return E_NOTIMPL;
}

HRESULT DXbase::getDeviceState()
{
	return E_NOTIMPL;
}

HRESULT DXbase::reset()
{
	return E_NOTIMPL;
}

bool DXbase::LoadContent()
{
	// Load shader...whatever....
	// Override with demo specifics, if any. . .
	return true;
}

void DXbase::UnloadContent()
{
	// Override with demo specifics, if any. . .
}
