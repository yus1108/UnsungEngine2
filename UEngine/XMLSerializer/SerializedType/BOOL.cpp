#include "stdafx.h"
#include "BOOL.h"

void UEngine::TYPE::BOOL::Serialize(TiXmlElement* node)
{
	auto variable = new TiXmlElement("BOOL");
	variable->SetAttribute(name.c_str(), value);
	node->LinkEndChild(variable);
}

void UEngine::TYPE::BOOL::DeSerialize(TiXmlNode* node)
{
	node->ToElement()->QueryBoolAttribute(name.c_str(), &value);
}

void UEngine::TYPE::BOOL::OnEditRender()
{
	ImGui::PushID(&this->name);
	ImGui::Checkbox(this->name.c_str(), &value);
	ImGui::PopID();
	ImGui::Separator();
}

