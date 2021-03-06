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
	float minY1 = min(l1[0].y, l1[1].y);
	float maxY1 = max(l1[0].y, l1[1].y);
	float minY2 = min(l2[0].y, l2[1].y);
	float maxY2 = max(l2[0].y, l2[1].y);
	float minX1 = min(l1[0].x, l1[1].x);
	float maxX1 = max(l1[0].x, l1[1].x);
	float minX2 = min(l2[0].x, l2[1].x);
	float maxX2 = max(l2[0].x, l2[1].x);

	if (l2[0].x < minX1 && l2[1].x < minX1) return false;
	if (l2[0].x > maxX1 && l2[1].x > maxX1) return false;
	if (l2[0].y < minY1 && l2[1].y < minY1) return false;
	if (l2[0].y > maxY1 && l2[1].y > maxY1) return false;

	bool isVertical1 = IsVertical(l1);
	bool isVertical2 = IsVertical(l2);
	float slope1, slope2;
	float x, y;

	if (isVertical1 && isVertical2) return true;

	if (isVertical1)
	{
		slope2 = CalcSlope(l2);
		x = (l2[0].y - l1[0].y) / (slope2);
		y = slope2 * l1[0].x + l2[0].y;
	}
	else if (isVertical2)
	{
		slope1 = CalcSlope(l1);
		x = (l1[0].y - l2[0].y) / (slope1);
		y = slope1 * l2[0].x + l1[0].y;
	}
	else
	{
		slope1 = CalcSlope(l1);
		slope2 = CalcSlope(l2);
		if (slope2 - slope1 == 0) return true;

		x = (l1[0].y - l2[0].y) / (slope2 - slope1);
		y = slope1 * x + l1[0].y;
	}
	
	if (x < minX1 || maxX1 < x) return false;
	if (x < minX2 || maxX2 < x) return false;
	if (y < minY1 || maxY1 < y) return false;
	if (y < minY2 || maxY2 < y) return false;

	return true;
}

std::vector<UEngine::Math::Physics2D::PointCoord> UEngine::Math::Physics2D::MakePointVector(PointCoord* points, size_t size)
{
	std::vector<PointCoord> vector;
	vector.reserve(size);
	for (size_t i = 0; i < size; i++)
		vector.push_back(points[i]);
	return vector;
}

UEngine::Math::Physics2D::TriangleCoords UEngine::Math::Physics2D::MakeTriangle(Matrix worldMatrix)
{
	Math::Physics2D::TriangleCoords triangle;
	auto triVertices = DXRenderer::Get()->ResourceManager->GetResource<DXRenderer::DXRenderMesh>("triangle")->GetVertices();
	for (size_t i = 0; i < triangle.size(); i++)
		triangle[i] = Vector2(triVertices[i].pos) * worldMatrix;
	return triangle;
}

UEngine::Math::Physics2D::AABB UEngine::Math::Physics2D::MakeAABB
(
	const std::vector<DXRenderer::SIMPLE_VERTEX> vertices, 
	Matrix worldMatrix
)
{
	std::vector<Vector2> vertsTransformed;
	vertsTransformed.reserve(vertices.size());
	for (size_t i = 0; i < vertices.size(); i++)
		vertsTransformed.push_back(Vector2(vertices[i].pos) * worldMatrix);
	AABB aabb = AABB{ vertsTransformed[0].x, vertsTransformed[0].y, vertsTransformed[0].x, vertsTransformed[0].y };
	for (size_t i = 1; i < vertices.size(); i++)
	{
		if (vertsTransformed[i].x < aabb.left) aabb.left = vertsTransformed[i].x;
		if (vertsTransformed[i].x > aabb.right) aabb.right = vertsTransformed[i].x;
		if (vertsTransformed[i].y < aabb.bottom) aabb.bottom = vertsTransformed[i].y;
		if (vertsTransformed[i].y > aabb.top) aabb.top = vertsTransformed[i].y;
	}
	return aabb;
}

