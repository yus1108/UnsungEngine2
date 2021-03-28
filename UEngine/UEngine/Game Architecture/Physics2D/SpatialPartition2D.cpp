#include "UEngine.h"
#include "SpatialPartition2D.h"

namespace UEngine
{
	namespace Physics2D
	{
		SpatialPartition2D::~SpatialPartition2D()
		{
			for (auto item : nodeManager)
				delete item;
			Release();
		}

		COMPARE_AABB_SIZE_RESULT SpatialPartition2D::CompareAABBSize(AABB aabb1, AABB aabb2)
		{
			auto width1 = aabb1.right - aabb1.left;
			auto height1 = aabb1.top - aabb1.bottom;
			auto width2 = aabb2.right - aabb2.left;
			auto height2 = aabb2.top - aabb2.bottom;

			width1 = width1 < 0 ? width1 * -1.0f : width1;
			height1 = height1 < 0 ? height1 * -1.0f : height1;
			width2 = width2 < 0 ? width2 * -1.0f : width2;
			height2 = height2 < 0 ? height2 * -1.0f : height2;

			if (width1 > width2 && height1 > height2)
				return COMPARE_AABB_SIZE_RESULT_BIGGER;
			else if (width1 < width2 && height1 < height2)
				return COMPARE_AABB_SIZE_RESULT_SMALLER;
			else if (width1 == width2 && height1 == height2)
				return COMPARE_AABB_SIZE_RESULT_EQUAL;
			return COMPARE_AABB_SIZE_RESULT_DIFFERENT;
		}


		void SpatialPartition2D::ConstructNode(Collider* collider)
		{
			if (head == nullptr)
			{
				head = RequestNode();
				head->aabb = headAABB;
				head->parent = nullptr;
				maxColliderPerNode = 0;
			}

			AddNode(head, collider);
		}

		void SpatialPartition2D::AddNode(SpatialPartition2D::SPACE_PARTITIONING_NODE* currNode, Collider* collider)
		{
			if (IsColliding(currNode->aabb, collider->GetWorldAABB()))
			{
				for (size_t i = 0; i < currNode->colliders.size(); i++)
				{
					if (currNode->colliders[i]->GetGameObject() != collider->GetGameObject())
					{
						// TODO: need to change to proper collider component
						collider->CalculateImpact(currNode->colliders[i]);
					}
				}
				auto result = CompareAABBSize(currNode->aabb, collider->GetWorldAABB());
				if (result == COMPARE_AABB_SIZE_RESULT_BIGGER)
				{
					if (currNode->children.size() == 0) { MakeQuadGrid(currNode); }
					for (size_t i = 0; i < currNode->children.size(); i++)
						AddNode(currNode->children[i], collider);
				}
				else
				{
					currNode->colliders.push_back(collider);
					if (currNode->colliders.size() > maxColliderPerNode)
						maxColliderPerNode = currNode->colliders.size();
					for (size_t i = 0; i < currNode->children.size(); i++)
						CheckCollision(currNode->children[i], collider);
				}

			}
		}

		void SpatialPartition2D::CheckCollision(SPACE_PARTITIONING_NODE* currNode, Collider* collider)
		{
			if (IsColliding(currNode->aabb, collider->GetWorldAABB()))
			{
				if (currNode->colliders.size() > maxColliderPerNode)
					maxColliderPerNode = currNode->colliders.size();
				for (size_t i = 0; i < currNode->colliders.size(); i++)
				{
					if (currNode->colliders[i]->GetGameObject() != collider->GetGameObject())
					{
						// TODO: need to change to proper collider component
						collider->CalculateImpact(currNode->colliders[i]);
					}
				}
				for (size_t i = 0; i < currNode->children.size(); i++)
					CheckCollision(currNode->children[i], collider);
			}
		}

