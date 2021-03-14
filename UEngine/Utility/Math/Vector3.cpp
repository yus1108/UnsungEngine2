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
