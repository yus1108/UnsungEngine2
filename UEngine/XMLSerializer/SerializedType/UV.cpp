#include "stdafx.h"
#include "UV.h"

void UEngine::TYPE::UV::Serialize(TiXmlElement* node)
{
	auto variable = new TiXmlElement("UV");
	variable->SetAttribute("name", name.c_str());
	variable->SetAttribute("maxX", value.maxX);
	variable->SetAttribute("maxY", value.maxY);
	variable->SetAttribute("minX", value.minX);
	variable->SetAttribute("minY", value.minY);
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
	const char* items[] = { "Input", "Drag", "Slider" };
	ImGui::Combo((std::string("Mode") + std::to_string(modeIndex)).c_str(), &item_current, items, IM_ARRAYSIZE(items));
	ImGui::Separator();
}