		void SpatialPartition2D::DebugRender
		(
			SPACE_PARTITIONING_NODE* currNode,
			Collider* collider,
			UEngine::Color nodeColor,
			UEngine::Color colliderColor
		)
		{
			if (currNode == nullptr) return;
			if (IsColliding(currNode->aabb, collider->GetWorldAABB()))
			{
				// TODO: spatial partitioning need to know debugrenderer in the same scene
				GameState::GetCurrentScene()->debugRenderer->Add_Rectangle(currNode->aabb, nodeColor);
				for (size_t i = 0; i < currNode->colliders.size(); i++)
				{
					if (currNode->colliders[i]->typeName == ".PAVCircleCollider@Physics2D@UEngine@@")
						GameState::GetCurrentScene()->debugRenderer->Add_Circle(static_cast<CircleCollider*>(currNode->colliders[i])->GetCollider(), colliderColor);
					else
						GameState::GetCurrentScene()->debugRenderer->Add_Rectangle(currNode->colliders[i]->GetWorldAABB(), colliderColor);
				}
			
				for (size_t i = 0; i < currNode->children.size(); i++)
					DebugRender(currNode->children[i], collider, nodeColor, colliderColor);
			}
		}

		void SpatialPartition2D::DebugRender
		(
			SPACE_PARTITIONING_NODE* currNode,
			Collider* collider,
			UEngine::Color color
		)
		{
			if (currNode == nullptr) return;
			if (IsColliding(currNode->aabb, collider->GetWorldAABB()))
			{
				for (size_t i = 0; i < currNode->colliders.size(); i++)
				{
					GameState::GetCurrentScene()->debugRenderer->Add_line
					(
						{
							currNode->colliders[i]->GetTransform()->localPosition.value,
							collider->GetTransform()->localPosition.value
						},
						color
					);
				}
				for (size_t i = 0; i < currNode->children.size(); i++)
					DebugRender(currNode->children[i], collider, color);
			}
		}

		void SpatialPartition2D::Release()
		{
			nodeCounter = 0;
			head = nullptr;
			headAABB = AABB{ FLT_MAX, -FLT_MAX, -FLT_MAX, FLT_MAX };
		}

		void SpatialPartition2D::EnlargeHead(AABB aabb)
		{
			headAABB = EnlargeGrid(headAABB, aabb);
		}

		void SpatialPartition2D::MakeQuadGrid(SpatialPartition2D::SPACE_PARTITIONING_NODE* node)
		{
			auto width = node->aabb.right - node->aabb.left;
			auto height = node->aabb.top - node->aabb.bottom;
			auto midX = node->aabb.left + width / 2.0f;
			auto midY = node->aabb.bottom + height / 2.0f;

			width = width < 0 ? width * -1.0f : width;
			height = height < 0 ? height * -1.0f : height;

			AABB alb, alt, art, arb;
			alb.left = node->aabb.left;
			alb.top = midY;
			alb.right = midX;
			alb.bottom = node->aabb.bottom;

			alt.left = node->aabb.left;
			alt.top = node->aabb.top;
			alt.right = midX;
			alt.bottom = midY;

			art.left = midX;
			art.top = node->aabb.top;
			art.right = node->aabb.right;
			art.bottom = midY;

			arb.left = midX;
			arb.top = midY;
			arb.right = node->aabb.right;
			arb.bottom = node->aabb.bottom;

			if (node->children.capacity() != 4)
				node->children.reserve(4);
			node->children.push_back(RequestNode());
			node->children.push_back(RequestNode());
			node->children.push_back(RequestNode());
			node->children.push_back(RequestNode());

			node->children[0]->aabb = alb;
			node->children[1]->aabb = alt;
			node->children[2]->aabb = art;
			node->children[3]->aabb = arb;

			node->children[0]->parent = node;
			node->children[1]->parent = node;
			node->children[2]->parent = node;
			node->children[3]->parent = node;
		}

		AABB SpatialPartition2D::EnlargeGrid(AABB aabb1, AABB aabb2)
		{
			aabb1.left = min(aabb1.left, aabb2.left);
			aabb1.right = max(aabb1.right, aabb2.right);
			aabb1.bottom = min(aabb1.bottom, aabb2.bottom);
			aabb1.top = max(aabb1.top, aabb2.top);
			return aabb1;
		}

		SpatialPartition2D::SPACE_PARTITIONING_NODE* SpatialPartition2D::RequestNode()
		{
			if (nodeCounter == nodeManager.size())
				nodeManager.emplace_back(new SPACE_PARTITIONING_NODE);
			else
			{
				nodeManager[nodeCounter]->colliders.clear();
				nodeManager[nodeCounter]->children.clear();
			}
			return nodeManager[nodeCounter++];
		}
	}
}
	