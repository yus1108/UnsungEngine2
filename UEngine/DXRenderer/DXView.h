#pragma once

namespace UEngine
{
	namespace DXRenderer
	{
		class DXView final
		{
		private:
			DXView() {}
			~DXView() = default;
		private:
			ViewContext context;

		public:
			static DXView* const Instantiate
			(
				DXRenderer* renderer,
				UINT width,
				UINT height,
				bool EnableDepthStencil,
				DXGI_SAMPLE_DESC sampleDesc
			);
			static void Release(DXView** const view);

			ID3D11DeviceContext* const GetDeviceContext() { return context.DeviceContext.Get(); }
			ID3D11ShaderResourceView* const GetViewResource() { return context.OutputShaderResourceView.Get(); }
			ID3D11ShaderResourceView* const* const GetAddressOfViewResource() { return context.OutputShaderResourceView.GetAddressOf(); }

			void Begin();
			void End();
			void Execute(ID3D11DeviceContext* deviceContext);
		};
	}
}
