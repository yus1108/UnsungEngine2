#pragma once

namespace UEngine
{
	using namespace Math;
	using namespace Math::Physics2D;
	class SpatialPartition2D
	{
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
		AABB EnlargeGrid(AABB aabb1, AABB aabb2); // TODO: use compute shader to boost speed

	public:
		AABB headAABB{ FLT_MAX, -FLT_MAX, -FLT_MAX, FLT_MAX };
		SPACE_PARTITIONING_NODE* head;

		SpatialPartition2D() : head(nullptr) {}
		~SpatialPartition2D() { Release(); }

		COMPARE_AABB_SIZE_RESULT CompareAABBSize(AABB aabb1, AABB aabb2);

		void Release();
		void EnlargeHead(AABB aabb);
		void ConstructNode(Collider* collider);
		void CheckCollision(SPACE_PARTITIONING_NODE* currNode, Collider* collider);
		void DebugRender(SPACE_PARTITIONING_NODE* currNode, Collider* collider, UEngine::Color nodeColor, UEngine::Color colliderColor);
		void DebugRender(SPACE_PARTITIONING_NODE* currNode, Collider* collider, UEngine::Color color);
	};
}



