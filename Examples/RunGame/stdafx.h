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

#include "./UEngine/UEngine.h"
#include "./WinApplication/WinConsole.h"
#include "./WinApplication/WinMemoryLeak.h"

#pragma comment(lib, "./SingletonManager.lib")
#pragma comment(lib, "./Utility.lib")
#pragma comment(lib, "./WinApplication.lib")
#pragma comment(lib, "./DXRenderer.lib")
#pragma comment(lib, "./UEngine.lib")
#pragma comment(lib, "./XMLSerializer.lib")
#pragma comment(lib, "./ImGui.lib")

using namespace UEngine;

#include "Scene.h"
#include "Background.h"
#include "Tile.h"
#include "Player.h"