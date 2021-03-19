#include "stdafx.h"
#include "UV.h"

void UEngine::TYPE::UV::Serialize(TiXmlElement* node)
{
	auto variable = new TiXmlElement("UV");
	variable->SetAttribute("name", name.c_str());
	variable->SetDoubleAttribute("maxX", value.maxX);
	variable->SetDoubleAttribute("maxY", value.maxY);
	variable->SetDoubleAttribute("minX", value.minX);
	variable->SetDoubleAttribute("minY", value.minY);
	node->LinkEndChild(variable);
}

void UEngine::TYPE::UV::DeSerialize(TiXmlNode* node)
{
	double maxX, maxY, minX, minY;
	node->ToElement()->QueryDoubleAttribute("maxX", &maxX);
	node->ToElement()->QueryDoubleAttribute("maxY", &maxY);
	node->ToElement()->QueryDoubleAttribute("minX", &minX);
	node->ToElement()->QueryDoubleAttribute("minY", &minY);
	value.maxX = static_cast<float>(maxX);
	value.maxY = static_cast<float>(maxY);
	value.minX = static_cast<float>(minX);
	value.minY = static_cast<float>(minY);
}

void UEngine::TYPE::UV::OnEditRender()
{
	ImGui::PushID(&this->name);
	switch (item_current)
	{
	case 0:
		ImGui::InputFloat4(this->name.c_str(), reinterpret_cast<float*>(&value));
		break;
	case 1:
		ImGui::DragFloat4(this->name.c_str(), reinterpret_cast<float*>(&value));
		break;
	case 2:
		ImGui::SliderFloat4(this->name.c_str(), reinterpret_cast<float*>(&value), 0, 1);
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

