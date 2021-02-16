// header.h : include file for standard system include files,
// or project specific include files
//

#pragma once

#include "targetver.h"
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