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

