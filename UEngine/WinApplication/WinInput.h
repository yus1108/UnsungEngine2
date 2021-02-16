#pragma once
#include "../Utility/Definition.h"

namespace UEngine
{
	class WinInput final
	{
	public:
		enum class MouseType {
			LEFT,
			RIGHT,
			MIDDLE,
			SCROLL,
			COUNT
		};
#pragma region Singleton
	public:
		static WinInput* Get() { return &instance; }

	private:
		WinInput() = default;
		~WinInput() = default;
		static WinInput instance;
#pragma endregion

	private:
		bool keys[256] = { false };
		bool mouse[3] = { false };
		unsigned counter[256] = { 0 };
		unsigned mouseCounter[3] = { 0 };
		UEngine::Definition::Coordinate2D currMousePos;

	public:
		UEngine::Definition::Coordinate2D prevMousePos;
		int scroll;
		bool LeftButtonClicked = false;
		bool RightButtonClicked = false;

		void SetKeyPress(bool _keypressed, unsigned _wParam);
		void SetMousePress(bool _keypressed, MouseType _wParam);
		void SetMousePos(UEngine::Definition::Coordinate2D mousePos);

		const bool& GetKey(const unsigned& _key) const { return keys[_key]; }// check the state key is being pressed
		const bool& GetMouseInput(const unsigned& _mouseInput) const { return mouse[_mouseInput]; }
		const UEngine::Definition::Coordinate2D& GetMousePos() const { return currMousePos; }
		const bool& GetKeyPress(const unsigned& _key); // check the state key pressed for one time while pressed down
		const bool& GetMousePress(const unsigned& _key); // check the state key pressed for one time while pressed down


	};
	
}


