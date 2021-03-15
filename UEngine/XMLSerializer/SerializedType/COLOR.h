#pragma once

#define SERIALIZED_COLOR(name) UEngine::TYPE::COLOR name{this, #name}
#define SERIALIZED_COLOR_INIT(name, value) UEngine::TYPE::COLOR name{this, #name, value}

namespace UEngine
{
	struct Color
	{
		float r;
		float g;
		float b;
		float a;
	};

	namespace TYPE
	{
		class COLOR : public SerializedType
		{
		private:
			// Inherited via SerializedType
			virtual void Serialize(TiXmlElement* node) override;
			virtual void DeSerialize(TiXmlNode* node) override;
		public:
			COLOR(Serializer* serializer, std::string name) : SerializedType(serializer, name) {}
			COLOR(Serializer* serializer, std::string name, Color value) : SerializedType(serializer, name) { this->value = value; }
			Color value{ 0, 0, 0, 1 };

			// copy assignment
			COLOR& operator=(const Color& other)
			{
				this->value = other;
				return *this;
			}

		};
	}
}
