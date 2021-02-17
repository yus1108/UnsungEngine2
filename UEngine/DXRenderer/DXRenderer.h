#pragma once
#include "dxrframework.h"

namespace UEngine
{
	using namespace UEngine;

	class DXRenderer final
	{
#pragma region Singleton
	public:
		static DXRenderer* Get() { return &instance; }

	private:
		DXRenderer() : hwnd(NULL) {};
		~DXRenderer() = default;
		static DXRenderer instance;
#pragma endregion

	private:
		HWND hwnd;
		//com 인터페이스
		Microsoft::WRL::ComPtr<ID3D11Device> device;
		Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;
		DXRenderViewContext immediate; // main render view
		DXShader* default_shader;
		DXRenderingDesc rendering_desc;
		D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_1_0_CORE;

	public:
		void Init(HWND OutputWindow, const DXRenderingDesc& desc = DXRenderingDesc());
		void Begin(const float clearRGBA[4] = DirectX::Colors::Transparent);
		void End();

		void ResizeMainRenderTarget(UINT width, UINT height);

		D3D_FEATURE_LEVEL GetFeatureLevel() { return featureLevel; }
		ID3D11Device* const GetDevice() { return device.Get(); }
		ID3D11DeviceContext* const GetImmediateDeviceContext() { return immediate.DeviceContext.Get(); }

		D3D11_DEPTH_STENCIL_DESC DSSCreateDesc() const;
		D3D11_DEPTH_STENCIL_VIEW_DESC DSVCreateDesc() const;
		D3D11_SAMPLER_DESC SSCreateDesc() const;
		D3D11_BLEND_DESC BSCreateDesc() const;

	private:
		void InitConstBuffer(UINT byteWidth, ID3D11Buffer** constBuffer);
		void InitViewport(D3D11_VIEWPORT* const _viewport, const RECT& clientSize);
		void InitDeviceContextSwapchain(const RECT& clientSize, bool isDebuggable);
		void InitRenderTargetView(ID3D11RenderTargetView** const rtv);
		void InitDepthStencil
		(
			ID3D11Texture2D** const depth_stencil_texture,
			ID3D11DepthStencilView** const depth_stencil_view,
			ID3D11DepthStencilState** const depth_stencil_state,
			RECT clientSize,
			D3D11_DEPTH_STENCIL_DESC dss_desc,
			D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc
		);
		void InitRasterizerState
		(
			ID3D11RasterizerState** const rasterizerState,
			const ID3D11DepthStencilView* const dsv = nullptr,
			const D3D11_FILL_MODE& fillMode = D3D11_FILL_SOLID,
			const D3D11_CULL_MODE& cullMode = D3D11_CULL_BACK,
			const BOOL& multisampleEnable = false,
			const BOOL& antialisedEnable = false,
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
