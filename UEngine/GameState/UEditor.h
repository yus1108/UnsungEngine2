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

#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_win32.h"
#include "ImGui\imgui_impl_dx11.h"

//유니코드 사용
#ifdef _UNICODE
#define _tstring wstring
#define _tcout   wcout
#else //멀티바이트시 사용
#define _tstring string
#define _tcout   cout
#endif

#include "../UEngine/UEngine.h"
#include "../WinApplication/WinConsole.h"
#include "..//WinApplication/WinMemoryLeak.h"
#include "../SingletonManager/SingletonManager.h"

#include "Editor\EditorState.h"