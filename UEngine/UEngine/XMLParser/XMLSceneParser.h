#pragma once

class XMLSceneParser
{
public:
	void SaveScene(std::string name, bool isDebugMode, std::list<UEngine::GameObject*> gameObjects);
	UEngine::GameScene* LoadScene(std::string name);
	
private:
	std::string name;
	TiXmlDocument doc;
	TiXmlElement* scene = nullptr;

	void LoadGameObject(TiXmlElement* sceneNode, UEngine::GameScene* gameScene);
	void LoadComponent(TiXmlElement* goNode, UEngine::GameObject* gameObject);
	void SaveGameObject
	(
		TiXmlElement* node,
		std::string name,
		bool isActive,
		bool isStatic,
		std::vector<UEngine::GameObject*> children,
		std::map<std::string, std::list<UEngine::Component*>*> components
	);
	void SaveComponent
	(
		TiXmlElement* node,
		bool enabled
	);
};