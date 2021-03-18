#include "stdafx.h"
#include "FLOAT.h"

void UEngine::TYPE::FLOAT::Serialize(TiXmlElement* node)
{
	auto variable = new TiXmlElement("FLOAT");
	variable->SetAttribute(name.c_str(), value);
	node->LinkEndChild(variable);
}

void UEngine::TYPE::FLOAT::DeSerialize(TiXmlNode* node)
{
	double doubleValue;
	node->ToElement()->QueryDoubleAttribute(name.c_str(), &doubleValue);
	value = static_cast<float>(doubleValue);
}

void UEngine::TYPE::FLOAT::OnEditRender()
{
	switch (item_current)
	{
	case 0:
		ImGui::InputFloat(this->name.c_str(), &value);
		break;
	case 1:
		ImGui::DragFloat(this->name.c_str(), &value);
		break;
	default:
		break;
	}
	const char* items[] = { "Input", "Drag" };
	ImGui::Combo((std::string("Mode") + std::to_string(modeIndex)).c_str(), &item_current, items, IM_ARRAYSIZE(items));
	ImGui::Separator();
}