UEngine::Math::Physics2D::AABB UEngine::Math::Physics2D::MakeAABB(const std::vector<DXRenderer::SIMPLE_VERTEX> vertices)
{
	std::vector<Vector2> vertsTransformed;
	vertsTransformed.reserve(vertices.size());
	for (size_t i = 0; i < vertices.size(); i++)
		vertsTransformed.push_back(Vector2(vertices[i].pos));
	AABB aabb = AABB{ vertsTransformed[0].x, vertsTransformed[0].y, vertsTransformed[0].x, vertsTransformed[0].y };
	for (size_t i = 1; i < vertices.size(); i++)
	{
		if (vertsTransformed[i].x < aabb.left) aabb.left = vertsTransformed[i].x;
		if (vertsTransformed[i].x > aabb.right) aabb.right = vertsTransformed[i].x;
		if (vertsTransformed[i].y < aabb.bottom) aabb.bottom = vertsTransformed[i].y;
		if (vertsTransformed[i].y > aabb.top) aabb.top = vertsTransformed[i].y;
	}
	return aabb;
}

UEngine::Math::Physics2D::AABB UEngine::Math::Physics2D::MakeAABB(std::vector<PointCoord> points, Matrix worldMatrix)
{
	for (size_t i = 0; i < points.size(); i++)
		points[i] = points[i] * worldMatrix;
	AABB aabb = AABB{ points[0].x, points[0].y, points[0].x, points[0].y };
	for (size_t i = 1; i < points.size(); i++)
	{
		if (points[i].x < aabb.left) aabb.left = points[i].x;
		if (points[i].x > aabb.right) aabb.right = points[i].x;
		if (points[i].y < aabb.bottom) aabb.bottom = points[i].y;
		if (points[i].y > aabb.top) aabb.top = points[i].y;
	}
	return aabb;
}

UEngine::Math::Physics2D::AABB UEngine::Math::Physics2D::MakeAABB(std::vector<PointCoord> points)
{
	AABB aabb = AABB{ points[0].x, points[0].y, points[0].x, points[0].y };
	for (size_t i = 1; i < points.size(); i++)
	{
		if (points[i].x < aabb.left) aabb.left = points[i].x;
		if (points[i].x > aabb.right) aabb.right = points[i].x;
		if (points[i].y < aabb.bottom) aabb.bottom = points[i].y;
		if (points[i].y > aabb.top) aabb.top = points[i].y;
	}
	return aabb;
}

UEngine::Math::Physics2D::AABB UEngine::Math::Physics2D::MakeAABB(Matrix worldMatrix)
{
	auto rectVertices = DXRenderer::Get()->ResourceManager->GetResource<DXRenderer::DXRenderMesh>("rectangle")->GetVertices();
	return MakeAABB(rectVertices, worldMatrix);
}

UEngine::Math::Physics2D::AABB UEngine::Math::Physics2D::MakeAABB(AABB local, Matrix worldMatrix)
{
	PointCoord points[4] = 
	{
		PointCoord(local.left, local.bottom),
		PointCoord(local.left, local.top),
		PointCoord(local.right, local.top),
		PointCoord(local.right, local.bottom)
	};

	for (size_t i = 0; i < 4; i++)
		points[i] = points[i] * worldMatrix;
	AABB aabb = AABB{ points[0].x, points[0].y, points[0].x, points[0].y };
	for (size_t i = 1; i < 4; i++)
	{
		if (points[i].x < aabb.left) aabb.left = points[i].x;
		if (points[i].x > aabb.right) aabb.right = points[i].x;
		if (points[i].y < aabb.bottom) aabb.bottom = points[i].y;
		if (points[i].y > aabb.top) aabb.top = points[i].y;
	}
	return aabb;
}

UEngine::Math::Physics2D::AABB UEngine::Math::Physics2D::MakeAABB(PointCoord point)
{
	AABB aabb = AABB{ point.x, point.y, point.x, point.y };
	return aabb;
}

UEngine::Math::Physics2D::AABB UEngine::Math::Physics2D::MakeAABB(LineCoords line)
{
	AABB aabb = AABB{ line[0].x, line[0].y, line[0].x, line[0].y };
	if (line[1].x < aabb.left) aabb.left = line[1].x;
	if (line[1].x > aabb.right) aabb.right = line[1].x;
	if (line[1].y < aabb.bottom) aabb.bottom = line[1].y;
	if (line[1].y > aabb.top) aabb.top = line[1].y;
	return aabb;
}

