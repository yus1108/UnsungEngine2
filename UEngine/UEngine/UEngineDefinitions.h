#pragma once
#include "UEngine.h"

namespace UEngine
{
	struct RenderObject
	{
		/*
			It is the number of DXRenderObject to access the resource by refering this number
		*/
		UINT objectNumber;
		/*
			key: std::string = typeid(T).raw_name()
			value: class DXConstantBuffer*
		*/
		std::map<std::string, DXRenderer::DXConstantBuffer*> constantBuffers;
	};

	struct DebugRenderPoint
	{
		Vector3 position;
		Color color{ 0 };

		DebugRenderPoint() = default;
		DebugRenderPoint(const DebugRenderPoint& point)
		{
			position = point.position;
			color = point.color;
		}
		DebugRenderPoint(Vector3 position, Color color)
		{
			this->position = position;
			this->color = color;
		}
	};

	struct DebugRenderLine
	{
		Vector3 lineStart;
		Vector3 lineEnd;
		Color color;

		DebugRenderLine() = default;
		DebugRenderLine(DebugRenderLine& line)
		{
			lineStart = line.lineStart;
			lineEnd = line.lineEnd;
			color = line.color;
		}
		DebugRenderLine(Vector3 lineStart, Vector3 lineEnd, Color color)
		{
			this->lineStart = lineStart;
			this->lineEnd = lineEnd;
			this->color = color;
		}
		DebugRenderPoint GetStartPoint() { return DebugRenderPoint(lineStart, color); }
		DebugRenderPoint GetEndPoint() { return DebugRenderPoint(lineEnd, color); }
	};

	namespace Math
	{
		namespace Physics2D
		{
			struct CollisionResult
			{
				bool isColliding;
				Vector2 coord;
			};

			typedef Vector2 PointCoord;
			typedef std::array<Vector2, 2> LineCoords;
			typedef std::array<Vector2, 3> TriangleCoords;
			struct AABB
			{
				float left;
				float top;
				float right;
				float bottom;
			};
			struct CircleCoord
			{
				Vector2 center;
				float radius;
			};
		}
	}
}
