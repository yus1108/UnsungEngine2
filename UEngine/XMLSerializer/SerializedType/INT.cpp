#include "stdafx.h"
#include "INT.h"

void UEngine::TYPE::INT::Serialize(TiXmlElement* node)
{
	auto variable = new TiXmlElement("INT");
	variable->SetAttribute(name.c_str(), value);
	node->LinkEndChild(variable);
}

void UEngine::TYPE::INT::DeSerialize(TiXmlNode* node)
{
	node->ToElement()->QueryIntAttribute(name.c_str(), &value);
}

void UEngine::TYPE::INT::OnEditRender()
{
	switch (item_current)
	{
	case 0:
		ImGui::InputInt(this->name.c_str(), &value);
		break;
	case 1:
		ImGui::DragInt(this->name.c_str(), &value);
		break;
	default:
		break;
	}
	const char* items[] = { "Input", "Drag" };
	ImGui::Combo((std::string("Mode") + std::to_string(modeIndex)).c_str(), &item_current, items, IM_ARRAYSIZE(items));
	ImGui::Separator();
}

