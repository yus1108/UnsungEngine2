#include "WinInput.h"

UEngine::WinInput UEngine::WinInput::instance;

void UEngine::WinInput::SetKeyPress(bool _keypressed, unsigned _wParam)
{
	keys[_wParam] = _keypressed;
}

void UEngine::WinInput::SetMousePress(bool _keypressed, MouseType _wParam)
{
	mouse[static_cast<unsigned>(_wParam)] = _keypressed;
}

void UEngine::WinInput::SetMousePos(UEngine::Definition::Coordinate2D mousePos)
{
	prevMousePos = currMousePos;
	currMousePos = mousePos;
}

const bool& UEngine::WinInput::GetKeyPress(const unsigned& _key)
{
	if (keys[_key])
	{
		counter[_key]++;
		if (counter[_key] == 1)
			return true;
		else if (counter[_key] == 0xffffffff) // UINT_MAX
			counter[_key] = 2;
	}
	else {
		counter[_key] = 0;
	}
	return false;
}

const bool& UEngine::WinInput::GetMousePress(const unsigned& _mouseInput)
{
	if (mouse[_mouseInput])
	{
		mouseCounter[_mouseInput]++;
		if (mouseCounter[_mouseInput] == 1)
			return true;
		else if (mouseCounter[_mouseInput] == 0xffffffff) // UINT_MAX
			mouseCounter[_mouseInput] = 2;
	}
	else {
		mouseCounter[_mouseInput] = 0;
	}
	return false;
}
