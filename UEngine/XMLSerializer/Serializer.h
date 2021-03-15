#pragma once

class Serializer
{
private:
	std::list<class SerializedType*> variables;

public:
	void AddVariable(SerializedType* variable) { variables.emplace_back(variable); }
	virtual void Serialize(TiXmlElement* node);
	virtual void DeSerialize(TiXmlNode* node);
};

