#pragma once

UENGINE_API void ATTACH_SINGLETONS(UEngine::SingletonManager::Singletons singletons);
UENGINE_API void INIT_GAMESTATE();
UENGINE_API void DeSerializeComponent(UEngine::Component* component, TiXmlNode* memberVarNode);
UENGINE_API void SetEnable(UEngine::Component* component, bool enabled);