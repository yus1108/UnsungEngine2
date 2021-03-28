#pragma once
#include "../Utility/Containers/UVector.h"

namespace UEngine
{
	namespace Physics2D
	{
		using namespace Math;
		using namespace Math::Physics2D;
		class SpatialPartition2D
		{
		public:
			struct SPACE_PARTITIONING_NODE
			{
				AABB aabb{ 0 };
				SPACE_PARTITIONING_NODE* parent{ nullptr };
				Utility::UVector<SPACE_PARTITIONING_NODE*> children;
				Utility::UVector<class Collider*> colliders;
			};

		private:
			std::vector<SPACE_PARTITIONING_NODE*> nodeManager;

			void AddNode(SPACE_PARTITIONING_NODE* currNode, Collider* collider);
			void MakeQuadGrid(SPACE_PARTITIONING_NODE* node);
			AABB EnlargeGrid(AABB aabb1, AABB aabb2); // TODO: use compute shader to boost speed
			SPACE_PARTITIONING_NODE* RequestNode();

		public:
			size_t nodeCounter = 0;
			size_t maxColliderPerNode = 0;
			AABB headAABB{ FLT_MAX, -FLT_MAX, -FLT_MAX, FLT_MAX };
			SPACE_PARTITIONING_NODE* head;

			SpatialPartition2D() : head(nullptr) {}
			~SpatialPartition2D();

			COMPARE_AABB_SIZE_RESULT CompareAABBSize(AABB aabb1, AABB aabb2);

			void Release();
			void EnlargeHead(AABB aabb);
			void ConstructNode(Collider* collider);
			void CheckCollision(SPACE_PARTITIONING_NODE* currNode, Collider* collider);
			void DebugRender(SPACE_PARTITIONING_NODE* currNode, Collider* collider, UEngine::Color nodeColor, UEngine::Color colliderColor);
			void DebugRender(SPACE_PARTITIONING_NODE* currNode, Collider* collider, UEngine::Color color);
		};
	}
}



