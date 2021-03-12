#pragma once
#include "UEngine.h"

namespace UEngine
{
	struct RenderObject
	{
		std::wstring name;
		UEngine::DXRenderer::DXShader* shader{ nullptr };
		UEngine::DXRenderer::DXRenderMesh* renderMesh{ nullptr };
		std::map<std::string, UEngine::DXRenderer::DXConstantBuffer*> constantBuffers;
		std::map<std::string, UEngine::DXRenderer::DXTexture*> textures;
	};

	struct DebugRenderPoint
	{
		Vector3 position;
		Color color{0};

		DebugRenderPoint() = default;
		DebugRenderPoint(const DebugRenderPoint &point)
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
		DebugRenderLine(DebugRenderLine &line)
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
		enum COMPARE_AABB_SIZE_RESULT
		{
			COMPARE_AABB_SIZE_RESULT_EQUAL,
			COMPARE_AABB_SIZE_RESULT_SMALLER,
			COMPARE_AABB_SIZE_RESULT_BIGGER,
			COMPARE_AABB_SIZE_RESULT_DIFFERENT
		};

		namespace Physics2D
		{
			struct CollisionResult
			{
				bool isColliding;
				Vector2 distance1;
				Vector2 distance2;
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
