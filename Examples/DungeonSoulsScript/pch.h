// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"

#include "./UEngine/UEngine.h"
#include "./SingletonManager/SingletonManager.h"
#include "./AudioEffect/AudioEffects.h"

#pragma comment(lib, "./SingletonManager.lib")
#pragma comment(lib, "./Utility.lib")
#pragma comment(lib, "./WinApplication.lib")
#pragma comment(lib, "./DXRenderer.lib")
#pragma comment(lib, "./UEngine.lib")
#pragma comment(lib, "./XMLSerializer.lib")
#pragma comment(lib, "./ImGui.lib")
#pragma comment(lib, "./AudioEffect.lib")

using namespace UEngine;
using namespace std;

#include "SoundManager.h"
#include "Animation.h"
#include "Dash.h"
#include "Player.h"
#include "Enemy.h"
#include "Weapon.h"
#include "Health.h"
#include "Skeleton.h"
#include "Boss.h"
#include "Projectile.h"

#endif //PCH_H
