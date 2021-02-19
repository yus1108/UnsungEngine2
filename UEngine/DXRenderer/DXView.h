#pragma once

namespace UEngine
{
	class DXView final
	{
	private:
		DXView() {}
		~DXView() = default;
	private:
		UEngine::DXRenderViewContext context;

		std::vector<DXRenderObject*> renderObjectList;

	public:
		static DXView* const Instantiate
		(
			DXRenderer* renderer,
			UINT width, 
			UINT height
		);
		static void Release(DXView** const view);

		ID3D11ShaderResourceView* const GetViewResource() { return context.OutputShaderResourceView.Get(); }
		ID3D11ShaderResourceView* const * const GetAddressOfViewResource() { return context.OutputShaderResourceView.GetAddressOf(); }

		void AddRenderObject(DXRenderObject* const renderObject);
		void Set();
		void Begin();
		void End(ID3D11DeviceContext* deviceContext);
	};
}
