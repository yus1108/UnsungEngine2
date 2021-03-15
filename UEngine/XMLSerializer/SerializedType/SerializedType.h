#pragma once

class SerializedType
{
protected:
	Serializer* serializer;
	std::string name;
public:
	SerializedType() = delete;
	SerializedType(SerializedType&) = delete;
	SerializedType(const SerializedType&) = delete;
	SerializedType(Serializer* serializer, std::string name);
	virtual void Serialize(TiXmlElement* node) = 0;
	virtual void DeSerialize(TiXmlNode* node) = 0;
};
