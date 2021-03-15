#pragma once

#define SERIALIZED_STRING(name) UEngine::TYPE::STRING name{this, #name}
#define SERIALIZED_STRING_INIT(name, value) UEngine::TYPE::STRING name{this, #name, value}

extern DWORD convert_unicode_to_utf8_string(
	__out std::string& utf8,
	__in const wchar_t* unicode,
	__in const size_t unicode_size
);
extern DWORD convert_utf8_to_unicode_string(
	__out std::wstring& unicode,
	__in const char* utf8,
	__in const size_t utf8_size
);

namespace UEngine
{
	namespace TYPE
	{
		class STRING : public SerializedType
		{
		private:
			// Inherited via SerializedType
			virtual void Serialize(TiXmlElement* node) override;
			virtual void DeSerialize(TiXmlNode* node) override;
		public:
			STRING(Serializer* serializer, std::string name) : SerializedType(serializer, name) {}
			STRING(Serializer* serializer, std::string name, std::string value) : SerializedType(serializer, name) { this->value = value; }
			std::string value;

			// copy assignment
			STRING& operator=(const std::string& other)
			{
				// Guard self assignment
				if (this->value == other)
					return *this;

				this->value = other;
				return *this;
			}
			// copy assignment
			STRING& operator=(const std::wstring& other)
			{
				std::string c_other;
				convert_unicode_to_utf8_string(c_other, other.c_str(), other.size());
				// Guard self assignment
				if (this->value == c_other)
					return *this;

				this->value = c_other;
				return *this;
			}

		};
	}
}
