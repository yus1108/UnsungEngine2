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

#include "./UEngine/UEngine.h"
#include "./SingletonManager/SingletonManager.h"
#include "./WinApplication/WinMemoryLeak.h"

#pragma comment(lib, "./SingletonManager.lib")
#pragma comment(lib, "./Utility.lib")
#pragma comment(lib, "./WinApplication.lib")
#pragma comment(lib, "./DXRenderer.lib")
#pragma comment(lib, "./UEngine.lib")
#pragma comment(lib, "./XMLSerializer.lib")
#pragma comment(lib, "./ImGui.lib")

using namespace UEngine;

#include "Game.h"