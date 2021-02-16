#include <Windows.h>
#include "Definition.h"
#include "..\WinApplication\WinApplication.h"
#include "UMath.h"

namespace UEngine
{
	namespace Utility
	{
		const float UMath::PI = 3.14159f;

		const float UMath::Convert_DegreeToRad(const float& degree)
		{
			//return degree / 180.0f * 3.14159f;
			return degree * 0.0174532f;
		}
		const double UMath::Convert_DegreeToRad(const double& degree)
		{
			//return degree / 180.0 * 3.14159265359;
			return degree * 0.0174532925199;
		}

		const UEngine::Definition::Coordinate2D UMath::ConvertPixelToNDC(const UEngine::Definition::Coordinate2D& mousePos, HWND window)
		{
			UEngine::Definition::Coordinate2D pos = mousePos;
			RECT clientSize;
			GetClientRect(window, &clientSize);
			pos.x /= clientSize.right - 1;
			pos.y /= clientSize.bottom - 1;

			pos.x = (pos.x - 0.5f) * 2.0f;
			pos.y = 1.0f - pos.y * 2.0f;

			return pos;
		}
	}
}


//void UMath::CalculateAABB_2D(std::vector<SIMPLE_VERTEX> vertices, const Matrix& worldmat, AABB_2D& out)
//{
//	// apply world matrix
//	Matrix transposed = XMMatrixTranspose(worldmat);
//
//	for (unsigned int i = 0; i < vertices.size(); i++)
//	{
//		DirectX::XMVECTOR temp = XMLoadFloat3(&vertices[i].pos);
//		temp.m128_f32[3] = 1;
//
//		DirectX::XMVECTOR pos = DirectX::XMVectorSet(0, 0, 0, 0);
//		pos.m128_f32[0] = DirectX::XMVector4Dot(
//			temp,
//			transposed.r[0]).m128_f32[0];
//		pos.m128_f32[1] = DirectX::XMVector4Dot(
//			temp,
//			transposed.r[1]).m128_f32[0];
//
//		XMStoreFloat3(&vertices[i].pos, pos);
//	}
//
//	out.xaxis = { vertices[0].pos.x, vertices[0].pos.x };
//	out.yaxis = { vertices[0].pos.y, vertices[0].pos.y };
//
//	for (unsigned int i = 1; i < vertices.size(); i++)
//	{
//		// min
//		if (out.xaxis.min > vertices[i].pos.x)
//			out.xaxis.min = vertices[i].pos.x;
//		if (out.yaxis.min > vertices[i].pos.y)
//			out.yaxis.min = vertices[i].pos.y;
//
//		// max
//		if (out.xaxis.max < vertices[i].pos.x)
//			out.xaxis.max = vertices[i].pos.x;
//		if (out.yaxis.max < vertices[i].pos.y)
//			out.yaxis.max = vertices[i].pos.y;
//	}
//}
//
//void UMath::CalculateAABB_2D(const RenderMesh* const renderMesh, const Matrix& worldmat, AABB_2D& out)
//{
//	CalculateAABB_2D(renderMesh->GetVertices(), worldmat, out);
//}
//
//void UMath::CalculateAABB_2D(const RenderMesh* const renderMesh, const Transform* const transform, AABB_2D& out)
//{
//	CalculateAABB_2D(renderMesh->GetVertices(), transform->GetWorldMatrix(), out);
//}
//
//bool UMath::CollisionTest(const AABB_2D& a, const AABB_2D& b) {
//	MinMax aX = a.xaxis;
//	MinMax aY = a.yaxis;
//	MinMax bX = b.xaxis;
//	MinMax bY = b.yaxis;
//
//	if (aX.max < bX.min || aX.min > bX.max) return false;
//	if (aY.max < bY.min || aY.min > bY.max) return false;
//
//	return true;
//}
//
//int UMath::RndInt(int start, int end)
//{
//	return rand() % (end - start + 1) + start;
//}
//
//float UMath::RndFloat(float start, float end)
//{
//	return (float)rand() / (float)RAND_MAX * (end - start) + start;
//}