#include "dxrframework.h"
#include "../WinApplication/WinApplication.h"
#include "DXRenderer.h"

UEngine::DXRenderer UEngine::DXRenderer::instance;

void UEngine::DXRenderer::Init(HWND outputWindow, const DXRenderingDesc& desc)
{

	RECT clientSize;
	hwnd = outputWindow;
	rendering_desc = desc;
	featureLevel = static_cast<D3D_FEATURE_LEVEL>(0);
	GetClientRect(outputWindow, &clientSize);
	InitViewport(&immediate.Viewport, clientSize);
	InitDeviceContextSwapchain(clientSize, rendering_desc.IsDebuggable);

	// TODO: initialize resources in multi-threading environment
	// multi-thread safe processes
	{
		// Render Target View
		InitRenderTargetView(immediate.RenderTargetView.GetAddressOf());

		// Depth Stencil Texture, View, State
		InitDepthStencil
		(
			immediate.DepthStencilTexture2D.GetAddressOf(),
			immediate.DepthStencilView.GetAddressOf(),
			immediate.DepthStencilState.GetAddressOf(),
			clientSize,
			DSSCreateDesc(),
			DSVCreateDesc()
		);

		// Rasterizer State
		InitRasterizerState
		(
			default_pipeline.rasterizerState.GetAddressOf(),
			immediate.DepthStencilView.Get(),
			rendering_desc.FillMode,
			rendering_desc.CullMode,
			rendering_desc.MultiSampleDesc.enable,
			rendering_desc.AntialiasedLineEnable,
			rendering_desc.ScissorEnable
		);

		// Sampler State
		auto ssDesc = SSCreateDesc();
		device->CreateSamplerState(&ssDesc, &default_pipeline.samplerState);

		// Blending State
		auto bsDesc = BSCreateDesc();
		device->CreateBlendState(&bsDesc, &default_pipeline.blendingState);
	}
}

void UEngine::DXRenderer::Begin(const float clearRGBA[4])
{
	// clearing depth buffer and render target
	immediate.DeviceContext->ClearRenderTargetView(immediate.RenderTargetView.Get(), clearRGBA);
	immediate.DeviceContext->RSSetViewports(1, &immediate.Viewport);
}

void UEngine::DXRenderer::End()
{
	immediate.DeviceContext->OMSetRenderTargets(1, immediate.RenderTargetView.GetAddressOf(), immediate.DepthStencilView.Get());
	immediate.DeviceContext->OMSetDepthStencilState(immediate.DepthStencilState.Get(), 1);

	swapchain->Present(0, 0);
}

void UEngine::DXRenderer::ResizeMainRenderTarget(UINT width, UINT height)
{
	swapchain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
	InitRenderTargetView(immediate.RenderTargetView.GetAddressOf());
}

D3D11_DEPTH_STENCIL_DESC UEngine::DXRenderer::DSSCreateDesc() const
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

D3D11_DEPTH_STENCIL_VIEW_DESC UEngine::DXRenderer::DSVCreateDesc() const
{
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	descDSV.Flags = NULL;
	descDSV.Texture2D.MipSlice = NULL;

	return descDSV;
}

D3D11_SAMPLER_DESC UEngine::DXRenderer::SSCreateDesc() const
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

D3D11_BLEND_DESC UEngine::DXRenderer::BSCreateDesc() const
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

void UEngine::DXRenderer::InitConstBuffer(UINT byteWidth, ID3D11Buffer** constBuffer)
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
	desc.SampleDesc = rendering_desc.MultiSampleDesc.enable ? rendering_desc.MultiSampleDesc.sample_description : DXGI_SAMPLE_DESC{ 1, 0 };
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

void UEngine::DXRenderer::InitRenderTargetView(ID3D11RenderTargetView** const rtv)
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
	RECT clientSize,
	D3D11_DEPTH_STENCIL_DESC dss_desc,
	D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc
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

	if (device->CreateTexture2D(&depthBuffer, nullptr, depth_stencil_texture) != S_OK) throw std::runtime_error::runtime_error("Rendering Creation Failed!");
	if (device->CreateDepthStencilState(&dss_desc, depth_stencil_state) != S_OK) throw std::runtime_error::runtime_error("Rendering Creation Failed!");
	if (device->CreateDepthStencilView(*depth_stencil_texture, &dsv_desc, depth_stencil_view) != S_OK) throw std::runtime_error::runtime_error("Rendering Creation Failed!");
}

void UEngine::DXRenderer::InitRasterizerState
(
	ID3D11RasterizerState** const rasterizerState,
	const ID3D11DepthStencilView* const dsv,
	const D3D11_FILL_MODE& fillMode,
	const D3D11_CULL_MODE& cullMode,
	const BOOL& multisampleEnable,
	const BOOL& antialisedEnable,
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
	desc.DepthClipEnable = dsv ? true : false; //깊이클리핑 끄기
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
