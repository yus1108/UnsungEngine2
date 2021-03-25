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
#pragma comment(lib, "./Libraries/SingletonManager.lib")
#pragma comment(lib, "./Libraries/Utility.lib")
#pragma comment(lib, "./Libraries/WinApplication.lib")
#pragma comment(lib, "./Libraries/DXRenderer.lib")
#pragma comment(lib, "./Libraries/UEngine.lib")
#pragma comment(lib, "./Libraries/XMLSerializer.lib")
#pragma comment(lib, "./Libraries/ImGui.lib")

#endif

using namespace UEngine;

#include "Game.h"