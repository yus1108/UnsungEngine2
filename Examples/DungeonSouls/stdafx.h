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

#include "../../UEngine/UEngine/UEngine.h"
#include "../../UEngine/SingletonManager/SingletonManager.h"
#include "../../UEngine/WinApplication/WinMemoryLeak.h"

#ifdef _DEBUG
#pragma comment(lib, "../../UEngine/Debug/SingletonManager.lib")
#pragma comment(lib, "../../UEngine/Debug/Utility.lib")
#pragma comment(lib, "../../UEngine/Debug/WinApplication.lib")
#pragma comment(lib, "../../UEngine/Debug/DXRenderer.lib")
#pragma comment(lib, "../../UEngine/Debug/UEngine.lib")
#pragma comment(lib, "../../UEngine/Debug/XMLSerializer.lib")
#pragma comment(lib, "../../UEngine/Debug/ImGui.lib")

#else
#pragma comment(lib, "../../UEngine/Release/SingletonManager.lib")
#pragma comment(lib, "../../UEngine/Release/Utility.lib")
#pragma comment(lib, "../../UEngine/Release/WinApplication.lib")
#pragma comment(lib, "../../UEngine/Release/DXRenderer.lib")
#pragma comment(lib, "../../UEngine/Release/UEngine.lib")
#pragma comment(lib, "../../UEngine/Release/XMLSerializer.lib")
#pragma comment(lib, "../../UEngine/Release/ImGui.lib")

#endif

using namespace UEngine;

#include "Game.h"