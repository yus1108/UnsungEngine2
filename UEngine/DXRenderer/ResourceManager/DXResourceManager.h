#pragma once

namespace UEngine
{
	namespace DXRenderer
	{
		class DXResourceManager
		{
		private:
			std::map<std::string, std::unordered_map<std::wstring, void*>> creationQueue, deletionQueue, resources;
			std::map<std::string, CONSTANT_BUFFER_DESC> cBufferPreset;

			void InitShaders();
			void InitMeshes();
			void InitCBuffers();

		public:
			std::string TYPE_VIEW = typeid(UEngine::DXRenderer::DXView).raw_name();
			std::string TYPE_TEXTURE = typeid(UEngine::DXRenderer::DXTexture).raw_name();
			std::string TYPE_SHADER = typeid(UEngine::DXRenderer::DXShader).raw_name();
			std::string TYPE_RENDERMESH = typeid(UEngine::DXRenderer::DXRenderMesh).raw_name();
			std::string TYPE_CONSTANT_BUFFER = typeid(UEngine::DXRenderer::DXConstantBuffer).raw_name();

			DXResourceManager() = default;
			~DXResourceManager() { Release(); }

			template <typename T>
			void AddResource(std::wstring name, T* resource);
			template <typename T>
			void RemoveResource(std::wstring name);
			template <typename T>
			T* GetResource(std::wstring name);

			const CONSTANT_BUFFER_DESC& GetCBufferPreset(std::string typeName) { return cBufferPreset[typeName]; }

			void ApplyChange();
			void Init();
			void Release();
		};

		template<typename T>
		inline void DXResourceManager::AddResource(std::wstring name, T* resource)
		{
			std::string typeName = typeid(T).raw_name();

			if (!std::is_same<T, UEngine::DXRenderer::DXView>::value)
				throw std::runtime_error("Invalid resource type");
			if (!std::is_same<T, UEngine::DXRenderer::DXTexture>::value)
				throw std::runtime_error("Invalid resource type");
			if (!std::is_same<T, UEngine::DXRenderer::DXShader>::value)
				throw std::runtime_error("Invalid resource type");
			if (!std::is_same<T, UEngine::DXRenderer::DXRenderMesh>::value)
				throw std::runtime_error("Invalid resource type");
			if (!std::is_same<T, UEngine::DXRenderer::DXConstantBuffer>::value)
				throw std::runtime_error("Invalid resource type");


			if (creationQueue[typeName].size() > 0 &&
				creationQueue[typeName].find(name) != creationQueue[typeName].end())
				throw std::runtime_error("A resource with the given name is already queued");
			creationQueue[typeName][name] = resource;
		}

		template<typename T>
		inline void DXResourceManager::RemoveResource(std::wstring name)
		{
			std::string typeName = typeid(T).raw_name();

			if (!std::is_same<T, UEngine::DXRenderer::DXView>::value)
				throw std::runtime_error("Invalid resource type");
			if (!std::is_same<T, UEngine::DXRenderer::DXTexture>::value)
				throw std::runtime_error("Invalid resource type");
			if (!std::is_same<T, UEngine::DXRenderer::DXShader>::value)
				throw std::runtime_error("Invalid resource type");
			if (!std::is_same<T, UEngine::DXRenderer::DXRenderMesh>::value)
				throw std::runtime_error("Invalid resource type");
			if (!std::is_same<T, UEngine::DXRenderer::DXConstantBuffer>::value)
				throw std::runtime_error("Invalid resource type");

			if (deletionQueue[typeName].size() > 0 &&
				deletionQueue[typeName].find(name) != deletionQueue[typeName].end())
				throw std::runtime_error("A resource with the given name already queued");

			if (resources[typeName].size() == 0 &&
				resources[typeName].find(name) == resources[typeName].end())
				throw std::runtime_error("A resource with the given name doesn't exists");

			deletionQueue[typeName][name] = resources[typeName][name];
		}

		template<typename T>
		T* DXResourceManager::GetResource(std::wstring name)
		{
			std::string typeName = typeid(T).raw_name();

			if (typeName != TYPE_VIEW &&
				typeName != TYPE_TEXTURE &&
				typeName != TYPE_SHADER &&
				typeName != TYPE_RENDERMESH &&
				typeName != TYPE_CONSTANT_BUFFER)
				throw std::runtime_error("A resource with the given type doesn't exists");

			return static_cast<T*>(resources[typeName][name]);
		}
	}
}