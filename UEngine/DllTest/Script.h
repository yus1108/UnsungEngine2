#pragma once
#include "pch.h"

#ifdef USER_DEFINED_SCRIPTS_EXPORTS  
#define UENGINE_CLASS(T) \
class T;\
extern "C" __declspec(dllexport) UEngine::Component* T##Creation(UEngine::GameObject* gameObject) \
{\
 return (UEngine::Component*)gameObject->AddComponent<T>(); \
}\
class T : public UEngine::Component
#else  
#define UENGINE_CLASS(T) \
class T;\
extern "C" __declspec(dllimport) UEngine::Component* T##Creation(UEngine::GameObject* gameObject); \
class T : public UEngine::Component
#endif  

UENGINE_CLASS(Script)
{
public:
	void Update() override;
};

