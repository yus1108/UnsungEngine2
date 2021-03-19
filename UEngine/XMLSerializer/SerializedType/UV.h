#pragma once

#define SERIALIZED_UV(name) UEngine::TYPE::UV name{this, #name}
#define SERIALIZED_UV_INIT(name, value) UEngine::TYPE::UV name{this, #name, value}

namespace UEngine
{
	struct UV
	{
		float minX;
		float minY;
		float maxX;
		float maxY;
	};

	namespace TYPE
	{
		class UV : public SerializedType
		{
		private:
			// Inherited via SerializedType
			virtual void Serialize(TiXmlElement* node) override;
			virtual void DeSerialize(TiXmlNode* node) override;
			virtual void OnEditRender() override;
		public:
			UV(Serializer* serializer, std::string name) : SerializedType(serializer, name) {}
			UV(Serializer* serializer, std::string name, UEngine::UV value) : SerializedType(serializer, name) { this->value = value; }
			UEngine::UV value{ 0, 0, 0, 0 };
			const char* items[3] = { "Input", "Drag", "Slider" };

			// copy assignment
			UV& operator=(const UEngine::UV& other)
			{
				this->value = other;
				return *this;
			}
		};
	}
}
