#include "stdafx.h"
#include "FLOAT.h"

void UEngine::TYPE::FLOAT::Serialize(TiXmlElement* node)
{
	auto variable = new TiXmlElement("FLOAT");
	variable->SetDoubleAttribute(name.c_str(), value);
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
	ImGui::PushID(&this->name);
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
	ImGui::PopID();
	ImGui::PushID(&this->items);
	ImGui::Combo("Mode", &item_current, items, IM_ARRAYSIZE(items));
	ImGui::PopID(); 
	ImGui::Separator();
}

