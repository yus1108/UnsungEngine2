#pragma once
#include "dxrframework.h"
#include "RenderComponents\DXShader.h"
#include "RenderComponents\DXRenderMesh.h"

namespace UEngine
{
	namespace DXRenderer
	{
		class DXRenderer final
		{
	#pragma region Singleton
		public:
			static DXRenderer* Get() { if (instance == nullptr) instance = new DXRenderer; return instance; }
			static void Attach(DXRenderer* renderer) { instance = renderer; }
			static void Detach() { instance = nullptr; }
			static void Release();

		private:
			DXRenderer();
			~DXRenderer() = default;
			static DXRenderer* instance;
#pragma endregion

		private:
			HWND hwnd{ NULL };
			Microsoft::WRL::ComPtr<ID3D11Device> device;
			Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;
			D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_1_0_CORE;

			RENDERER_DESC rendering_desc;

			ViewContext immediate; // main render view
			class DXShader* default_shader{ nullptr };
			class DXRenderMesh* default_renderMesh{ nullptr };
			class DXConstantBuffer* default_colorBuffer{ nullptr };

			Microsoft::WRL::ComPtr<ID3D11DeviceContext> textureContext;
			Microsoft::WRL::ComPtr<ID3D11CommandList> textureCommands;
		public:
			class DXResourceManager* const ResourceManager;

			void Init(HWND outputWindow, const RENDERER_DESC* desc = nullptr);
			void Begin(const float clearRGBA[4] = DirectX::Colors::Transparent);
			void Draw(ID3D11ShaderResourceView** sceneTexture);
			void End();

			void ResizeMainRenderTarget(UINT width, UINT height);

			RENDERER_DESC GetDescription() { return rendering_desc; }
			D3D_FEATURE_LEVEL GetFeatureLevel() { return featureLevel; }
			ID3D11Device* const GetDevice() { return device.Get(); }
			ID3D11DeviceContext* const GetImmediateDeviceContext() { return immediate.DeviceContext.Get(); }
			ID3D11DeviceContext* const GetTextureLoadDeviceContext() { return textureContext.Get(); }

			void InitConstantBuffer(UINT byteWidth, ID3D11Buffer** constBuffer);

			void InitViewport(D3D11_VIEWPORT* const _viewport, const RECT& clientSize);
			void InitDeviceContextSwapchain(const RECT& clientSize, bool isDebuggable);
			void InitMainRenderTargetView(ID3D11RenderTargetView** const rtv);
			void InitDepthStencil
			(
				bool EnableDepthStencil,
				const RECT clientSize,
				ID3D11DepthStencilState** const depth_stencil_state,
				ID3D11Texture2D** const depth_stencil_texture,
				ID3D11DepthStencilView** const depth_stencil_view,
				DXGI_SAMPLE_DESC sampleDesc
			);
			void InitRenderViewContext
			(
				ViewContext** const context,
				UINT width,
				UINT height,
				bool EnableDepthStencil,
				DXGI_SAMPLE_DESC sampleDesc
			);
			void InitRasterizerState
			(
				ID3D11RasterizerState** const rasterizerState,
				const D3D11_FILL_MODE& fillMode = D3D11_FILL_SOLID,
				const D3D11_CULL_MODE& cullMode = D3D11_CULL_BACK,
				const BOOL& multisampleEnable = false,
				const BOOL& antialisedEnable = false,
				const BOOL& depthClipEnable = false
			);
		};

		static DXRenderer* Get() { return DXRenderer::Get(); }
		static RENDERER_DESC CreateDefaultInitDesc()
		{
			RASTERIZER_DESC rasterDesc;
			ZeroMemory(&rasterDesc, sizeof(RASTERIZER_DESC));
			rasterDesc.FillMode = D3D11_FILL_SOLID;
			rasterDesc.CullMode = D3D11_CULL_BACK;

			RENDERER_DESC renderingDesc;
			ZeroMemory(&renderingDesc, sizeof(RENDERER_DESC));
			renderingDesc.RefreshRate = { 60, 1 };
			renderingDesc.MultisampleDesc = { 1, 0 };
			renderingDesc.FillMode = D3D11_FILL_SOLID;
			renderingDesc.CullMode = D3D11_CULL_BACK;
			renderingDesc.IsDebuggable = true;

			return renderingDesc;
		}
	}
}
