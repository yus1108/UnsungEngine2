#pragma once

class SerializedType
{
protected:
	int item_current = 0;
	Serializer* serializer = nullptr;
	std::string name;
public:
	SerializedType() = delete;
	SerializedType(SerializedType&) = delete;
	SerializedType(const SerializedType&) = delete;
	SerializedType(Serializer* serializer, std::string name);
	virtual ~SerializedType() = default;
	virtual void Serialize(TiXmlElement* node) = 0;
	virtual void DeSerialize(TiXmlNode* node) = 0;
	virtual void OnEditRender() = 0;
};
