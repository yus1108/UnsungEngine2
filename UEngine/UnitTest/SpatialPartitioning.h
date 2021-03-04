#pragma once

using namespace UEngine;
using namespace UEngine::Math::Physics2D;
class SpatialPartitioning
{
public:
	struct Collider
	{
		AABB aabb{ 0 };
		GameObject* gameObject{ nullptr };
		std::map<Collider*, Collider*> others;
		// 충돌체 변수
	};

	enum COMPARE_AABB_SIZE_RESULT
	{
		COMPARE_AABB_SIZE_RESULT_EQUAL,
		COMPARE_AABB_SIZE_RESULT_SMALLER,
		COMPARE_AABB_SIZE_RESULT_BIGGER,
		COMPARE_AABB_SIZE_RESULT_DIFFERENT
	};
private:
	struct SPACE_PARTITIONING_NODE
	{
		AABB aabb{ 0 };
		SPACE_PARTITIONING_NODE* parent{ nullptr };
		std::vector<SPACE_PARTITIONING_NODE*> children;
		std::map<Collider*, Collider*> colliders;
	};

	std::list<SPACE_PARTITIONING_NODE*> nodeManager;

	void AddNode(SPACE_PARTITIONING_NODE* currNode, Collider* collider);
	void MakeQuadGrid(SPACE_PARTITIONING_NODE* node);

public:
	SPACE_PARTITIONING_NODE* head;

	SpatialPartitioning() : head(nullptr) {}
	~SpatialPartitioning() { Release(); }

	AABB EnlargeGrid(AABB aabb1, AABB aabb2);
	COMPARE_AABB_SIZE_RESULT CompareAABBSize(AABB aabb1, AABB aabb2);

	void Release();
	void ConstructNode(AABB worldAABB, const std::vector<Collider*>& collider);
	void CheckCollision(SPACE_PARTITIONING_NODE* currNode, Collider* collider);
	void DebugRender(SPACE_PARTITIONING_NODE* currNode, Collider* collider, UEngine::Color nodeColor, UEngine::Color colliderColor);
	void DebugRender(SPACE_PARTITIONING_NODE* currNode, Collider* collider, UEngine::Color color);

};

