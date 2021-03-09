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

#include "../UEngine/UEngine.h"
#include "../WinApplication/WinConsole.h"
#include "..//WinApplication/WinMemoryLeak.h"

using namespace UEngine;

#include "Scene.h"
#include "Background.h"
#include "Tile.h"
#include "Player.h"