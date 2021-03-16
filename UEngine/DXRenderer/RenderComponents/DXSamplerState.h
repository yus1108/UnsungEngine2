#pragma once

#pragma once

namespace UEngine
{
	namespace DXRenderer
	{
		class DXSamplerState final
		{
		private:
			std::string fileName;
			Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;

		private:
			DXSamplerState() = default;
			~DXSamplerState() = default;

		public:
			ID3D11SamplerState* const* const GetSamplerStateAddressOf() { return samplerState.GetAddressOf(); }
			ID3D11SamplerState* const GetSamplerState() { return samplerState.Get(); }

			void Set(ID3D11DeviceContext* const deviceContext, UINT startSlot);

			static DXSamplerState* Load(D3D11_SAMPLER_DESC desc);
			static std::string MakeName(D3D11_SAMPLER_DESC desc);
			static void Release(DXSamplerState* const texture);
		};
	}
}
