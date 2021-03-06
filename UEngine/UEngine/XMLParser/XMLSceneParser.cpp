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

	gameObjects.erase(gameObjects.begin());
	for (auto gameObject : gameObjects)
	{
		if (gameObject->GetParent() != nullptr) continue;
		SaveGameObject(scene, gameObject->name, gameObject->GetActive(), gameObject->IsStatic, gameObject->GetChildren(), *gameObject->GetComponents());
	}
	doc.SaveFile(this->name.c_str());
}

UEngine::GameScene* XMLSceneParser::LoadScene(std::string name, bool editorMode)
{
	this->editorMode = editorMode;
	doc.LoadFile(name.c_str());
	auto node = doc.FirstChild();
	node = node->NextSibling();
	if (node == nullptr) throw std::runtime_error("invalid scene file");

	UEngine::GameScene* currentScene = new UEngine::GameScene();

	currentScene->Init(editorMode);
	currentScene->name = node->ToElement()->Attribute("name");

	auto editorObj = UEngine::GameObject::Instantiate(currentScene, "editorCamera");
	if (editorMode) editorObj->AddComponent<UEngine::EditorScript>();
	auto editorCamera = editorObj->AddComponent<UEngine::Camera>();
	editorCamera->viewWidth.value = 1280;
	editorCamera->viewHeight.value = 720;
	currentScene->EditorCamera = editorCamera;

	LoadGameObject(node, currentScene);

	return currentScene;
}

void XMLSceneParser::LoadGameObject(TiXmlNode* parentNode, UEngine::GameScene* gameScene)
{
	auto goNode = parentNode->FirstChild();
	while (goNode)
	{
		std::string name;
		bool isActive;
		name = goNode->ToElement()->Attribute("name");
		goNode->ToElement()->QueryBoolAttribute("isActive", &isActive);
		auto gameObject = UEngine::GameObject::Instantiate(gameScene, name);
		gameObject->SetActive(isActive);
		goNode->ToElement()->QueryBoolAttribute("isStatic", &gameObject->IsStatic);

		{
			auto subNode = goNode->FirstChild();
			while (subNode)
			{
				std::string subName = subNode->ToElement()->Value();
				if (subName == "GameObject")
					LoadGameObject(goNode, subNode, gameObject);
				else
					LoadComponent(subNode, gameObject);

				gameScene->ResourceManager.ApplyChange();
				subNode = subNode->NextSibling();
			}
			if (editorMode) gameObject->AddComponent<UEngine::EditorScript>();
		}
		gameScene->ResourceManager.ApplyChange();

		goNode = goNode->NextSibling();
	}
}

void XMLSceneParser::LoadGameObject(TiXmlNode* parentNode, TiXmlNode* goNode, UEngine::GameObject* parent)
{
	std::string name;
	bool isActive;
	name = goNode->ToElement()->Attribute("name");
	goNode->ToElement()->QueryBoolAttribute("isActive", &isActive);
	auto child = UEngine::GameObject::Instantiate(parent->GetScene(), name);
	child->SetActive(isActive);
	goNode->ToElement()->QueryBoolAttribute("isStatic", &child->IsStatic);
	child->SetParent(parent);

	{
		auto subNode = goNode->FirstChild();
		while (subNode)
		{
			std::string subName = subNode->ToElement()->Value();
			if (subName == "GameObject")
				LoadGameObject(goNode, subNode, child);
			else
				LoadComponent(subNode, child);

			parent->GetScene()->ResourceManager.ApplyChange();
			subNode = subNode->NextSibling();
		}
		if (editorMode) child->AddComponent<UEngine::EditorScript>();
	}
}

void XMLSceneParser::LoadComponent(TiXmlNode* componentListNode, UEngine::GameObject* gameObject)
{
	std::string componentType = componentListNode->ToElement()->Value();
	
	auto componentNode = componentListNode->FirstChild();
	while (componentNode)
	{
		bool enabled;
		componentNode->ToElement()->QueryBoolAttribute("enabled", &enabled);
		UEngine::Component* component = CreateComponent(componentType, gameObject);

		auto memberVarNode = componentNode->FirstChild();
		if (componentType == "Transform" ||
			componentType == "RenderComponent" ||
			componentType == "Material" ||
			componentType == "Camera" ||
			componentType == "CircleCollider" ||
			componentType == "RectCollider")
		{
			if (memberVarNode) component->DeSerialize(memberVarNode);
			component->SetEnable(enabled);
		}
		else if (componentType == "GameObject")
		{
			throw std::runtime_error("it's impossible to be here");
		}
		else
		{
			typedef void (*DeSerializeComponent) (UEngine::Component*, TiXmlNode*);
			typedef void (*SetEnable) (UEngine::Component*, bool);
			DeSerializeComponent deserializeComponent = NULL;
			SetEnable setEnable = NULL;
			deserializeComponent = (DeSerializeComponent)UEngine::WinApplication::Get()->FindFunction("DeSerializeComponent");
			setEnable = (SetEnable)UEngine::WinApplication::Get()->FindFunction("SetEnable");
			if (memberVarNode) deserializeComponent(component, memberVarNode);
			setEnable(component, enabled);
		}
		componentNode = componentNode->NextSibling();
	}
}

UEngine::Component* XMLSceneParser::CreateComponent(std::string componentType, UEngine::GameObject* gameObject)
{
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
	else if (componentType == "GameObject")
	{
		throw std::runtime_error("it's impossible to be here");
	}
	else
	{
		typedef UEngine::Component* (*AddScript) (UEngine::GameObject* gameObject);
		AddScript scriptCreation = NULL;
		scriptCreation = (AddScript)UEngine::WinApplication::Get()->FindFunction(componentType + "Creation");
		component = scriptCreation(gameObject);
	}
	return component;
}

void XMLSceneParser::SaveGameObject
(
	TiXmlElement* node,
	std::string name,
	bool isActive,
	bool isStatic, 
	std::vector<UEngine::GameObject*> children,
	std::vector<UEngine::Component*> components)
{
	auto gameObject = new TiXmlElement("GameObject");
	gameObject->SetAttribute("name", name.c_str());
	gameObject->SetAttribute("isActive", isActive);
	gameObject->SetAttribute("isStatic", isStatic);
	for (auto component : components)
	{
		std::string raw_typeName = component->typeName;
		TiXmlString typeName = TiXmlString(raw_typeName.substr(4, raw_typeName.find_first_of('@') - 4).c_str());
		if (typeName != "EditorScript")
		{
			auto componentType = new TiXmlElement(typeName.c_str());
			auto xmlComponent = new TiXmlElement("Component");
			xmlComponent->SetAttribute("enabled", component->GetEnable());
			component->Serialize(xmlComponent);
			componentType->LinkEndChild(xmlComponent);
			gameObject->LinkEndChild(componentType);
		}
	}
	for (auto child : children)
		SaveGameObject(gameObject, child->name, child->GetActive(), child->IsStatic, child->GetChildren(), *child->GetComponents());
	node->LinkEndChild(gameObject);
}