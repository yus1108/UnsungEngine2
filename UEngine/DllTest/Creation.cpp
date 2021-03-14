#include "pch.h"
#include "Creation.h"

UENGINE_CLASS UEngine::Component* ScriptCreation(UEngine::GameObject* gameObject)
{
	return (UEngine::Component*)gameObject->AddComponent<Script>();
}
