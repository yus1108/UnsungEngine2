#pragma once

namespace UEngine
{
	class Transform final : public IComponent
	{
	private:
		Vector3 worldPosition{ 0, 0, 0 };
	protected:
		Vector3 localPosition{ 0, 0, 0 };
		Vector3 position{ 0, 0, 0 };
	};
}
