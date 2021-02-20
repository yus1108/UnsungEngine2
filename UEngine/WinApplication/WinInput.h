#pragma once
#include "../Utility/UtilityDefinitions.h"

namespace UEngine
{
	class WinInput final
	{
	public:
#pragma region Singleton
	public:
		static WinInput* Get() { return &instance; }

	private:
		WinInput() = default;
		~WinInput() = default;
		static WinInput instance;
#pragma endregion

	private:
		HWND handler;
		bool keys[256] = { false };
		BYTE PrevKey = 0;               //전에 누르고있던 키

	public:
		void AttachHandler(HWND hwnd) { this->handler = hwnd; }

		const bool GetKeyDown(const unsigned& _key);
		const bool GetKey(const unsigned& _key);
		const bool GetKeyUp(const unsigned& _key);
		const UEngine::Utility::Coordinate2D GetMousePos() const;
	};
	
}