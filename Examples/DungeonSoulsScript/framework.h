#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>

#ifdef USER_DEFINED_SCRIPTS_EXPORTS  

#define UENGINE_CLASS(CLASS_NAME) \
class CLASS_NAME;\
extern "C" __declspec(dllexport) UEngine::Component* CLASS_NAME##Creation(UEngine::GameObject* gameObject) \
{\
 return (UEngine::Component*)gameObject->AddComponent<CLASS_NAME>(); \
}\
class CLASS_NAME : public UEngine::Component

#define UENGINE_CLASS_INHERIT(CLASS_NAME, CLASS_CHILD_OF_COMPONENT) \
class CLASS_NAME;\
extern "C" __declspec(dllexport) UEngine::Component* CLASS_NAME##Creation(UEngine::GameObject* gameObject) \
{\
 return (UEngine::Component*)gameObject->AddComponent<CLASS_NAME>(); \
}\
class CLASS_NAME : public CLASS_CHILD_OF_COMPONENT

#define UENGINE_API extern "C" __declspec(dllexport)

#else  

#define UENGINE_CLASS(CLASS_NAME) \
class CLASS_NAME;\
extern "C" __declspec(dllimport) UEngine::Component* CLASS_NAME##Creation(UEngine::GameObject* gameObject); \
class CLASS_NAME : public UEngine::Component

#define UENGINE_CLASS_INHERIT(CLASS_NAME, CLASS_CHILD_OF_COMPONENT) \
class CLASS_NAME;\
extern "C" __declspec(dllimport) UEngine::Component* CLASS_NAME##Creation(UEngine::GameObject* gameObject); \
class CLASS_NAME : public CLASS_CHILD_OF_COMPONENT

#define UENGINE_API extern "C" __declspec(dllimport)

#endif  

