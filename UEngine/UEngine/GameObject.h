#pragma once

namespace UEngine
{
	class GameObject final
	{
	private:
		GameObject();
		~GameObject();

	private:
		DXRenderer::DXRenderObject* const renderObject;

	public:
		Transform* const transform;
		std::vector<GameObject*> children;
		std::unordered_map<std::string, IComponent*> components;
		
	public:
		static GameObject* Instantiate();
	};
}

