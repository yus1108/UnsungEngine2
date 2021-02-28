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

	struct DebugVertex
	{
		Vector3 position;
		Color color;

		DebugVertex() = default;
		DebugVertex(DebugVertex& debugVertex)
		{
			position = debugVertex.position;
			color = debugVertex.color;
		}
		DebugVertex(Vector3 position, Color color)
		{
			this->position = position;
			this->color = color;
		}
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
		}
	}
}
