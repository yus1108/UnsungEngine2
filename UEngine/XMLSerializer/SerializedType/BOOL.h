#pragma once

#define SERIALIZED_BOOL(name) UEngine::TYPE::BOOL name{this, #name}
#define SERIALIZED_BOOL_INIT(name, value) UEngine::TYPE::BOOL name{this, #name, value}

namespace UEngine
{
	namespace TYPE
	{
		class BOOL : public SerializedType
		{
		private:
			// Inherited via SerializedType
			virtual void Serialize(TiXmlElement* node) override;
			virtual void DeSerialize(TiXmlNode* node) override;
		public:
			BOOL(Serializer* serializer, std::string name) : SerializedType(serializer, name) {}
			BOOL(Serializer* serializer, std::string name, bool value) : SerializedType(serializer, name) { this->value = value; }
			bool value = false;

			// copy assignment
			BOOL& operator=(const bool& other)
			{
				// Guard self assignment
				if (this->value == other)
					return *this;

				this->value = other;
				return *this;
			}

			// Inherited via SerializedType
		};
	}
}
