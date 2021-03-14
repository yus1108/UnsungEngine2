#include "stdafx.h"
#include "Serializer.h"

void Serializer::Serialize(TiXmlElement* node)
{
	for (auto variable : variables)
		variable->Serialize(node);
}
