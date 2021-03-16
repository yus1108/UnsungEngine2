#pragma once

namespace UEngine
{
	namespace DXRenderer
	{
		class DXTexture final
		{
		private:
			std::wstring fileName;
			Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shader_resource_view;
			Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;

		private:
			DXTexture() = default;
			~DXTexture() = default;

		public:
			ID3D11ShaderResourceView* const* const GetTextureAddressOf() { return shader_resource_view.GetAddressOf(); }
			ID3D11ShaderResourceView* const GetTexture() { return shader_resource_view.Get(); }

			void Set(ID3D11DeviceContext* const deviceContext, UINT startSlot);

			static DXTexture* Load(std::wstring fileName);
			static void Release(DXTexture* const texture);
		};
	}
}
