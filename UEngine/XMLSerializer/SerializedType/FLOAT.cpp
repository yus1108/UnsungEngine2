#include "stdafx.h"
#include "FLOAT.h"

void UEngine::TYPE::FLOAT::Serialize(TiXmlElement* node)
{
	auto variable = new TiXmlElement("FLOAT");
	variable->SetAttribute(name.c_str(), value);
	node->LinkEndChild(variable);
}

void UEngine::TYPE::FLOAT::DeSerialize(TiXmlNode* node)
{
	double doubleValue;
	node->ToElement()->QueryDoubleAttribute(name.c_str(), &doubleValue);
	value = static_cast<float>(doubleValue);
}

