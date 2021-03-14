#include "UEngine.h"
#include "XMLSceneParser.h"

void XMLSceneParser::SaveScene(std::string name, bool isDebugMode, std::list<UEngine::GameObject*> gameObjects)
{
	TiXmlDeclaration* pDec1 = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(pDec1);

	//루트 노드 추가
	scene = new TiXmlElement("GameScene");
	doc.LinkEndChild(scene);

	this->name = name + ".uscene";
	scene->SetAttribute("name", name.c_str());
	scene->SetAttribute("isDebugMode", isDebugMode);
	for (auto gameObject : gameObjects)
	{
		if (gameObject->GetParent() != nullptr) continue;
		SaveGameObject(scene, gameObject->name, gameObject->GetActive(), gameObject->IsStatic, gameObject->GetChildren(), gameObject->GetComponents());
	}
	doc.SaveFile(this->name.c_str());
}

UEngine::GameScene* XMLSceneParser::LoadScene(std::string name)
{
	doc.LoadFile(name.c_str());
	auto node = doc.FirstChild();
	node = node->NextSibling();
	if (node == nullptr) throw std::runtime_error("invalid scene file");

	UEngine::GameScene* currentScene = new UEngine::GameScene();

	bool isDebugMode;
	node->ToElement()->QueryBoolAttribute("isDebugMode", &isDebugMode);
	currentScene->Init(isDebugMode);
	currentScene->name = node->ToElement()->Attribute("name");

	LoadGameObject(node->ToElement(), currentScene);

	return currentScene;
}

void XMLSceneParser::LoadGameObject(TiXmlElement* sceneNode, UEngine::GameScene* gameScene)
{
	auto goNode = sceneNode->FirstChild();
	while (goNode)
	{
		std::string name;
		bool isActive, isStatic;
		name = goNode->ToElement()->Attribute("name");
		goNode->ToElement()->QueryBoolAttribute("isActive", &isActive);
		auto gameObject = UEngine::GameObject::Instantiate(gameScene, name);
		gameObject->SetActive(isActive);

		LoadComponent(goNode->ToElement(), gameObject);

		goNode->ToElement()->QueryBoolAttribute("isStatic", &gameObject->IsStatic);


		goNode = goNode->NextSibling();
	}
}

void XMLSceneParser::LoadComponent(TiXmlElement* goNode, UEngine::GameObject* gameObject)
{
	auto componentNode = goNode->FirstChild();
	while (componentNode)
	{
		std::string componentType = componentNode->ToElement()->Value();
		
		UEngine::Component* component = nullptr;
		if (componentType == "Transform")
		{
			component = gameObject->GetComponent<UEngine::Transform>();
		}
		else if (componentType == "RenderComponent")
		{
			component = gameObject->AddComponent<UEngine::RenderComponent>();
		}
		else if (componentType == "Material")
		{
			component = gameObject->AddComponent<UEngine::Material>();
		}
		else if (componentType == "Camera")
		{
			component = gameObject->AddComponent<UEngine::Camera>();
		}
		else if (componentType == "CircleCollider")
		{
			component = gameObject->AddComponent<UEngine::Physics2D::CircleCollider>();
		}
		else if (componentType == "RectCollider")
		{
			component = gameObject->AddComponent<UEngine::Physics2D::RectCollider>();
		}
		else
		{
			typedef UEngine::Component* (*AddScript) (UEngine::GameObject* gameObject);
			AddScript scriptCreation = NULL;
			//scriptCreation = (AddScript)GetProcAddress(UEngine::WinApplication, "ScriptCreation");

		}

		bool enabled;
		goNode->ToElement()->QueryBoolAttribute("enabled", &enabled);
		component->SetEnable(enabled);

		// deserialize

		componentNode = componentNode->NextSibling();
	}
}

void XMLSceneParser::SaveGameObject
(
	TiXmlElement* node,
	std::string name,
	bool isActive,
	bool isStatic, 
	std::vector<UEngine::GameObject*> children,
	std::map<std::string, std::list<UEngine::Component*>*> components)
{
	auto gameObject = new TiXmlElement("GameObject");
	gameObject->SetAttribute("name", name.c_str());
	gameObject->SetAttribute("isActive", isActive);
	gameObject->SetAttribute("isStatic", isStatic);
	for (auto componentList : components)
	{
		std::string raw_typeName = componentList.first;
		TiXmlString typeName = TiXmlString(raw_typeName.substr(4, raw_typeName.find_first_of('@') - 4).c_str());
		auto componentType = new TiXmlElement(typeName.c_str());
		for (auto component : *componentList.second)
		{
			auto xmlComponent = new TiXmlElement("Component");
			xmlComponent->SetAttribute("enabled", component->GetEnable());
			component->Serialize(xmlComponent);
			componentType->LinkEndChild(xmlComponent);
		}
		gameObject->LinkEndChild(componentType);
	}
	for (auto child : children)
		SaveGameObject(gameObject, child->name, child->GetActive(), child->IsStatic, child->GetChildren(), child->GetComponents());
	node->LinkEndChild(gameObject);
}

void XMLSceneParser::SaveComponent
(
	TiXmlElement* node,
	bool enabled
)
{
	auto component = new TiXmlElement("Component");
	component->SetAttribute("enabled", enabled);
	node->LinkEndChild(component);
}