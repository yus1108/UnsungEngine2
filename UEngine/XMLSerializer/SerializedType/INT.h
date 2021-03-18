#pragma once

#define SERIALIZED_INT(name) UEngine::TYPE::INT name{this, #name}
#define SERIALIZED_INT_INIT(name, value) UEngine::TYPE::INT name{this, #name, value}

namespace UEngine
{
	namespace TYPE
	{
		class INT : public SerializedType
		{
		private:
			// Inherited via SerializedType
			virtual void Serialize(TiXmlElement* node) override;
			virtual void DeSerialize(TiXmlNode* node) override;
			virtual void OnEditRender() override;
		public:
			INT(Serializer* serializer, std::string name) : SerializedType(serializer, name) {}
			INT(Serializer* serializer, std::string name, int value) : SerializedType(serializer, name) { this->value = value; }
			int value = 0;
			const char* items[2] = { "Input", "Drag" };

			// copy assignment
			INT& operator=(const int& other)
			{
				// Guard self assignment
				if (this->value == other)
					return *this;

				this->value = other;
				return *this;
			}

		};
	}
}