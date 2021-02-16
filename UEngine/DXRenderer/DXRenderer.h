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
		DXRenderer() = default;
		~DXRenderer() = default;
		static DXRenderer instance;
#pragma endregion

	private:
		//com 인터페이스
		Microsoft::WRL::ComPtr<ID3D11Device> device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediateDeviceContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;
		DXRenderViewResource default_render_view_resource; // main render view
		DXRPipeline default_pipeline;
		DXRenderingDesc rendering_desc;
		D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_1_0_CORE;

	public:
		void Init(const DXRenderingDesc& desc = DXRenderingDesc());
		void Begin(ID3D11DeviceContext* const deviceContext = nullptr);
		void End();

		void ResizeMainRenderTarget(UINT width, UINT height);

		D3D_FEATURE_LEVEL GetFeatureLevel() { return featureLevel; }
		ID3D11Device* const GetDevice() { return device.Get(); }
		ID3D11DeviceContext* const GetImmediateDeviceContext() { return immediateDeviceContext.Get(); }

	private:
		void InitConstBuffer(UINT byteWidth, ID3D11Buffer** constBuffer);
		void InitViewport(D3D11_VIEWPORT& _viewport, const RECT& clientSize);
		void InitDeviceContextSwapchain(const RECT& clientSize, bool isDebuggable);
		void InitRenderTargetView(ID3D11RenderTargetView** rtv);

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
