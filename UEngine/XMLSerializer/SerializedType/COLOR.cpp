#include "stdafx.h"
#include "COLOR.h"

void UEngine::TYPE::COLOR::Serialize(TiXmlElement* node)
{
	auto variable = new TiXmlElement("COLOR");
	variable->SetAttribute("name", name.c_str());
	variable->SetAttribute("r", value.r);
	variable->SetAttribute("g", value.g);
	variable->SetAttribute("b", value.b);
	variable->SetAttribute("a", value.a);
	node->LinkEndChild(variable);
}

void UEngine::TYPE::COLOR::DeSerialize(TiXmlNode* node)
{
	double r, g, b, a;
	node->ToElement()->QueryDoubleAttribute("r", &r);
	node->ToElement()->QueryDoubleAttribute("g", &g);
	node->ToElement()->QueryDoubleAttribute("b", &b);
	node->ToElement()->QueryDoubleAttribute("a", &a);
	value.r = static_cast<float>(r);
	value.g = static_cast<float>(g);
	value.b = static_cast<float>(b);
	value.a = static_cast<float>(a);
}

void UEngine::TYPE::COLOR::OnEditRender()
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

