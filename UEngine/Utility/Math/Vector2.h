#pragma once
#include "../UtilityDefinitions.h"
#include "Vector3.h"

namespace UEngine
{
	struct Vector2
	{
		float x{ 0 };
		float y{ 0 };

		Vector2() = default;
		Vector2(const struct Vector3 vector3);
		Vector2(float x, float y)
		{
			this->x = x;
			this->y = y;
		}
		Vector2(const POINT& point)
		{
			x = static_cast<float>(point.x);
			y = static_cast<float>(point.y);
		}
		Vector2(const Utility::Coordinate2D& coord2d)
		{
			x = coord2d.x;
			y = coord2d.y;
		}
		Vector2(const DirectX::XMFLOAT3& xmfloat3)
		{
			x = xmfloat3.x;
			y = xmfloat3.y;
		}
		Vector2(const DirectX::XMVECTOR& xmvector)
		{
			x = xmvector.m128_f32[0];
			y = xmvector.m128_f32[1];
		}

		static Vector2 Cross(const Vector2& lhs, const Vector2& rhs)
		{
			DirectX::XMVECTOR lVector = DirectX::XMVectorSet(lhs.x, lhs.y, 0, 0);
			DirectX::XMVECTOR rVector = DirectX::XMVectorSet(rhs.x, rhs.y, 0, 0);
			return DirectX::XMVector2Cross(lVector, rVector);
		}

		Vector2 Cross(const Vector2& rhs)
		{
			return Cross(*this, rhs);
		}

		static Vector2 Normalize(const Vector2& vector)
		{
			DirectX::XMVECTOR thisVector = DirectX::XMVectorSet(vector.x, vector.y, 0, 0);
			return DirectX::XMVector2Normalize(thisVector);
		}

		Vector2 Normalize()
		{
			return Normalize(*this);
		}

		static float Magnitude(const Vector2& vector)
		{
			DirectX::XMVECTOR thisVector = DirectX::XMVectorSet(vector.x, vector.y, 0, 0);
			return DirectX::XMVector2Length(thisVector).m128_f32[0];
		}

		float Magnitude()
		{
			return Magnitude(*this);
		}

		Vector2 operator+(const Vector2& rhs)
		{
			DirectX::XMVECTOR thisVector = DirectX::XMVectorSet(this->x, this->y, 0, 0);
			DirectX::XMVECTOR rhsVector = DirectX::XMVectorSet(rhs.x, rhs.y, 0, 0);
			return DirectX::XMVectorAdd(thisVector, rhsVector);
		}

		Vector2 operator-(const Vector2& rhs)
		{
			DirectX::XMVECTOR thisVector = DirectX::XMVectorSet(this->x, this->y, 0, 0);
			DirectX::XMVECTOR rhsVector = DirectX::XMVectorSet(rhs.x, rhs.y, 0, 0);
			return DirectX::XMVectorSubtract(thisVector, rhsVector);
		}

		float operator*(const Vector2& rhs)
		{
			DirectX::XMVECTOR thisVector = DirectX::XMVectorSet(this->x, this->y, 0, 0);
			DirectX::XMVECTOR rhsVector = DirectX::XMVectorSet(rhs.x, rhs.y, 0, 0);
			return DirectX::XMVector2Dot(thisVector, rhsVector).m128_f32[0];
		}

		Vector2 operator*(const Matrix& rhs)
		{
			DirectX::XMVECTOR thisVector = DirectX::XMVectorSet(this->x, this->y, 0, 0);
			return DirectX::XMVector2TransformCoord(thisVector, rhs);
		}

		Vector2 operator*(const float rhs)
		{
			DirectX::XMVECTOR thisVector = DirectX::XMVectorSet(this->x * rhs, this->y * rhs, 0, 0);
			return thisVector;
		}

		Vector2 operator*=(const Matrix& rhs)
		{
			DirectX::XMVECTOR thisVector = DirectX::XMVectorSet(this->x, this->y, 0, 0);
			return DirectX::XMVector2TransformCoord(thisVector, rhs);
		}
	};
}