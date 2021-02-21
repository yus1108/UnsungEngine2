#include "dxrframework.h"
#include "DXRenderer.h"
#include "../WinApplication/WinApplication.h"

UEngine::DXRenderer UEngine::DXRenderer::instance;

void UEngine::DXRenderer::Init
(
	HWND outputWindow,
	const DX_RENDERER_DESC* desc
)
{
	Release();

	RECT clientSize;
	hwnd = outputWindow;
	rendering_desc = CreateDefaultInitDesc();
	if (desc) rendering_desc = *desc;
	GetClientRect(outputWindow, &clientSize);
	InitViewport(&immediate.Viewport, clientSize);
	InitDeviceContextSwapchain(clientSize, rendering_desc.IsDebuggable);

	// TODO: initialize resources in multi-threading environment
	// multi-thread safe processes
	{
		// Render Target View
		InitMainRenderTargetView(immediate.RenderTargetView.GetAddressOf());

		// Depth Stencil Texture, View, State
		InitDepthStencil
		(
			rendering_desc.EnableDepthStencil,
			clientSize,
			immediate.DepthStencilState.GetAddressOf(),
			immediate.DepthStencilTexture2D.GetAddressOf(),
			immediate.DepthStencilView.GetAddressOf(),
			rendering_desc.MultisampleDesc
		);

		{
			// Shader
			UEngine::DX_RASTERIZER_DESC rsDesc = UEngine::DX_RASTERIZER_DESC();
			rsDesc.EnableAntialisedLine = rendering_desc.EnableAntialisedLine;
			rsDesc.EnableDepthStencil = rendering_desc.EnableDepthStencil;
			rsDesc.EnableMultisampling = rendering_desc.EnableMultisampling;
			auto default_shader = DXShader::Instantiate
			(
				this,
				"../_Shaders/DefaultVS.hlsl",
				"../_Shaders/DefaultPS.hlsl",
				rendering_desc.IsDebuggable,
				rendering_desc.EnableBlendState,
				&rsDesc
			);

			default_color_shader = DXShader::Instantiate
			(
				this,
				"../_Shaders/DefaultVS.hlsl",
				"../_Shaders/ColorPS.hlsl",
				rendering_desc.IsDebuggable,
				rendering_desc.EnableBlendState,
				&rsDesc
			);

			// RenderMesh
			SIMPLE_VERTEX vertices[] =
			{
				SIMPLE_VERTEX{DirectX::XMFLOAT3{-1, -1, 0}, DirectX::XMFLOAT2{0, 1}},
				SIMPLE_VERTEX{DirectX::XMFLOAT3{-1, 1, 0}, DirectX::XMFLOAT2{0, 0}},
				SIMPLE_VERTEX{DirectX::XMFLOAT3{1, -1, 0}, DirectX::XMFLOAT2{1, 1}},
				SIMPLE_VERTEX{DirectX::XMFLOAT3{1, 1, 0}, DirectX::XMFLOAT2{1, 0}},
			};
			unsigned indices[] = { 0, 1, 2, 2, 1, 3 };
			auto default_renderMesh = DXRenderMesh::Instantiate<SIMPLE_VERTEX>(device.Get(), &vertices[0], ARRAYSIZE(vertices), indices, ARRAYSIZE(indices));

			// RenderObject
			DirectX::XMFLOAT4 color{ 1,1,1,1 };
			default_renderObject = DXRenderObject::Instantiate(default_renderMesh, default_shader, true);
			default_renderObject->AddConstantBuffer(this, "Color", sizeof(DirectX::XMFLOAT4), UENGINE_DXSHADERTYPE_PIXEL_SHADER);
			default_renderObject->CBCopyData<DirectX::XMFLOAT4>("Color", &color, sizeof(color));
		}
	}
}

void UEngine::DXRenderer::Release()
{
	DXRenderObject::Release(&default_renderObject);
}

void UEngine::DXRenderer::UpdateConstantBuffers()
{
	default_renderObject->CBUpdateAll(immediate.DeviceContext.Get());
}

