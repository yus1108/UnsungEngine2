#include "stdafx.h"
#include "FLOAT.h"

void UEngine::TYPE::FLOAT::Serialize(TiXmlElement* node)
{
	auto variable = new TiXmlElement("FLOAT");
	variable->SetAttribute("name", name.c_str());
	variable->SetAttribute("value", value);
	node->LinkEndChild(variable);
}

