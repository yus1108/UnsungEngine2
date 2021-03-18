#pragma once
#include "../UtilityDefinitions.h"

#define SERIALIZED_VECTOR3(name) UEngine::VECTOR3 name{this, #name}
#define SERIALIZED_VECTOR3_INIT(name, value) UEngine::VECTOR3 name{this, #name, value}

namespace UEngine
{
	struct Vector3
	{
		float x{ 0 };
		float y{ 0 };
		float z{ 0 };

		Vector3() = default;
		Vector3(const Vector3& vector) : x(vector.x), y(vector.y), z(vector.z) {}
		Vector3(const struct Vector2& vector2);
		Vector3(float x, float y, float z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}
		Vector3(const Utility::Coordinate2D& coord2d)
		{
			x = coord2d.x;
			y = coord2d.y;
		}
		Vector3(const DirectX::XMVECTOR& xmvector)
		{
			x = xmvector.m128_f32[0];
			y = xmvector.m128_f32[1];
			z = xmvector.m128_f32[2];
		}

		static Vector3 Cross(const Vector3& lhs, const Vector3& rhs)
		{
			DirectX::XMVECTOR lVector = DirectX::XMVectorSet(lhs.x, lhs.y, lhs.z, 0);
			DirectX::XMVECTOR rVector = DirectX::XMVectorSet(rhs.x, rhs.y, rhs.z, 0);
			return DirectX::XMVector3Cross(lVector, rVector);
		}

		Vector3 Cross(const Vector3& rhs)
		{
			return Cross(*this, rhs);
		}

		static Vector3 Normalize(const Vector3& vector)
		{
			DirectX::XMVECTOR thisVector = DirectX::XMVectorSet(vector.x, vector.y, vector.z, 0);
			return DirectX::XMVector3Normalize(thisVector);
		}

		float Magnitude()
		{
			return Magnitude(*this);
		}

		static float Magnitude(const Vector3& vector)
		{
			DirectX::XMVECTOR thisVector = DirectX::XMVectorSet(vector.x, vector.y, vector.z, 0);
			return DirectX::XMVector3Length(thisVector).m128_f32[0];
		}

		Vector3 Normalize()
		{
			return Normalize(*this);
		}

		Vector3 operator+(const Vector3& rhs)
		{
			DirectX::XMVECTOR thisVector = DirectX::XMVectorSet(this->x, this->y, this->z, 0);
			DirectX::XMVECTOR rhsVector = DirectX::XMVectorSet(rhs.x, rhs.y, rhs.z, 0);
			return DirectX::XMVectorAdd(thisVector, rhsVector);
		}

		Vector3 operator-(const Vector3& rhs)
		{
			DirectX::XMVECTOR thisVector = DirectX::XMVectorSet(this->x, this->y, this->z, 0);
			DirectX::XMVECTOR rhsVector = DirectX::XMVectorSet(rhs.x, rhs.y, rhs.z, 0);
			return DirectX::XMVectorSubtract(thisVector, rhsVector);
		}

		float operator*(const Vector3& rhs)
		{
			DirectX::XMVECTOR thisVector = DirectX::XMVectorSet(this->x, this->y, this->z, 0);
			DirectX::XMVECTOR rhsVector = DirectX::XMVectorSet(rhs.x, rhs.y, rhs.z, 0);
			return DirectX::XMVector3Dot(thisVector, rhsVector).m128_f32[0];
		}

		Vector3 operator*(const Matrix& rhs)
		{
			DirectX::XMVECTOR thisVector = DirectX::XMVectorSet(this->x, this->y, this->z, 0);
			return DirectX::XMVector3TransformCoord(thisVector, rhs);
		}

		Vector3 operator*(const float rhs)
		{
			DirectX::XMVECTOR thisVector = DirectX::XMVectorSet(this->x * rhs, this->y * rhs, this->z * rhs, 0);
			return thisVector;
		}
	};

	class VECTOR3 : public SerializedType
	{
		// Inherited via SerializedType
		virtual void Serialize(TiXmlElement* node) override;
		virtual void DeSerialize(TiXmlNode* node) override;
		virtual void OnEditRender() override;
	public:
		VECTOR3(Serializer* serializer, std::string name) : SerializedType(serializer, name) {}
		VECTOR3(Serializer* serializer, std::string name, Vector3 value) : SerializedType(serializer, name) { this->value = value; }
		Vector3 value;

		// copy assignment
		VECTOR3& operator=(const Vector3& other)
		{
			this->value = other;
			return *this;
		}
		// copy assignment
		VECTOR3& operator=(const Vector2& other)
		{
			this->value = other;
			return *this;
		}
	};
}