UEngine::Math::Physics2D::AABB UEngine::Math::Physics2D::MakeAABB(TriangleCoords triangle)
{
	AABB aabb = AABB{ triangle[0].x, triangle[0].y, triangle[0].x, triangle[0].y };
	for (size_t i = 1; i < 3; i++)
	{
		if (triangle[i].x < aabb.left) aabb.left = triangle[i].x;
		if (triangle[i].x > aabb.right) aabb.right = triangle[i].x;
		if (triangle[i].y < aabb.bottom) aabb.bottom = triangle[i].y;
		if (triangle[i].y > aabb.top) aabb.top = triangle[i].y;
	}
	return aabb;
}

UEngine::Math::Physics2D::AABB UEngine::Math::Physics2D::MakeAABB(CircleCoord circle)
{
	AABB aabb = AABB
	{
		circle.center.x - circle.radius, 
		circle.center.y + circle.radius, 
		circle.center.x + circle.radius, 
		circle.center.y - circle.radius };
	return aabb;
}

UEngine::Math::Physics2D::CircleCoord UEngine::Math::Physics2D::MakeCircle(Vector2 center, float radius)
{
	Math::Physics2D::CircleCoord circle;
	circle.center = center;
	circle.radius = radius;
	return circle;
}

bool UEngine::Math::Physics2D::IsAABB1Smaller(AABB aabb1, AABB aabb2)
{
	if (aabb2.left < aabb1.left && aabb1.right < aabb2.right
		&& aabb2.bottom < aabb1.bottom && aabb1.top < aabb2.top)
		return true;
	return false;
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

bool UEngine::Math::Physics2D::IsColliding(PointCoord point, AABB aabb)
{
	if(aabb.left <= point.x && point.x <= aabb.right &&
		aabb.bottom <= point.y && point.y <= aabb.top)
		return true;
	return false;
}

bool UEngine::Math::Physics2D::IsColliding(PointCoord point, OBB obb)
{
	auto determinant = DirectX::XMMatrixDeterminant(obb.worldMatrix);
	Matrix inverseMatrix = DirectX::XMMatrixInverse(&determinant, obb.worldMatrix);
	point = point * inverseMatrix;
	if (obb.localVertices[0].x <= point.x && point.x <= obb.localVertices[3].x &&
		obb.localVertices[0].y <= point.y && point.y <= obb.localVertices[3].y)
		return true;
	return false;
}

bool UEngine::Math::Physics2D::IsColliding(PointCoord point, CircleCoord circle)
{
	float distance = (point - circle.center).Magnitude();
	if (distance > circle.radius)
		return false;
	return true;
}

bool UEngine::Math::Physics2D::IsColliding(LineCoords line, TriangleCoords triangle)
{
	if (IsColliding(line[0], triangle)) return true;
	if (IsColliding(line[1], triangle)) return true;
	if (IsIntersecting(line, LineCoords{ triangle[0], triangle[1] })) return true;
	if (IsIntersecting(line, LineCoords{ triangle[1], triangle[2] })) return true;
	if (IsIntersecting(line, LineCoords{ triangle[2], triangle[0] })) return true;

	return false;
}

bool UEngine::Math::Physics2D::IsColliding(LineCoords line, AABB aabb)
{
	if (IsColliding(line[0], aabb)) return true;
	if (IsColliding(line[1], aabb)) return true;
	PointCoord points[] =
	{
		PointCoord(aabb.left, aabb.bottom),
		PointCoord(aabb.left, aabb.top),
		PointCoord(aabb.right, aabb.top),
		PointCoord(aabb.right, aabb.bottom)
	};
	if (IsIntersecting(line, LineCoords{ points[0], points[1] })) return true;
	if (IsIntersecting(line, LineCoords{ points[1], points[2] })) return true;
	if (IsIntersecting(line, LineCoords{ points[2], points[3] })) return true;
	if (IsIntersecting(line, LineCoords{ points[3], points[0] })) return true;
	
	return false;
}

bool UEngine::Math::Physics2D::IsColliding(LineCoords line, OBB obb)
{
	auto determinant = DirectX::XMMatrixDeterminant(obb.worldMatrix);
	Matrix inverseMatrix = DirectX::XMMatrixInverse(&determinant, obb.worldMatrix);
	line[0] = line[0] * inverseMatrix;
	line[1] = line[1] * inverseMatrix;

	if (IsColliding(line[0], obb)) return true;
	if (IsColliding(line[1], obb)) return true;

	if (IsIntersecting(line, LineCoords{ obb.localVertices[0], obb.localVertices[1] })) return true;
	if (IsIntersecting(line, LineCoords{ obb.localVertices[1], obb.localVertices[2] })) return true;
	if (IsIntersecting(line, LineCoords{ obb.localVertices[2], obb.localVertices[3] })) return true;
	if (IsIntersecting(line, LineCoords{ obb.localVertices[3], obb.localVertices[0] })) return true;
	return false;
}

bool UEngine::Math::Physics2D::IsColliding(LineCoords line, CircleCoord circle)
{
	Vector2 direction = line[1] - line[0];
	float ratio = direction * (circle.center - line[0]);
	ratio = Clamp(ratio, 0, 1);
	Vector2 closestPoint = direction * ratio + line[0];
	float distance = (circle.center - closestPoint).Magnitude();
	return distance <= circle.radius;
}

bool UEngine::Math::Physics2D::IsColliding(TriangleCoords triangle1, TriangleCoords triangle2)
{
	if (IsColliding(triangle1[0], triangle2)) return true;
	if (IsColliding(triangle1[1], triangle2)) return true;
	if (IsColliding(triangle1[2], triangle2)) return true;

	if (IsColliding(triangle2[0], triangle1)) return true;
	if (IsColliding(triangle2[1], triangle1)) return true;
	if (IsColliding(triangle2[2], triangle1)) return true;

	if (IsIntersecting(LineCoords{ triangle1[0], triangle1[1] }, LineCoords{ triangle2[0], triangle2[1] })) return true;
	if (IsIntersecting(LineCoords{ triangle1[1], triangle1[2] }, LineCoords{ triangle2[0], triangle2[1] })) return true;
	if (IsIntersecting(LineCoords{ triangle1[2], triangle1[0] }, LineCoords{ triangle2[0], triangle2[1] })) return true;

	if (IsIntersecting(LineCoords{ triangle1[0], triangle1[1] }, LineCoords{ triangle2[1], triangle2[2] })) return true;
	if (IsIntersecting(LineCoords{ triangle1[1], triangle1[2] }, LineCoords{ triangle2[1], triangle2[2] })) return true;
	if (IsIntersecting(LineCoords{ triangle1[2], triangle1[0] }, LineCoords{ triangle2[1], triangle2[2] })) return true;

	if (IsIntersecting(LineCoords{ triangle1[0], triangle1[1] }, LineCoords{ triangle2[2], triangle2[0] })) return true;
	if (IsIntersecting(LineCoords{ triangle1[1], triangle1[2] }, LineCoords{ triangle2[2], triangle2[0] })) return true;
	if (IsIntersecting(LineCoords{ triangle1[2], triangle1[0] }, LineCoords{ triangle2[2], triangle2[0] })) return true;

	return false;
}

bool UEngine::Math::Physics2D::IsColliding(TriangleCoords triangle, AABB aabb)
{
	LineCoords lines[] =
	{
		LineCoords{triangle[0], triangle[1]},
		LineCoords{triangle[1], triangle[2]},
		LineCoords{triangle[2], triangle[0]}
	};
	PointCoord center = (triangle[0] + triangle[1] + triangle[2]) * (1 / 3.0f);

	if (IsColliding(center, aabb)) return true;
	if (IsColliding(lines[0], aabb)) return true;
	if (IsColliding(lines[1], aabb)) return true;
	if (IsColliding(lines[2], aabb)) return true;
	return false;
}

bool UEngine::Math::Physics2D::IsColliding(TriangleCoords triangle, OBB obb)
{
	auto determinant = DirectX::XMMatrixDeterminant(obb.worldMatrix);
	Matrix inverseMatrix = DirectX::XMMatrixInverse(&determinant, obb.worldMatrix);
	triangle[0] = triangle[0] * inverseMatrix;
	triangle[1] = triangle[1] * inverseMatrix;
	triangle[2] = triangle[2] * inverseMatrix;

	LineCoords lines[] =
	{
		LineCoords{triangle[0], triangle[1]},
		LineCoords{triangle[1], triangle[2]},
		LineCoords{triangle[2], triangle[0]}
	};
	PointCoord center = (triangle[0] + triangle[1] + triangle[2]) * (1 / 3.0f);

	if (IsColliding(center, obb)) return true;
	if (IsColliding(lines[0], obb)) return true;
	if (IsColliding(lines[1], obb)) return true;
	if (IsColliding(lines[2], obb)) return true;

	return false;
}

bool UEngine::Math::Physics2D::IsColliding(TriangleCoords triangle, CircleCoord circle)
{
	if (IsColliding(circle.center, triangle)) return true;
	LineCoords lines[] =
	{
		LineCoords{triangle[0], triangle[1]},
		LineCoords{triangle[1], triangle[2]},
		LineCoords{triangle[2], triangle[0]}
	};
	if (IsColliding(lines[0], circle)) return true;
	if (IsColliding(lines[1], circle)) return true;
	if (IsColliding(lines[2], circle)) return true;
	return false;
}

bool UEngine::Math::Physics2D::IsColliding(AABB aabb1, AABB aabb2)
{
	if (aabb1.right < aabb2.left || aabb1.left > aabb2.right) return false;
	if (aabb1.top < aabb2.bottom || aabb1.bottom > aabb2.top) return false;
	return true;
}

bool UEngine::Math::Physics2D::IsColliding(AABB aabb, OBB obb)
{
	OBB obb1;
	obb1.localVertices[0] = Vector2(aabb.left, aabb.bottom);
	obb1.localVertices[1] = Vector2(aabb.left, aabb.top);
	obb1.localVertices[2] = Vector2(aabb.right, aabb.top);
	obb1.localVertices[3] = Vector2(aabb.right, aabb.bottom);
	obb1.worldMatrix = DirectX::XMMatrixIdentity();

	return IsColliding(obb1, obb);
}

bool UEngine::Math::Physics2D::IsColliding(AABB aabb, CircleCoord circle)
{
	if (IsColliding(circle.center, aabb)) return true;
	Vector2 closestPoint;
	closestPoint.x = Clamp(circle.center.x, aabb.left, aabb.right);
	closestPoint.y = Clamp(circle.center.y, aabb.bottom, aabb.top);
	float distance = (circle.center - closestPoint).Magnitude();
	return distance <= circle.radius;
}

bool UEngine::Math::Physics2D::IsColliding(OBB obb1, OBB obb2)
{
	using namespace DirectX;
	Vector2 vertices1[4], vertices2[4];
	Vector2 xaxis, yaxis;
	float dist1, dist2;
	float min1, min2, max1, max2;
	// xaxis
	min1 = min2 = FLT_MAX;
	max1 = max2 = -FLT_MAX;

	xaxis = XMVector3Normalize(obb1.worldMatrix.r[0]);
	yaxis = XMVector3Normalize(obb1.worldMatrix.r[1]);
	for (int i = 0; i < 4; i++)
	{
		vertices1[i] = obb1.localVertices[i] * obb1.worldMatrix;
		vertices2[i] = obb2.localVertices[i] * obb2.worldMatrix;

		dist1 = xaxis * vertices1[i];
		dist2 = xaxis * vertices2[i];

		min1 = min1 > dist1 ? dist1 : min1;
		max1 = max1 < dist1 ? dist1 : max1;
		min2 = min2 > dist2 ? dist2 : min2;
		max2 = max2 < dist2 ? dist2 : max2;
	}
	float center1 = (max1 + min1) / 2.0f;
	float center2 = (max2 + min2) / 2.0f;
	if ((max2 - center2) + (max1 - center1) < abs(center2 - center1))
		return false;

	// yaxis
	min1 = max1 = yaxis * vertices1[0];
	min2 = max2 = yaxis * vertices2[0];
	for (int i = 1; i < 4; i++)
	{
		dist1 = yaxis * vertices1[i];
		dist2 = yaxis * vertices2[i];

		min1 = min1 > dist1 ? dist1 : min1;
		max1 = max1 < dist1 ? dist1 : max1;
		min2 = min2 > dist2 ? dist2 : min2;
		max2 = max2 < dist2 ? dist2 : max2;
	}
	center1 = (max1 + min1) / 2.0f;
	center2 = (max2 + min2) / 2.0f;
	if ((max2 - center2) + (max1 - center1) < abs(center2 - center1))
		return false;

	xaxis = XMVector3Normalize(obb2.worldMatrix.r[0]);
	yaxis = XMVector3Normalize(obb2.worldMatrix.r[1]);
	// xaxis
	min1 = max1 = xaxis * vertices1[0];
	min2 = max2 = xaxis * vertices2[0];
	for (int i = 1; i < 4; i++)
	{
		dist1 = xaxis * vertices1[i];
		dist2 = xaxis * vertices2[i];

		min1 = min1 > dist1 ? dist1 : min1;
		max1 = max1 < dist1 ? dist1 : max1;
		min2 = min2 > dist2 ? dist2 : min2;
		max2 = max2 < dist2 ? dist2 : max2;
	}
	center1 = (max1 + min1) / 2.0f;
	center2 = (max2 + min2) / 2.0f;
	if ((max2 - center2) + (max1 - center1) < abs(center2 - center1))
		return false;

	// yaxis
	min1 = max1 = yaxis * vertices1[0];
	min2 = max2 = yaxis * vertices2[0];
	for (int i = 1; i < 4; i++)
	{
		dist1 = yaxis * vertices1[i];
		dist2 = yaxis * vertices2[i];

		min1 = min1 > dist1 ? dist1 : min1;
		max1 = max1 < dist1 ? dist1 : max1;
		min2 = min2 > dist2 ? dist2 : min2;
		max2 = max2 < dist2 ? dist2 : max2;
	}
	center1 = (max1 + min1) / 2.0f;
	center2 = (max2 + min2) / 2.0f;
	if ((max2 - center2) + (max1 - center1) < abs(center2 - center1))
		return false;

	return true;
}

bool UEngine::Math::Physics2D::IsColliding(OBB obb, CircleCoord circle)
{
	auto determinant = DirectX::XMMatrixDeterminant(obb.worldMatrix);
	Matrix inverseMatrix = DirectX::XMMatrixInverse(&determinant, obb.worldMatrix);
	circle.center = circle.center * inverseMatrix;

	if (IsColliding(circle.center, obb)) return true;
	Vector2 closestPoint;
	closestPoint.x = Clamp(circle.center.x, obb.localVertices[0].x, obb.localVertices[3].x);
	closestPoint.y = Clamp(circle.center.y, obb.localVertices[0].y, obb.localVertices[3].y);
	float distance = (circle.center - closestPoint).Magnitude();
	return distance <= circle.radius;
}

bool UEngine::Math::Physics2D::IsColliding(CircleCoord circle1, CircleCoord circle2)
{
	float distance = (circle1.center - circle2.center).Magnitude();
	float maxDistance = circle1.radius + circle2.radius;
	return distance <= maxDistance;
}

UEngine::Math::Physics2D::CollisionResult UEngine::Math::Physics2D::FindColliding
(
	UEngine::Physics2D::RectCollider* rect1, 
	UEngine::Physics2D::RectCollider* rect2
)
{
	CollisionResult result;
	if (rect1->GetCollider().right <= rect2->GetCollider().left || rect1->GetCollider().left >= rect2->GetCollider().right)
	{
		result.isColliding = false;
		return result;
	}
	if (rect1->GetCollider().top <= rect2->GetCollider().bottom || rect1->GetCollider().bottom >= rect2->GetCollider().top)
	{
		result.isColliding = false;
		return result;
	}

	Vector2 center1 = Vector2
	(
		(rect1->GetCollider().right + rect1->GetCollider().left) / 2.0f,
		(rect1->GetCollider().top + rect1->GetCollider().bottom) / 2.0f
	);
	Vector2 center2 = Vector2
	(
		(rect2->GetCollider().right + rect2->GetCollider().left) / 2.0f,
		(rect2->GetCollider().top + rect2->GetCollider().bottom) / 2.0f
	);

	Vector2 closestPoint1;
	closestPoint1.x = Clamp(center1.x, rect2->GetCollider().left, rect2->GetCollider().right);
	closestPoint1.y = Clamp(center1.y, rect2->GetCollider().bottom, rect2->GetCollider().top);

	Vector2 closestPoint2;
	closestPoint2.x = Clamp(center2.x, rect1->GetCollider().left, rect1->GetCollider().right);
	closestPoint2.y = Clamp(center2.y, rect1->GetCollider().bottom, rect1->GetCollider().top);

	Vector2 dist = closestPoint2 - closestPoint1;
	Vector2 realDist;

	realDist.x = abs(dist.x) > abs(dist.y) ? 0 : dist.x;
	realDist.y = abs(dist.x) < abs(dist.y) ? 0 : dist.y;
	if (dist.x == 0 || dist.y == 0)
		realDist = dist;
	result.isColliding = true;
	result.distance1 = realDist * -0.5f;
	result.distance2 = realDist * 0.5f;

	return result;
}

UEngine::Math::Physics2D::CollisionResult UEngine::Math::Physics2D::FindColliding
(
	UEngine::Physics2D::RectCollider* rectangle, 
	UEngine::Physics2D::CircleCollider* circle
)
{
	CollisionResult result;

	Vector2 closestPoint;
	closestPoint.x = Clamp(circle->GetCollider().center.x, rectangle->GetCollider().left, rectangle->GetCollider().right);
	closestPoint.y = Clamp(circle->GetCollider().center.y, rectangle->GetCollider().bottom, rectangle->GetCollider().top);
	Vector2 dir = closestPoint - circle->GetCollider().center;
	float distance = dir.Magnitude();
	result.isColliding = distance <= circle->GetCollider().radius;

	if (!result.isColliding) return result;

	float distToMove = (circle->GetCollider().radius - distance) / 2.0f + 0.00001f;
	result.isColliding = true;
	result.distance1 = dir.Normalize() * -distToMove;
	result.distance2 = dir.Normalize() * distToMove;

	return result;
}

UEngine::Math::Physics2D::CollisionResult UEngine::Math::Physics2D::FindColliding
(
	UEngine::Physics2D::CircleCollider* circle1, 
	UEngine::Physics2D::CircleCollider* circle2
)
{
	CollisionResult result;
	
	auto transform = circle1->GetTransform();
	result.distance1 = circle1->GetCollider().center - circle2->GetCollider().center;
	float magnitude = result.distance1.Magnitude();
	float maxDistance = circle1->GetCollider().radius + circle2->GetCollider().radius;
	result.isColliding = magnitude <= maxDistance;

	if (!result.isColliding) return result;

	result.distance2 = result.distance1 * -1.0f;

	float distance = (maxDistance - abs(magnitude)) / 2.0f;
	if (magnitude == 0)
	{
		result.distance1 = Vector2(Math::RndFloat(0, 2) - 1.0f, Math::RndFloat(0, 2) - 1.0f);
		result.distance2 = Vector2(Math::RndFloat(0, 2) - 1.0f, Math::RndFloat(0, 2) - 1.0f);
	}

	result.isColliding = true;
	result.distance1 = result.distance1.Normalize() * distance;
	result.distance2 = result.distance2.Normalize() * distance;

	return result;
}

const UEngine::Vector2 UEngine::Math::GetMousePosToWorld()
{
	using namespace DirectX;
	// mouse ndc position to world position = mousePos * Projection^-1 * View^-1
	auto mousePos = Input::GetMousePos();
	
	if (Camera::GetMainCamera())
	{
		auto cpu_camera = Camera::GetMainCamera()->GetCameraMatrix();
		Vector2 vMousePos = mousePos;
		auto pDet = XMMatrixDeterminant(cpu_camera.projection);
		auto vDet = XMMatrixDeterminant(cpu_camera.view);
		vMousePos = vMousePos * XMMatrixInverse(&pDet, cpu_camera.projection);
		vMousePos = vMousePos * XMMatrixInverse(&vDet, cpu_camera.view);
		return vMousePos;
	}
	else return Vector2();
}

const float UEngine::Math::Clamp(float value, float min, float max)
{
	return value < min ? min : value > max ? max : value;
}

bool randInstantiated = false;
int UEngine::Math::RndInt(int start, int end)
{
	if (!randInstantiated)
	{
		srand((unsigned)time(nullptr));
		randInstantiated = true;
	}
	return rand() % (end - start + 1) + start;
};

float UEngine::Math::RndFloat(float start, float end)
{
	if (!randInstantiated)
	{
		srand((unsigned)time(nullptr));
		randInstantiated = true;
	}
	return (float)rand() / (float)RAND_MAX * (end - start) + start;
};
