#pragma once

//디버그,문자집합 매크로
#if defined UNICODE && defined _DEBUG
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 
#elif defined _MBCS && defined _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

namespace UEngine
{
	namespace WinConsole
	{
        static void ResetMousePos()
        {
            #include <Windows.h>
            HANDLE hOut;
            COORD Position;

            hOut = GetStdHandle(STD_OUTPUT_HANDLE);

            Position.X = 0;
            Position.Y = 0;
            SetConsoleCursorPosition(hOut, Position);
        }
	}
}