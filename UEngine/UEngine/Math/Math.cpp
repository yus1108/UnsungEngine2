#include "UEngine.h"
#include "Math.h"

bool UEngine::Math::Physics2D::IsVertical(LineCoords line)
{
	return line[1].x - line[0].x == 0;
}

float UEngine::Math::Physics2D::CalcSlope(LineCoords line)
{
	return (line[1].y - line[0].y) / (line[1].x - line[0].x);
}

bool UEngine::Math::Physics2D::IsIntersecting(LineCoords l1, LineCoords l2)
{
	CollisionResult result;
	bool isVertical1 = IsVertical(l1);
	bool isVertical2 = IsVertical(l2);
	float minY1 = min(l1[0].y, l1[1].y);
	float maxY1 = max(l1[0].y, l1[1].y);
	float minY2 = min(l2[0].y, l2[1].y);
	float maxY2 = max(l2[0].y, l2[1].y);
	float minX1 = min(l1[0].x, l1[1].x);
	float maxX1 = max(l1[0].x, l1[1].x);
	float minX2 = min(l2[0].x, l2[1].x);
	float maxX2 = max(l2[0].x, l2[1].x);
	float slope1, slope2;
	if (isVertical1 && isVertical2)
	{
		// if both lines are vertical
		if (l2[0].y < minY1 && l2[1].y < minY1) return false;
		if (l2[0].y > maxY1 && l2[1].y > maxY1) return false;
		return true;
	}
	if (isVertical1)
	{
		float slope = CalcSlope(l2);
		float y = slope * l1[0].x + l2[0].y;
		if (minY1 <= y && y <= maxY1) return true;
		return false;
	}
	if (isVertical2)
	{
		slope1 = CalcSlope(l1);
		float y = slope1 * l2[0].x + l1[0].y;
		if (minY2 <= y && y <= maxY2) return true;
		return false;
	}
	
	slope1 = CalcSlope(l1);
	slope2 = CalcSlope(l2);
	if (slope2 - slope1 == 0)
	{
		if (l2[0].x < minX1 && l2[1].x < minX1) return false;
		if (l2[0].x > maxX1 && l2[1].x > maxX1) return false;
		return true;
	}

	float x = (l1[0].y - l2[0].y) / (slope2 - slope1);
	if (x < minX1 || maxX1 < x) return false;
	if (x < minX2 || maxX2 < x) return false;

	return true;
}

bool UEngine::Math::Physics2D::IsColliding(PointCoord point, TriangleCoords triangle)
{
	Vector2 A, B, C;
	Vector2 CB, AC, BA;
	Vector2 pCB, pAC, pBA;

	A = triangle[0];
	B = triangle[1];
	C = triangle[2];

	CB = C - B;
	AC = A - C;
	BA = B - A;

	pCB = Vector2(CB.y, -CB.x);
	pAC = Vector2(AC.y, -AC.x);
	pBA = Vector2(BA.y, -BA.x);

	float d1, d2, d3;
	d1 = pCB * (point - B);
	d2 = pAC * (point - C);
	d3 = pBA * (point - A);

	if (d1 >= 0 && d2 >= 0 && d3 >= 0)
		return true;
	return false;
}

const UEngine::Vector3 UEngine::Math::GetMousePosToWorld()
{
	using namespace DirectX;
	// mouse ndc position to world position = mousePos * Projection^-1 * View^-1
	auto mousePos = Utility::UMath::ConvertPixelToNDC(WinInput::Get()->GetMousePos(), WinApplication::Get()->GetHandler());
	auto cpu_camera = Camera::mainCamera->GetCameraMatrix();
	Vector3 vMousePos = mousePos;
	auto pDet = XMMatrixDeterminant(cpu_camera.projection);
	auto vDet = XMMatrixDeterminant(cpu_camera.view);
	vMousePos = vMousePos * XMMatrixInverse(&pDet, cpu_camera.projection);
	vMousePos = vMousePos * XMMatrixInverse(&vDet, cpu_camera.view);
	vMousePos.z = 0;
	return Vector3(vMousePos);
}
