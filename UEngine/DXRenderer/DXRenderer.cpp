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
	InitViewport(default_render_view_resource.viewport, clientSize);
	InitDeviceContextSwapchain(clientSize, rendering_desc.IsDebuggable);

	// TODO: initialize resources in multi-threading environment
	// multi-thread safe processes
	{
		// Render Target View
		InitRenderTargetView(default_render_view_resource.render_target_view.GetAddressOf());

		// Disable DepthStencil
		{
			//해당 뎁스스텐실 기능관련부분은 꺼두는 옵션으로 설정
			D3D11_DEPTH_STENCIL_DESC desc = { 0 };
			desc.DepthEnable = false;
			desc.StencilEnable = false;

			device->CreateDepthStencilState(&desc, default_render_view_resource.depth_stencil_state.GetAddressOf());
		}
		default_render_view_resource.depth_stencil_view = nullptr;

		// Rasterizer State
		{
			D3D11_RASTERIZER_DESC desc;
			ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
			desc.FillMode = rendering_desc.FillMode; // D3D11_FILL_WIREFRAME은 테두리만나옴
			desc.CullMode = rendering_desc.CullMode; //컬링할때 앞면,뒷면 추려내기 할건지
			desc.DepthBias = 0;
			desc.FrontCounterClockwise = false; //cw ccw 값 정하기
			desc.DepthBiasClamp = 0;
			desc.SlopeScaledDepthBias = 0;
			desc.DepthClipEnable = default_render_view_resource.depth_stencil_view ? true : false; //깊이클리핑 끄기
			desc.ScissorEnable = rendering_desc.ScissorEnable; //시저테스트 하지 않음
			desc.MultisampleEnable = rendering_desc.MultiSampleDesc.enable; //멀티 샘플링 하지않음
			desc.AntialiasedLineEnable = rendering_desc.AntialiasedLineEnable; //라인안티앨리어싱 없음
			device->CreateRasterizerState(&desc, default_pipeline.rasterizerState.GetAddressOf());
		}
	}
}

void UEngine::DXRenderer::Begin(ID3D11DeviceContext* const deviceContext)
{
	// clearing depth buffer and render target
	immediateDeviceContext->OMSetDepthStencilState(default_render_view_resource.depth_stencil_state.Get(), 1);
	immediateDeviceContext->OMSetRenderTargets(1, default_render_view_resource.render_target_view.GetAddressOf(),
		default_render_view_resource.depth_stencil_view.Get());

	immediateDeviceContext->ClearRenderTargetView(default_render_view_resource.render_target_view.Get(), DirectX::Colors::Transparent);
}

void UEngine::DXRenderer::End()
{
	swapchain->Present(0, 0);
}

void UEngine::DXRenderer::ResizeMainRenderTarget(UINT width, UINT height)
{
	swapchain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
	InitRenderTargetView(default_render_view_resource.render_target_view.GetAddressOf());
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

void UEngine::DXRenderer::InitViewport(D3D11_VIEWPORT& _viewport, const RECT& clientSize)
{
	// viewport setting
	_viewport.Width = (float)(clientSize.right - clientSize.left);
	_viewport.Height = (float)(clientSize.bottom - clientSize.top);
	_viewport.MinDepth = 0;
	_viewport.MaxDepth = 1;
	_viewport.TopLeftX = (float)clientSize.left;
	_viewport.TopLeftY = (float)clientSize.top;
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
		immediateDeviceContext.GetAddressOf());

	assert(SUCCEEDED(hr) && "Not able to create Device and Swapchain");
}

void UEngine::DXRenderer::InitRenderTargetView(ID3D11RenderTargetView** rtv)
{
	Microsoft::WRL::ComPtr<ID3D11Debug> pDebugger;

	device->QueryInterface(__uuidof(ID3D11Debug), (void**)&pDebugger);
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pSwapChainBuffer;
	swapchain->GetBuffer(0, __uuidof(pSwapChainBuffer), (void**)&pSwapChainBuffer);
	device->CreateRenderTargetView(pSwapChainBuffer.Get(), nullptr, rtv);
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
		VSSetConstantBuffers(constBuffer, data, _Size, startSlot, immediateDeviceContext.Get(), numBuffers);
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
		PSSetConstantBuffers(constBuffer, data, _Size, startSlot, immediateDeviceContext.Get(), numBuffers);
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
