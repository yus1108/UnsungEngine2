#pragma once

namespace UEngine
{
	namespace Utility
	{
		class UMath {
		public:
			static const float PI;
			static const float Convert_DegreeToRad(const float& degree);
			static const double Convert_DegreeToRad(const double& degree);

			static const UEngine::Definition::Coordinate2D ConvertPixelToNDC(const UEngine::Definition::Coordinate2D& mousePos, const UEngine::WinApplication* const window);

			/*static void CalculateAABB_2D(std::vector<SIMPLE_VERTEX> vertices, const Matrix& worldmat, AABB_2D& out);
			static void CalculateAABB_2D(const class RenderMesh* const renderMesh, const Matrix& worldmat, AABB_2D& out);
			static void CalculateAABB_2D(const class RenderMesh* const renderMesh, const class Transform* const transform, AABB_2D& out);
			static bool CollisionTest(const AABB_2D& a, const AABB_2D& b);

			static int RndInt(int start = 0, int end = 1);
			static float RndFloat(float start = 0.0f, float end = 1.0f);*/
		};
	}
}

