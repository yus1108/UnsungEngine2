#pragma once
#include "dxrframework.h"
#include "DXRenderMesh.h"

namespace UEngine
{
	using namespace UEngine;

	class DXRenderer final
	{
#pragma region Singleton
	public:
		static DXRenderer* Get() { return &instance; }

	private:
		DXRenderer() = default;
		~DXRenderer() { Release(); }
		static DXRenderer instance;
#pragma endregion

	private:
		HWND hwnd{ NULL };
		Microsoft::WRL::ComPtr<ID3D11Device> device;
		Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;
		D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_1_0_CORE;

		DXRenderingDesc rendering_desc;

		DXRenderViewContext immediate; // main render view
		class DXShader* default_shader{ nullptr };
		class DXRenderMesh* default_renderMesh{ nullptr };

	public:
		void Init(HWND outputWindow, const DXRenderingDesc* desc = nullptr);
		void Release();
		void Begin(const float clearRGBA[4] = DirectX::Colors::Transparent);
		void End();

		void ResizeMainRenderTarget(UINT width, UINT height);

		D3D_FEATURE_LEVEL GetFeatureLevel() { return featureLevel; }
		ID3D11Device* const GetDevice() { return device.Get(); }
		ID3D11DeviceContext* const GetImmediateDeviceContext() { return immediate.DeviceContext.Get(); }

		static DXRenderingDesc CreateDefaultInitDesc();

		void InitConstantBuffer(UINT byteWidth, ID3D11Buffer** constBuffer);

		void InitViewport(D3D11_VIEWPORT* const _viewport, const RECT& clientSize);
		void InitDeviceContextSwapchain(const RECT& clientSize, bool isDebuggable);
		void InitMainRenderTargetView(ID3D11RenderTargetView** const rtv);
		void InitDepthStencil
		(
			bool enableDepthStencil,
			const RECT clientSize,
			ID3D11DepthStencilState** const depth_stencil_state,
			ID3D11Texture2D** const depth_stencil_texture,
			ID3D11DepthStencilView** const depth_stencil_view,
			DXGI_SAMPLE_DESC sampleDesc
		);
		void InitRenderViewContext
		(
			UEngine::DXRenderViewContext** const context,
			UINT width,
			UINT height,
			bool enableDepthStencil,
			DXGI_SAMPLE_DESC sampleDesc
		);
		void InitRasterizerState
		(
			ID3D11RasterizerState** const rasterizerState,
			const D3D11_FILL_MODE& fillMode = D3D11_FILL_SOLID,
			const D3D11_CULL_MODE& cullMode = D3D11_CULL_BACK,
			const BOOL& multisampleEnable = false,
			const BOOL& antialisedEnable = false,
			const BOOL& depthClipEnable = false,
			const BOOL& scissorEnable = false
		);

		void VSSetConstantBuffers
		(
			ID3D11Buffer* const* const constBuffer, 
			const void* data, 
			size_t _Size, 
			const unsigned& startSlot, 
			ID3D11DeviceContext* const deviceContext = nullptr,
			const unsigned& numBuffers = 1
		);
		void PSSetConstantBuffers
		(
			ID3D11Buffer* const* const constBuffer, 
			const void* data, 
			size_t _Size, 
			const unsigned& startSlot, 
			ID3D11DeviceContext* const deviceContext = nullptr,
			const unsigned& numBuffers = 1
		);
	};
}
