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

