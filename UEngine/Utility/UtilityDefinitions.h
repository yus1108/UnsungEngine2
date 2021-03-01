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

	typedef DirectX::XMMATRIX Matrix;
	inline Matrix MatrixIdentity() { return DirectX::XMMatrixIdentity(); }

	struct Color
	{
		float r;
		float g;
		float b;
		float a;
	};
}

#include "Math\Vector2.h"
#include "Math\Vector3.h"