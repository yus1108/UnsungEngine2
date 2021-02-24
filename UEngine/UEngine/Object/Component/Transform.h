#pragma once

namespace UEngine
{
	class Transform final : public IComponent
	{
	private:
		DXRenderer::DXConstantBuffer* worldBuffer;
		Vector3 worldPosition{ 0, 0, 0 };
		CPU_WORLD world;
	public:
		Vector3 localPosition{ 0, 0, 0 };
		Vector3 localRotation{ 0, 0, 0 };
		Vector3 scale{ 1, 1, 1 };
	public:
		Transform();
		~Transform() { GameState::Get()->constantBufferPool.Remove(worldBuffer); }
		virtual void Update() override;
	};
}
