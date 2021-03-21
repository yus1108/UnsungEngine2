#pragma once

class Serializer
{
private:
	std::list<class SerializedType*> variables;

protected:
	virtual void OnEditRender();

public:
	void AddVariable(SerializedType* variable) { variables.emplace_back(variable); }
	virtual void Serialize(TiXmlElement* node);
	virtual void DeSerialize(TiXmlNode* node);
};

