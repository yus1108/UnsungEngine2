#pragma once
#include "dxrframework.h"

namespace UEngine
{
	namespace DXRenderer
	{
		class DXConstantBuffer final
		{
		private:
			DXConstantBuffer() {}
			~DXConstantBuffer() { if (!attached) delete data; };
		private:
			Microsoft::WRL::ComPtr<ID3D11Buffer> constBuffer;
			void* data{ nullptr };
			size_t _Size{ 0 };
			bool attached{ false };

		public:
			UINT StartSlot{ 0 };
			UINT Flags{ 0 };

			static DXConstantBuffer* Instantiate
			(
				DXRenderer* const renderer, 
				size_t _Data_Size,
				UINT flags = UENGINE_DXSHADERTYPE_UNKNOWN,
				UINT startSlot = 0
			);
			static void Release(DXConstantBuffer** const constantBuffer);

			ID3D11Buffer* GetBuffer() { return constBuffer.Get(); }
			ID3D11Buffer* const * GetBufferAddressOf() { return constBuffer.GetAddressOf(); }
			size_t const GetDataSize() { return _Size; }

			template <typename T>
			T* GetData() { return data; }
			template <typename T>
			void CopyData(const T* data, size_t _Size);
			void AttachData(const void* data, size_t _Size);

			void Update(ID3D11DeviceContext* const deviceContext);
			void Set(ID3D11DeviceContext* const deviceContext);
		};

		template <typename T>
		void DXConstantBuffer::CopyData(const T* data, size_t _Size)
		{
			if (!attached) delete this->data;
			this->data = new T;
			this->_Size = _Size;
			memcpy(this->data, data, _Size);
			attached = false;
		}
	}
}