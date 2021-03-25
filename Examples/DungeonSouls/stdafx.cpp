#include "stdafx.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    UEngine::WinMemoryLeak::Detect();

    Game game(hInstance, 1280, 720);
    game.Load();
    int result = game.Run(200);

    return result;
}