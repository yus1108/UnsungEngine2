#include "UEngine.h"
#include "Input.h"

bool UEngine::Input::GetKey(const unsigned VK_KEY)
{
	bool isFocuesd = GameState::Get()->isFocused;
	if (isFocuesd &&
		UEngine::WinInput::Get()->GetKey(VK_KEY))
	{
		return true;
	}
	return false;
}

bool UEngine::Input::GetKeyDown(const unsigned VK_KEY)
{
	bool isFocuesd = GameState::Get()->isFocused;
	if (isFocuesd &&
		UEngine::WinInput::Get()->GetKeyDown(VK_KEY))
	{
		return true;
	}
	return false;
}

bool UEngine::Input::GetKeyUp(const unsigned VK_KEY)
{
	bool isFocuesd = GameState::Get()->isFocused;
	if (isFocuesd &&
		UEngine::WinInput::Get()->GetKeyUp(VK_KEY))
	{
		return true;
	}
	return false;
}

UEngine::Vector2 UEngine::Input::GetMousePos()
{
	RECT windowSize;
	auto state = GameState::Get();
	windowSize.left = state->startWindowPos.x;
	windowSize.right = state->windowSize.x;
	windowSize.top = state->startWindowPos.y;
	windowSize.bottom = state->windowSize.y;
	return Utility::UMath::ConvertPixelToNDC(WinInput::Get()->GetMousePos(), windowSize);
}

bool UEngine::Input::IsMouseInWindow(__out UEngine::Vector2& mousePos)
{
	Math::Physics2D::AABB windowSize;
	auto state = GameState::Get();
	windowSize.left = state->startWindowPos.x;
	windowSize.right = windowSize.left + state->windowSize.x;
	windowSize.bottom = state->startWindowPos.y;
	windowSize.top = windowSize.bottom + state->windowSize.y;
	mousePos = GetMousePos();

	Vector2 pixelPos = WinInput::Get()->GetMousePos();

	return Math::Physics2D::IsColliding(pixelPos, windowSize);
}
