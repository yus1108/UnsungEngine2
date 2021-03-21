#pragma once

namespace UEngine
{
	class Input
	{
	public:
		static bool GetKey(const unsigned VK_KEY);
		static bool GetKeyDown(const unsigned VK_KEY);
		static bool GetKeyUp(const unsigned VK_KEY);
		static bool GetMouseDown(const unsigned VK_KEY);
		static UEngine::Vector2 GetMousePos();
		static bool IsMouseInWindow(__out UEngine::Vector2& mousePos);
	};
}