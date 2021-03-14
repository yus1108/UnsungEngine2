#pragma once
#include "../Utility/UtilityDefinitions.h"

namespace UEngine
{
	class WinInput final
	{
	public:
#pragma region Singleton
	public:
		static WinInput* Get() { if (instance == nullptr) instance = new WinInput; return instance; }
		static void Attach(WinInput* input) { instance = input; }
		static void Detach() { instance = nullptr; }
		static void Release() { delete instance; Detach(); }

	private:
		WinInput() = default;
		~WinInput() = default;
		static WinInput* instance;

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