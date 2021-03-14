#pragma once

#ifdef USER_DEFINED_SCRIPTS_EXPORTS  
#define UENGINE_CLASS __declspec(dllexport)
#else  
#define UENGINE_CLASS __declspec(dllimport)
#endif  

extern "C" UENGINE_CLASS UEngine::Component * ScriptCreation(UEngine::GameObject * gameObject);

