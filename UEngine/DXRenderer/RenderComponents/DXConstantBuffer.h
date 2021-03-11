#pragma once
#include "dxrframework.h"

namespace UEngine
{
	namespace DXRenderer
	{
		class DXConstantBuffer final
		{
		private:
			DXConstantBuffer() { UID = nextId++; }
			~DXConstantBuffer() { if (!attached) delete data; };
		private:
			static long long nextId;
			Microsoft::WRL::ComPtr<ID3D11Buffer> constBuffer;
			void* data{ nullptr };
			size_t Size{ 0 };
			bool attached{ false };

		public:
			std::wstring UID;
			/*
				Start slot of each pipeline in order of UENGINE_DXRENDERER_SHADERTYPE
				i.e) if Flag = UENGINE_DXRENDERER_SHADERTYPE_VERTEX_SHADER | UENGINE_DXRENDERER_SHADERTYPE_GEOMETRY_SHADER,
				StartSlots = ARRAY{ slot_of_vertex_shader, slot_of_geometry_shader }
			*/
			UINT* StartSlots{ 0 };
			UINT Flags{ 0 }; // UENGINE_DXRENDERER_SHADERTYPE

			static DXConstantBuffer* Instantiate
			(
				DXRenderer* const renderer, 
				CONSTANT_BUFFER_DESC desc
			);
			static void Release(DXConstantBuffer** const constantBuffer);
			static void Release(DXConstantBuffer* const constantBuffer);

			ID3D11Buffer* GetBuffer() { return constBuffer.Get(); }
			ID3D11Buffer* const * GetBufferAddressOf() { return constBuffer.GetAddressOf(); }
			size_t const GetSize() { return Size; }

			template <typename T>
			T* GetData() { return data; }
			const void* GetData() { return data; }
			template <typename T>
			void CopyData(const T* data, size_t Size);
			void AttachData(const void* data, size_t Size);

			void Update(ID3D11DeviceContext* const deviceContext);
			void Set(ID3D11DeviceContext* const deviceContext);
		};

		template <typename T>
		void DXConstantBuffer::CopyData(const T* data, size_t Size)
		{
			if (!attached) delete this->data;
			this->data = new T;
			this->Size = Size;
			memcpy(this->data, data, Size);
			attached = false;
		}
	}
}