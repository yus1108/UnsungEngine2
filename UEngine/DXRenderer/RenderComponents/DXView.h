#pragma once

namespace UEngine
{
	namespace DXRenderer
	{
		class DXView final
		{
		private:
			DXView() { UID = std::to_string(nextId++); }
			~DXView() = default;
		private:
			static long long nextId;
			ViewContext context;

		public:
			std::string UID;
			static DXView* const Instantiate
			(
				DXRenderer* renderer,
				UINT width,
				UINT height,
				bool EnableDepthStencil,
				DXGI_SAMPLE_DESC sampleDesc
			);
			static void Release(DXView* const view);
			static void Release(DXView** const view);

			ViewContext* const GetContext() { return &context; }
			ID3D11DeviceContext* const GetDeviceContext() { return context.DeviceContext.Get(); }
			ID3D11ShaderResourceView* const GetViewResource() { return context.OutputShaderResourceView.Get(); }
			ID3D11ShaderResourceView** const GetAddressOfViewResource() { return context.OutputShaderResourceView.GetAddressOf(); }

			void Begin();
			void End();
			void Execute(ID3D11DeviceContext* deviceContext);
		};
	}
}
