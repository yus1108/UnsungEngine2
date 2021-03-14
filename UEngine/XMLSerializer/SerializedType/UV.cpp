#include "stdafx.h"
#include "UV.h"

void UEngine::TYPE::UV::Serialize(TiXmlElement* node)
{
	auto variable = new TiXmlElement("BOOL");
	variable->SetAttribute("name", name.c_str());
	variable->SetAttribute("maxX", value.maxX);
	variable->SetAttribute("maxY", value.maxY);
	variable->SetAttribute("minX", value.minX);
	variable->SetAttribute("minY", value.minY);
	node->LinkEndChild(variable);
}

