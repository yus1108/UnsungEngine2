#pragma once

namespace UEngine
{
	namespace Math
	{
		const Vector3 GetMousePosToWorld();
		const float Clamp(float value, float min, float max);

		namespace Physics2D
		{
			bool IsVertical(LineCoords line);
			float CalcSlope(LineCoords line);
			bool IsIntersecting(LineCoords l1, LineCoords l2);
			std::vector<PointCoord> MakePointVector(PointCoord* points, size_t size);
			TriangleCoords MakeTriangle(Matrix worldMatrix); // Make AABB of the triangle with the given world matrix
			AABB MakeAABB(const std::vector<DXRenderer::SIMPLE_VERTEX> vertices, Matrix worldMatrix); // make aabb of any model
			AABB MakeAABB(std::vector<PointCoord> points, Matrix worldMatrix); // Make AABB of the points
			AABB MakeAABB(std::vector<PointCoord> points); // Make AABB of the points
			AABB MakeAABB(PointCoord point); // Make AABB of the point
			AABB MakeAABB(LineCoords line); // Make AABB of the line with the given world matrix
			AABB MakeAABB(Matrix worldMatrix); // Make AABB of the rectangle with the given world matrix
			CircleCoord MakeCircle(Vector2 center, float radius); // Make AABB of the circle with the given world matrix
			bool IsColliding(PointCoord point, TriangleCoords triangle);
			bool IsColliding(PointCoord point, AABB aabb);
			bool IsColliding(PointCoord point, CircleCoord circle);
			bool IsColliding(LineCoords line, TriangleCoords triangle);
			bool IsColliding(LineCoords line, AABB aabb);
			bool IsColliding(LineCoords line, CircleCoord circle);
			bool IsColliding(TriangleCoords triangle1, TriangleCoords triangle2);
			bool IsColliding(TriangleCoords triangle, AABB aabb);
			bool IsColliding(TriangleCoords triangle, CircleCoord circle);
			bool IsColliding(AABB aabb1, AABB aabb2);
			bool IsColliding(AABB aabb, CircleCoord circle);
			bool IsColliding(CircleCoord circle1, CircleCoord circle2);
		}
	}
}