#include "stdafx.h"
#include "BOOL.h"

void UEngine::TYPE::BOOL::Serialize(TiXmlElement* node)
{
	auto variable = new TiXmlElement("BOOL");
	variable->SetAttribute("name", name.c_str());
	variable->SetAttribute("value", value);
	node->LinkEndChild(variable);
}

