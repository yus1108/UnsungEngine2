#pragma once

namespace UEngine
{
	namespace Math
	{
		const Vector3 GetMousePosToWorld();

		namespace Physics2D
		{
			bool IsVertical(LineCoords line);
			float CalcSlope(LineCoords line);
			bool IsIntersecting(LineCoords l1, LineCoords l2);
			bool IsColliding(PointCoord point, TriangleCoords triangle);
			//bool IsColliding(LineCoords point, TriangleCoords triangle);
		}
	}
}