#include "stdafx.h"
#include "INT.h"

void UEngine::TYPE::INT::Serialize(TiXmlElement* node)
{
	auto variable = new TiXmlElement("INT");
	variable->SetAttribute(name.c_str(), value);
	node->LinkEndChild(variable);
}

void UEngine::TYPE::INT::DeSerialize(TiXmlNode* node)
{
	node->ToElement()->QueryIntAttribute(name.c_str(), &value);
}

