#pragma once

class SerializedType
{
protected:
	static size_t s_modeIndex;

	int item_current = 0;
	size_t modeIndex = s_modeIndex++;
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
