#include "stdafx.h"
#include "SerializedType.h"

SerializedType::SerializedType(Serializer* serializer, std::string name)
{
	this->serializer = nullptr;
	if (serializer == nullptr) return;
	this->serializer = serializer;
	this->name = name;
	serializer->AddVariable(this);
}
