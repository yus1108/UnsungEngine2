#include "dxrframework.h"
#include "DXRenderer.h"
#include "../WinApplication/WinApplication.h"

UEngine::DXRenderer UEngine::DXRenderer::instance;

void UEngine::DXRenderer::Init
(
	HWND outputWindow,
	bool enableDepthStencil,
	bool enableBlendState,
	const DXRenderingDesc* desc,
	const D3D11_DEPTH_STENCIL_DESC* dssDesc,
	const D3D11_DEPTH_STENCIL_VIEW_DESC* dsvDesc ,
	const D3D11_SAMPLER_DESC* ssDesc,
	const D3D11_BLEND_DESC* bsDesc
)
{
	Release();

	RECT clientSize;
	hwnd = outputWindow;
	rendering_desc = CreateDefaultInitDesc();
	if (desc) rendering_desc = *desc;
	featureLevel = static_cast<D3D_FEATURE_LEVEL>(0);
	GetClientRect(outputWindow, &clientSize);
	InitViewport(&immediate.Viewport, clientSize);
	InitDeviceContextSwapchain(clientSize, rendering_desc.IsDebuggable);

	// TODO: initialize resources in multi-threading environment
	// multi-thread safe processes
	{
		// Render Target View
		InitMainRenderTargetView(immediate.RenderTargetView.GetAddressOf());

		// Depth Stencil Texture, View, State
		auto default_dssDesc = DSSCreateDesc();
		auto default_dsvDesc = DSVCreateDesc();
		if (!dssDesc) dssDesc = &default_dssDesc;
		if (!dsvDesc) dsvDesc = &default_dsvDesc;
		InitDepthStencil
		(
			immediate.DepthStencilTexture2D.GetAddressOf(),
			enableDepthStencil ? immediate.DepthStencilView.GetAddressOf() : nullptr,
			immediate.DepthStencilState.GetAddressOf(),
			clientSize,
			dssDesc,
			dsvDesc
		);

		// Shader
		auto default_ssDesc = SSCreateDesc();
		auto default_bsDesc = BSCreateDesc();
		if (!ssDesc) ssDesc = &default_ssDesc;
		if (!bsDesc) bsDesc = &default_bsDesc;
		if (!enableBlendState) bsDesc = nullptr;
		default_shader = DXShader::Instantiate
		(
			this,
			"../_Shaders/DefaultVS.hlsl",
			"../_Shaders/DefaultPS.hlsl",
			rendering_desc.IsDebuggable,
			&rendering_desc.RasterizerStateDesc,
			ssDesc,
			bsDesc
		);

		// RenderMesh
		UEngine::SIMPLE_VERTEX vertices[] =
		{
			UEngine::SIMPLE_VERTEX{DirectX::XMFLOAT3{-1, -1, 0}, DirectX::XMFLOAT2{0, 1}},
			UEngine::SIMPLE_VERTEX{DirectX::XMFLOAT3{-1, 1, 0}, DirectX::XMFLOAT2{0, 0}},
			UEngine::SIMPLE_VERTEX{DirectX::XMFLOAT3{1, -1, 0}, DirectX::XMFLOAT2{1, 1}},
			UEngine::SIMPLE_VERTEX{DirectX::XMFLOAT3{1, 1, 0}, DirectX::XMFLOAT2{1, 0}},
		};
		unsigned indices[] = { 0, 1, 2, 2, 1, 3 };
		
		default_renderMesh = UEngine::DXRenderMesh::Instantiate<UEngine::SIMPLE_VERTEX>(device.Get(), &vertices[0], ARRAYSIZE(vertices), indices, ARRAYSIZE(indices));
	}
}

void UEngine::DXRenderer::Release()
{
	DXShader::Release(&default_shader);
	DXRenderMesh::Release(&default_renderMesh);
}

void UEngine::DXRenderer::Begin(const float clearRGBA[4])
{
	// clearing depth buffer and render target
	immediate.DeviceContext->ClearRenderTargetView(immediate.RenderTargetView.Get(), clearRGBA);
	immediate.DeviceContext->RSSetViewports(1, &immediate.Viewport);

	immediate.DeviceContext->OMSetRenderTargets(1, immediate.RenderTargetView.GetAddressOf(), immediate.DepthStencilView.Get());
	immediate.DeviceContext->OMSetDepthStencilState(immediate.DepthStencilState.Get(), 1);

	default_shader->Set(immediate.DeviceContext.Get());
	default_renderMesh->Set(immediate.DeviceContext.Get());
}