void UEngine::DXRenderer::Begin(const float clearRGBA[4])
{
	immediate.DeviceContext->ClearRenderTargetView(immediate.RenderTargetView.Get(), clearRGBA);
	if (immediate.DepthStencilView) immediate.DeviceContext->ClearDepthStencilView(immediate.DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	immediate.DeviceContext->RSSetViewports(1, &immediate.Viewport);

	default_renderObject->Set(immediate.DeviceContext.Get());

	immediate.DeviceContext->OMSetDepthStencilState(immediate.DepthStencilState.Get(), 1);
	immediate.DeviceContext->OMSetRenderTargets(1, immediate.RenderTargetView.GetAddressOf(), immediate.DepthStencilView.Get());
}

void UEngine::DXRenderer::End()
{
	default_renderObject->Draw(immediate.DeviceContext.Get());
	swapchain->Present(0, 0);
}

void UEngine::DXRenderer::ResizeMainRenderTarget(UINT width, UINT height)
{
	swapchain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
	InitMainRenderTargetView(immediate.RenderTargetView.GetAddressOf());
}

UEngine::DX_RENDERER_DESC UEngine::DXRenderer::CreateDefaultInitDesc()
{
	DX_RASTERIZER_DESC rasterDesc;
	ZeroMemory(&rasterDesc, sizeof(DX_RASTERIZER_DESC));
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.CullMode = D3D11_CULL_BACK;

	DX_RENDERER_DESC renderingDesc;
	ZeroMemory(&renderingDesc, sizeof(DX_RENDERER_DESC));
	renderingDesc.RefreshRate = { 60, 1 };
	renderingDesc.MultisampleDesc = { 1, 0 };
	renderingDesc.FillMode = D3D11_FILL_SOLID;
	renderingDesc.CullMode = D3D11_CULL_BACK;
	renderingDesc.IsDebuggable = true;

	return renderingDesc;
}

void UEngine::DXRenderer::InitConstantBuffer(UINT byteWidth, ID3D11Buffer** constBuffer)
{
	// Create Constant Buffers
	D3D11_BUFFER_DESC constBufferDesc;
	ZeroMemory(&constBufferDesc, sizeof(constBufferDesc));
	constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufferDesc.ByteWidth = byteWidth;
	device->CreateBuffer(&constBufferDesc, nullptr, constBuffer);
}

void UEngine::DXRenderer::InitViewport(D3D11_VIEWPORT* const _viewport, const RECT& clientSize)
{
	// viewport setting
	_viewport->Width = (float)(clientSize.right - clientSize.left);
	_viewport->Height = (float)(clientSize.bottom - clientSize.top);
	_viewport->MinDepth = 0;
	_viewport->MaxDepth = 1;
	_viewport->TopLeftX = (float)clientSize.left;
	_viewport->TopLeftY = (float)clientSize.top;
}

void UEngine::DXRenderer::InitDeviceContextSwapchain(const RECT& clientSize, bool isDebuggable)
{
	//// directX compatibility
	D3D_FEATURE_LEVEL FeatureLevels[2];
	FeatureLevels[0] = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;
	FeatureLevels[1] = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_1;

	// debugging directX
	UINT flag = D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_BGRA_SUPPORT;

	// window specification
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BufferCount = 1; // 이중 버퍼링으로 고정
	desc.BufferDesc.Width = (UINT)(clientSize.right - clientSize.left);
	desc.BufferDesc.Height = (UINT)(clientSize.bottom - clientSize.top);
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.RefreshRate = rendering_desc.RefreshRate;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	desc.OutputWindow = hwnd;
	desc.SampleDesc = rendering_desc.MultisampleDesc;
	desc.Windowed = !rendering_desc.IsFullScreen;

	if (isDebuggable) flag |= D3D11_CREATE_DEVICE_DEBUG;

	// create swapchain, device, and context
	HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr,
		D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		flag,
		rendering_desc.FeatureLevels == nullptr ? FeatureLevels : rendering_desc.FeatureLevels,
		rendering_desc.FeatureLevels == nullptr ? 2 : rendering_desc.FeatureLevelsCount,
		D3D11_SDK_VERSION,
		&desc,
		swapchain.GetAddressOf(),
		device.GetAddressOf(),
		&featureLevel,
		immediate.DeviceContext.GetAddressOf());

	assert(SUCCEEDED(hr) && "Not able to create Device and Swapchain");
}

void UEngine::DXRenderer::InitMainRenderTargetView(ID3D11RenderTargetView** const rtv)
{
	Microsoft::WRL::ComPtr<ID3D11Debug> pDebugger;

	device->QueryInterface(__uuidof(ID3D11Debug), (void**)&pDebugger);
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pSwapChainBuffer;
	swapchain->GetBuffer(0, __uuidof(pSwapChainBuffer), (void**)&pSwapChainBuffer);
	device->CreateRenderTargetView(pSwapChainBuffer.Get(), nullptr, rtv);
}

