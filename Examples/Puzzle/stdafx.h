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

//�����ڵ� ���
#ifdef _UNICODE
#define _tstring wstring
#define _tcout   wcout
#else //��Ƽ����Ʈ�� ���
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

#include "Scene.h"
#include "Puzzle.h"