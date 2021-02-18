#pragma once
#include "dxrframework.h"

namespace UEngine
{
	class DXConstantBuffer final
	{
	private:
		DXConstantBuffer() {}
		~DXConstantBuffer() = default;
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> constBuffer;

	public:
		// Initialize to Draw line strip with only vertices
		static DXConstantBuffer* const Instantiate(DXRenderer* const renderer, size_t _Data_Size);
		static void Release(DXConstantBuffer** const constantBuffer);

		void UpdateBuffer(ID3D11DeviceContext* const deviceContext, const void* data, size_t _Size);
		ID3D11Buffer* const GetBuffer() { return constBuffer.Get(); }
		ID3D11Buffer* const * const GetBufferAddressOf() { return constBuffer.GetAddressOf(); }
	};
}