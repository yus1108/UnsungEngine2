#pragma once

#define SERIALIZED_FLOAT(name) UEngine::TYPE::FLOAT name{this, #name}
#define SERIALIZED_FLOAT_INIT(name, value) UEngine::TYPE::FLOAT name{this, #name, value}

namespace UEngine
{
	namespace TYPE
	{
		class FLOAT : public SerializedType
		{
		private:
			// Inherited via SerializedType
			virtual void Serialize(TiXmlElement* node) override;
		public:
			FLOAT(Serializer* serializer, std::string name) : SerializedType(serializer, name) {}
			FLOAT(Serializer* serializer, std::string name, float value) : SerializedType(serializer, name) { this->value = value; }
			float value = 0;

			// copy assignment
			FLOAT& operator=(const float& other)
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