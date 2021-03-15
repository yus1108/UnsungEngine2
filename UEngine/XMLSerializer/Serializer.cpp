#include "stdafx.h"
#include "Serializer.h"

void Serializer::Serialize(TiXmlElement* node)
{
	for (auto variable : variables)
		variable->Serialize(node);
}

void Serializer::DeSerialize(TiXmlNode* node)
{
	auto variable = variables.begin();
	while (node)
	{
		(*variable)->DeSerialize(node);
		node = node->NextSibling();
		variable++;
	}
}