void UEngine::DXRenderer::End()
{
	immediate.DeviceContext->DrawIndexed(default_renderMesh->GetIndexCount(), 0, 0);

	swapchain->Present(0, 0);
}

void UEngine::DXRenderer::ResizeMainRenderTarget(UINT width, UINT height)
{
	swapchain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
	InitMainRenderTargetView(immediate.RenderTargetView.GetAddressOf());
}

UEngine::DXRenderingDesc UEngine::DXRenderer::CreateDefaultInitDesc()
{
	DXRasterDesc rasterDesc;
	ZeroMemory(&rasterDesc, sizeof(DXRasterDesc));
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.CullMode = D3D11_CULL_BACK;

	DXRenderingDesc renderingDesc;
	ZeroMemory(&renderingDesc, sizeof(DXRenderingDesc));
	renderingDesc.RefreshRate = { 60, 1 };
	renderingDesc.MultisampleDesc = { 1, 0 };
	renderingDesc.RasterizerStateDesc = rasterDesc;

	return renderingDesc;
}

D3D11_DEPTH_STENCIL_DESC UEngine::DXRenderer::DSSCreateDesc()
{
	D3D11_DEPTH_STENCIL_DESC depthState;

	// Depth test parameters
	depthState.DepthEnable = true;
	depthState.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthState.DepthFunc = D3D11_COMPARISON_LESS;

	// Stencil test parameters
	depthState.StencilEnable = true;
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

	return depthState;
}

D3D11_DEPTH_STENCIL_VIEW_DESC UEngine::DXRenderer::DSVCreateDesc()
{
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	descDSV.Flags = NULL;
	descDSV.Texture2D.MipSlice = NULL;

	return descDSV;
}

D3D11_SAMPLER_DESC UEngine::DXRenderer::SSCreateDesc()
{
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.MaxLOD = FLT_MAX;
	samplerDesc.MinLOD = -FLT_MAX;
	samplerDesc.MipLODBias = 0;

	return samplerDesc;
}

D3D11_BLEND_DESC UEngine::DXRenderer::BSCreateDesc()
{
	D3D11_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));
	desc.AlphaToCoverageEnable = true;
	desc.IndependentBlendEnable = false;
	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	return desc;
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
	ID3D11Texture2D** const depth_stencil_texture,
	ID3D11DepthStencilView** const depth_stencil_view,
	ID3D11DepthStencilState** const depth_stencil_state,
	const RECT clientSize,
	const D3D11_DEPTH_STENCIL_DESC* const dss_desc,
	const D3D11_DEPTH_STENCIL_VIEW_DESC* const dsv_desc
)
{
	D3D11_TEXTURE2D_DESC depthBuffer;
	depthBuffer.Width = (UINT)(clientSize.right - clientSize.left);
	depthBuffer.Height = (UINT)(clientSize.bottom - clientSize.top);
	depthBuffer.MipLevels = 1;
	depthBuffer.ArraySize = 1;
	depthBuffer.Format = DXGI_FORMAT_D32_FLOAT;
	depthBuffer.SampleDesc.Count = 1;
	depthBuffer.SampleDesc.Quality = 0;
	depthBuffer.Usage = D3D11_USAGE_DEFAULT;
	depthBuffer.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBuffer.CPUAccessFlags = NULL;
	depthBuffer.MiscFlags = NULL;

	if (device->CreateDepthStencilState(dss_desc, depth_stencil_state) != S_OK) throw std::runtime_error::runtime_error("Rendering Creation Failed!");
	if (depth_stencil_view == nullptr) return;
	if (device->CreateTexture2D(&depthBuffer, nullptr, depth_stencil_texture) != S_OK) throw std::runtime_error::runtime_error("Rendering Creation Failed!");
	if (device->CreateDepthStencilView(*depth_stencil_texture, dsv_desc, depth_stencil_view) != S_OK) throw std::runtime_error::runtime_error("Rendering Creation Failed!");
}

