#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>

#ifdef USER_DEFINED_SCRIPTS_EXPORTS  

#define UENGINE_CLASS(T) \
class T;\
extern "C" __declspec(dllexport) UEngine::Component* T##Creation(UEngine::GameObject* gameObject) \
{\
 return (UEngine::Component*)gameObject->AddComponent<T>(); \
}\
class T : public UEngine::Component

#define UENGINE_API extern "C" __declspec(dllexport)

#else  

#define UENGINE_CLASS(T) \
class T;\
extern "C" __declspec(dllimport) UEngine::Component* T##Creation(UEngine::GameObject* gameObject); \
class T : public UEngine::Component

#define UENGINE_API extern "C" __declspec(dllimport)

#endif  