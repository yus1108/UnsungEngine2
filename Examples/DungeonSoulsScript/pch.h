// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"

#include "../../UEngine/UEngine/UEngine.h"
#include "../../UEngine/SingletonManager/SingletonManager.h"

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
using namespace std;

#include "Animation.h"
#include "Player.h"
#include "Enemy.h"
#include "Weapon.h"
#include "Health.h"
#include "Skeleton.h"

#endif //PCH_H