void UEngine::DXRenderer::InitDepthStencil
(
	bool EnableDepthStencil,
	const RECT clientSize,
	ID3D11DepthStencilState** const depth_stencil_state,
	ID3D11Texture2D** const depth_stencil_texture,
	ID3D11DepthStencilView** const depth_stencil_view,
	DXGI_SAMPLE_DESC sampleDesc
)
{
	D3D11_DEPTH_STENCIL_DESC depthState;

	// Depth test parameters
	depthState.DepthEnable = EnableDepthStencil;
	depthState.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthState.DepthFunc = D3D11_COMPARISON_LESS;

	// Stencil test parameters
	depthState.StencilEnable = EnableDepthStencil;
	depthState.StencilReadMask = 0xFF;
	depthState.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	depthState.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthState.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthState.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthState.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	depthState.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthState.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthState.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthState.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	D3D11_TEXTURE2D_DESC depthBuffer;
	depthBuffer.Width = (UINT)(clientSize.right - clientSize.left);
	depthBuffer.Height = (UINT)(clientSize.bottom - clientSize.top);
	depthBuffer.MipLevels = 1;
	depthBuffer.ArraySize = 1;
	depthBuffer.Format = DXGI_FORMAT_D32_FLOAT;
	depthBuffer.SampleDesc = sampleDesc;
	depthBuffer.Usage = D3D11_USAGE_DEFAULT;
	depthBuffer.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBuffer.CPUAccessFlags = NULL;
	depthBuffer.MiscFlags = NULL;

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	descDSV.Flags = NULL;
	descDSV.Texture2D.MipSlice = NULL;

	if (device->CreateDepthStencilState(&depthState, depth_stencil_state) != S_OK) throw std::runtime_error::runtime_error("Rendering Creation Failed!");
	if (EnableDepthStencil == false) return;
	if (device->CreateTexture2D(&depthBuffer, nullptr, depth_stencil_texture) != S_OK) throw std::runtime_error::runtime_error("Rendering Creation Failed!");
	if (device->CreateDepthStencilView(*depth_stencil_texture, &descDSV, depth_stencil_view) != S_OK) throw std::runtime_error::runtime_error("Rendering Creation Failed!");
}

void UEngine::DXRenderer::InitRenderViewContext
(
	UEngine::DXRenderViewContext** const context,
	UINT width,
	UINT height,
	bool EnableDepthStencil,
	DXGI_SAMPLE_DESC sampleDesc
)
{
	RECT viewSize = { 0 };
	viewSize.right = width;
	viewSize.bottom = height;

	InitViewport(&(*context)->Viewport, viewSize);

	// Depth Stencil Texture, View, State
	InitDepthStencil
	(
		EnableDepthStencil,
		viewSize,
		(*context)->DepthStencilState.GetAddressOf(),
		(*context)->DepthStencilTexture2D.GetAddressOf(),
		(*context)->DepthStencilView.GetAddressOf(),
		sampleDesc
	);

	//
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc.SampleDesc = sampleDesc;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	device->CreateTexture2D(&desc, nullptr, (*context)->RenderTargetViewTexture2D.GetAddressOf());

	/////////////////////// Map's Render Target
		// Setup the description of the render target view.
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = desc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	HRESULT h_ok = device->CreateRenderTargetView
	(
		(*context)->RenderTargetViewTexture2D.Get(), 
		&renderTargetViewDesc, 
		(*context)->RenderTargetView.GetAddressOf()
	);

	(*context)->width = width;
	(*context)->height = height;

	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	// Setup the description of the shader resource view.
	D3D11_SHADER_RESOURCE_VIEW_DESC singleSRVDesc;
	singleSRVDesc.Format = desc.Format;
	singleSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	singleSRVDesc.Texture2D.MostDetailedMip = 0;
	singleSRVDesc.Texture2D.MipLevels = 1;

	device->CreateTexture2D(&desc, nullptr, (*context)->OutputTexture2D.GetAddressOf());
	device->CreateShaderResourceView
	(
		(*context)->OutputTexture2D.Get(), 
		&singleSRVDesc, 
		(*context)->OutputShaderResourceView.GetAddressOf()
	);

	device->CreateDeferredContext(NULL, &(*context)->DeviceContext);
}

void UEngine::DXRenderer::InitRasterizerState
(
	ID3D11RasterizerState** const rasterizerState,
	const D3D11_FILL_MODE& fillMode,
	const D3D11_CULL_MODE& cullMode,
	const BOOL& multisampleEnable,
	const BOOL& antialisedEnable,
	const BOOL& depthClipEnable
)
{
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
	desc.FillMode = fillMode; // D3D11_FILL_WIREFRAME은 테두리만나옴
	desc.CullMode = cullMode; //컬링할때 앞면,뒷면 추려내기 할건지
	desc.DepthBias = 0;
	desc.FrontCounterClockwise = false; //cw ccw 값 정하기
	desc.DepthBiasClamp = 0;
	desc.SlopeScaledDepthBias = 0;
	desc.DepthClipEnable = depthClipEnable; //깊이클리핑 끄기
	desc.ScissorEnable = false; //시저테스트 하지 않음
	desc.MultisampleEnable = multisampleEnable; //멀티 샘플링 하지않음
	desc.AntialiasedLineEnable = antialisedEnable; //라인안티앨리어싱 없음
	device->CreateRasterizerState(&desc, rasterizerState);
}

