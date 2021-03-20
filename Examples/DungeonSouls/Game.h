#pragma once

class Game
{
private:
	POINT screenSize;
	WinApplication* app = nullptr;
	DXRenderer::DXRenderer* renderer = nullptr;
public:
	Game() = delete;
	Game(HINSTANCE hInstance, int width = 800, int height = 600);
	~Game();
	void Load();
	int Run(double targetHz = -1);
};

