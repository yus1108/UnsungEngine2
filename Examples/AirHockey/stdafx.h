#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <iostream>

//유니코드 사용
#ifdef _UNICODE
#define _tstring wstring
#define _tcout   wcout
#else //멀티바이트시 사용
#define _tstring string
#define _tcout   cout
#endif

#include "../../UEngine/UEngine/UEngine.h"
#include "../../UEngine/WinApplication/WinConsole.h"
#include "../../UEngine/WinApplication/WinMemoryLeak.h"

#ifdef _DEBUG
#pragma comment(lib, "../../UEngine/Debug/Utility.lib")
#pragma comment(lib, "../../UEngine/Debug/WinApplication.lib")
#pragma comment(lib, "../../UEngine/Debug/DXRenderer.lib")
#pragma comment(lib, "../../UEngine/Debug/UEngine.lib")
#endif

using namespace UEngine;

#include "Player.h"
#include "Player1.h"
#include "Player2.h"
#include "Ball.h"
#include "GoalPost.h"
#include "Scene.h"