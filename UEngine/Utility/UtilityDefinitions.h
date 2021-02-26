#pragma once
#include <DirectXMath.h>

namespace UEngine
{
	namespace Utility
	{
		struct Coordinate2D
		{
			float x;
			float y;
		};
	}

	struct Vector3
	{
		float x;
		float y;
		float z;

		Vector3() = default;
		Vector3(float x, float y, float z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}
		Vector3(const DirectX::XMVECTOR& xmvector)
		{
			x = xmvector.m128_f32[0];
			y = xmvector.m128_f32[1];
			z = xmvector.m128_f32[2];
		}
	};

	struct Color
	{
		float r;
		float g;
		float b;
		float a;
	};
}
