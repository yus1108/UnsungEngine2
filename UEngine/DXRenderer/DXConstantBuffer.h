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
		UINT StartSlot{ 0 };
		UINT Flags{ 0 };

		static DXConstantBuffer* const Instantiate
		(
			DXRenderer* const renderer, 
			size_t _Data_Size,
			UINT flags = UENGINE_DXSHADERTYPE_UNKNOWN,
			UINT startSlot = 0
		);
		static void Release(DXConstantBuffer** const constantBuffer);

		ID3D11Buffer* const GetBuffer() { return constBuffer.Get(); }
		ID3D11Buffer* const* const GetBufferAddressOf() { return constBuffer.GetAddressOf(); }

		void UpdateBuffer(ID3D11DeviceContext* const deviceContext, const void* data, size_t _Size);
		void Set(ID3D11DeviceContext* const deviceContext);
	};
}