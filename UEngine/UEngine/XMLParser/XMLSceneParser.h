#pragma once

class XMLSceneParser
{
public:
	void SaveScene(std::string name, bool isDebugMode, std::list<UEngine::GameObject*> gameObjects);
	UEngine::GameScene* LoadScene(std::string name, bool editorMode);
	
private:
	std::string name;
	TiXmlDocument doc;
	TiXmlElement* scene = nullptr;
	bool editorMode = false;

	void LoadGameObject(TiXmlNode* goNode, UEngine::GameScene* gameScene);
	void LoadGameObject(TiXmlNode* parentNode, TiXmlNode* goNode, UEngine::GameObject* parent);
	void LoadComponent(TiXmlNode* componentListNode, UEngine::GameObject* gameObject);
	UEngine::Component* CreateComponent(std::string componentType, UEngine::GameObject* gameObject);
	void SaveGameObject
	(
		TiXmlElement* node,
		std::string name,
		bool isActive,
		bool isStatic,
		std::vector<UEngine::GameObject*> children,
		std::map<std::string, std::list<UEngine::Component*>*> components
	);
};