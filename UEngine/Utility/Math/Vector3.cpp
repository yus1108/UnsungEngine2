#include "Vector3.h"

UEngine::Vector3::Vector3(const Vector2& vector2)
{
	x = vector2.x;
	y = vector2.y;
}

void UEngine::VECTOR3::Serialize(TiXmlElement* node)
{
	auto variable = new TiXmlElement("Vector3");
	variable->SetAttribute("name", name.c_str());
	variable->SetDoubleAttribute("x", value.x);
	variable->SetDoubleAttribute("y", value.y);
	variable->SetDoubleAttribute("z", value.z);
	node->LinkEndChild(variable);
}

void UEngine::VECTOR3::DeSerialize(TiXmlNode* node)
{
	double x, y, z;
	node->ToElement()->QueryDoubleAttribute("x", &x);
	node->ToElement()->QueryDoubleAttribute("y", &y);
	node->ToElement()->QueryDoubleAttribute("z", &z);
	value.x = static_cast<float>(x);
	value.y = static_cast<float>(y);
	value.z = static_cast<float>(z);
}

void UEngine::VECTOR3::OnEditRender()
{
	ImGui::PushID(&this->name);
	switch (item_current)
	{
	case 0:
		ImGui::InputFloat3(this->name.c_str(), reinterpret_cast<float*>(&value));
		break;
	case 1:
		ImGui::DragFloat3(this->name.c_str(), reinterpret_cast<float*>(&value));
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
