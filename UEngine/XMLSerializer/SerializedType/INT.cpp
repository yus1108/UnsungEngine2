#include "stdafx.h"
#include "INT.h"

void UEngine::TYPE::INT::Serialize(TiXmlElement* node)
{
	auto variable = new TiXmlElement("INT");
	variable->SetAttribute("name", name.c_str());
	variable->SetAttribute("value", value);
	node->LinkEndChild(variable);
}

