#pragma once

namespace UEngine
{
	class DXRenderObject final
	{
	private:
		DXRenderObject() {}
		~DXRenderObject() = default;
	private:
		class DXRenderMesh* renderMesh{ nullptr };
		class DXShader* shader{ nullptr };
		std::unordered_map<std::string, class DXConstantBuffer*> constantBuffers;

	public:
		bool AllowGCforSubComponents{ false };

		static DXRenderObject* const Instantiate
		(
			DXRenderMesh* renderMesh,
			DXShader* shader,
			bool allowGCforSubComponents = false
		);
		static void Release(DXRenderObject** const renderObject);

		DXRenderMesh* const GetDXRenderMesh() { return renderMesh; }
		DXShader* const GetDXShader() { return shader; }
		DXConstantBuffer* const GetConstantBuffer(const std::string bufferName) { return constantBuffers[bufferName]; }
		std::unordered_map<std::string, DXConstantBuffer*> const GetConstantBuffers() { return constantBuffers; }

		void AddConstantBuffer
		(
			DXRenderer* const renderer,
			const std::string bufferName,
			size_t _Data_Size,
			UINT flags,
			UINT startSlot = UENGINE_DXSHADERTYPE_UNKNOWN
		);
		void AddConstantBuffer(const std::string bufferName, DXConstantBuffer * const constantBuffer);
		
		size_t CBGetDataSize(const std::string bufferName) { return constantBuffers[bufferName]->GetDataSize(); }
		template <typename T>
		T* const CBGetData(const std::string bufferName) { return constantBuffers[bufferName]->GetData<T>(); }
		template <typename T>
		void CBCopyData(const std::string bufferName, const T* data, size_t _Size);
		void CBAttachData(const std::string bufferName, const void* data, size_t _Size);

		void CBUpdate(ID3D11DeviceContext* const deviceContext, const std::string bufferName);
		void CBUpdateAll(ID3D11DeviceContext* const deviceContext);

		void Set(ID3D11DeviceContext* const deviceContext);
		void Draw(ID3D11DeviceContext* const deviceContext);
	};

	template <typename T>
	void DXRenderObject::CBCopyData(const std::string bufferName, const T* data, size_t _Size)
	{
		constantBuffers[bufferName]->CopyData<T>(data, _Size);
	}
}