void UEngine::DXRenderer::InitRenderViewContext
(
	UEngine::DXRenderViewContext** const context,
	UINT width,
	UINT height,
	const D3D11_DEPTH_STENCIL_DESC* const dssDesc,
	const D3D11_DEPTH_STENCIL_VIEW_DESC* const dsvDesc,
	const DXGI_SAMPLE_DESC* const sampleDesc
)
{
	RECT viewSize = { 0 };
	viewSize.right = width;
	viewSize.bottom = height;

	InitViewport(&(*context)->Viewport, viewSize);

	// Depth Stencil Texture, View, State
	auto a = dssDesc && dsvDesc ? (*context)->DepthStencilView.GetAddressOf() : nullptr;
	auto default_dssDesc = DSSCreateDesc();

	InitDepthStencil
	(
		(*context)->DepthStencilTexture2D.GetAddressOf(),
		dssDesc && dsvDesc ? (*context)->DepthStencilView.GetAddressOf() : nullptr,
		(*context)->DepthStencilState.GetAddressOf(),
		viewSize,
		dssDesc ? dssDesc : &default_dssDesc,
		dsvDesc
	);

	//
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc.SampleDesc = sampleDesc ? *sampleDesc : DXGI_SAMPLE_DESC{ 1,0 };
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

	HRESULT h_ok = device->CreateRenderTargetView((*context)->RenderTargetViewTexture2D.Get(), &renderTargetViewDesc, (*context)->RenderTargetView.GetAddressOf());

	(*context)->width = width;
	(*context)->height = height;

	/*desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;*/

	// Setup the description of the shader resource view.
	D3D11_SHADER_RESOURCE_VIEW_DESC singleSRVDesc;
	singleSRVDesc.Format = desc.Format;
	singleSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	singleSRVDesc.Texture2D.MostDetailedMip = 0;
	singleSRVDesc.Texture2D.MipLevels = 1;

	//device->CreateTexture2D(&desc, nullptr, (*context)->OutputTexture2D.GetAddressOf());
	device->CreateShaderResourceView((*context)->RenderTargetViewTexture2D.Get(), &singleSRVDesc, (*context)->OutputShaderResourceView.GetAddressOf());

	device->CreateDeferredContext(NULL, &(*context)->DeviceContext);
}

void UEngine::DXRenderer::InitRasterizerState
(
	ID3D11RasterizerState** const rasterizerState,
	const D3D11_FILL_MODE& fillMode,
	const D3D11_CULL_MODE& cullMode,
	const BOOL& multisampleEnable,
	const BOOL& antialisedEnable,
	const BOOL& depthClipEnable,
	const BOOL& scissorEnable
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
	desc.ScissorEnable = scissorEnable; //시저테스트 하지 않음
	desc.MultisampleEnable = multisampleEnable; //멀티 샘플링 하지않음
	desc.AntialiasedLineEnable = antialisedEnable; //라인안티앨리어싱 없음
	device->CreateRasterizerState(&desc, rasterizerState);
}

void UEngine::DXRenderer::VSSetConstantBuffers
(
	ID3D11Buffer* const* const constBuffer, 
	const void* data, size_t _Size, 
	const unsigned& startSlot, 
	ID3D11DeviceContext* const deviceContext,
	const unsigned& numBuffers
)
{
	if (deviceContext == nullptr)
	{
		VSSetConstantBuffers(constBuffer, data, _Size, startSlot, immediate.DeviceContext.Get(), numBuffers);
		return;
	}

	// world matrix
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	deviceContext->Map(*constBuffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &mappedResource);
	memcpy(mappedResource.pData, data, _Size);
	deviceContext->Unmap(*constBuffer, 0);

	deviceContext->VSSetConstantBuffers(startSlot, numBuffers, constBuffer);
}

void UEngine::DXRenderer::PSSetConstantBuffers
(
	ID3D11Buffer* const* const constBuffer,
	const void* data, size_t _Size,
	const unsigned& startSlot,
	ID3D11DeviceContext* const deviceContext,
	const unsigned& numBuffers
)
{
	if (deviceContext == nullptr)
	{
		PSSetConstantBuffers(constBuffer, data, _Size, startSlot, immediate.DeviceContext.Get(), numBuffers);
		return;
	}

	// world matrix
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	deviceContext->Map(*constBuffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &mappedResource);
	memcpy(mappedResource.pData, data, _Size);
	deviceContext->Unmap(*constBuffer, 0);

	deviceContext->PSSetConstantBuffers(startSlot, numBuffers, constBuffer);
}
