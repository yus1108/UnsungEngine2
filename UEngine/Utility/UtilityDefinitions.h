#pragma once
#include <DirectXMath.h>
#include "../XMLSerializer/stdafx.h"

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
}

#include "Math\Vector2.h"
#include "Math\Vector3.h"