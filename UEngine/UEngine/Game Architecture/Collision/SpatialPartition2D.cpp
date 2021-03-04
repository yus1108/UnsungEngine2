#include "UEngine.h"
#include "SpatialPartition2D.h"

namespace UEngine
{
	COMPARE_AABB_SIZE_RESULT SpatialPartition2D::CompareAABBSize(AABB aabb1, AABB aabb2)
	{
		auto width1 = abs(aabb1.right - aabb1.left);
		auto height1 = abs(aabb1.top - aabb1.bottom);
		auto width2 = abs(aabb2.right - aabb2.left);
		auto height2 = abs(aabb2.top - aabb2.bottom);

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
			head = new SPACE_PARTITIONING_NODE;
			head->aabb = headAABB;
			nodeManager.emplace_back(head);
		}

		AddNode(head, collider);
	}

	void SpatialPartition2D::AddNode(SpatialPartition2D::SPACE_PARTITIONING_NODE* currNode, Collider* collider)
	{
		if (IsColliding(currNode->aabb, collider->GetWorldAABB()))
		{
			for (auto otherPair : currNode->colliders)
			{
				auto other = otherPair.second;
				auto oGameObject = otherPair.second->GetGameObject();
				if (oGameObject != collider->GetGameObject())
				{
					// TODO: need to change to proper collider component
					if (collider->others.find(other) == collider->others.end())
					{
						collider->isAABBColliding = true;
						otherPair.second->isAABBColliding = true;
						/*if (Math::Physics2D::IsColliding(script2->circle, script1->circle))
						{
							script1->collideTimer = 0.0f;
							script2->collideTimer = 0.0f;
							auto transform = collider->gameObject->GetTransform();
							script1->dir = (transform->localPosition - oGameObject->GetTransform()->localPosition).Normalize();
							script2->dir = script1->dir * -1.0f;
							if (script1->dir.Magnitude() == 0)
							{
								script1->dir = Vector2(Math::RndFloat(0, 2) - 1.0f, Math::RndFloat(0, 2) - 1.0f).Normalize();
								script2->dir = Vector2(Math::RndFloat(0, 2) - 1.0f, Math::RndFloat(0, 2) - 1.0f).Normalize();
							}

							collider->others[other] = other;
							other->others[collider] = collider;
						}*/
					}
				}
			}
			auto result = CompareAABBSize(currNode->aabb, collider->GetWorldAABB());
			if (result == COMPARE_AABB_SIZE_RESULT_BIGGER)
			{
				if (currNode->children.size() == 0) { MakeQuadGrid(currNode); }
				for (auto child : currNode->children)
					AddNode(child, collider);
			}
			else
			{
				currNode->colliders[collider] = collider;
				for (auto child : currNode->children)
					CheckCollision(child, collider);
			}

		}
	}

	void SpatialPartition2D::CheckCollision(SPACE_PARTITIONING_NODE* currNode, Collider* collider)
	{
		if (IsColliding(currNode->aabb, collider->GetWorldAABB()))
		{
			for (auto otherPair : currNode->colliders)
			{
				auto other = otherPair.second;
				auto oGameObject = otherPair.second->GetGameObject();
				if (oGameObject != collider->GetGameObject())
				{
					// TODO: need to change to proper collider component
					if (collider->others.find(other) == collider->others.end())
					{
						collider->isAABBColliding = true;
						otherPair.second->isAABBColliding = true;
						/*if (Math::Physics2D::IsColliding(script2->circle, script1->circle))
						{
							script1->collideTimer = 0.0f;
							script2->collideTimer = 0.0f;
							auto transform = collider->gameObject->GetTransform();
							script1->dir = (transform->localPosition - oGameObject->GetTransform()->localPosition).Normalize();
							script2->dir = script1->dir * -1.0f;
							if (script1->dir.Magnitude() == 0)
							{
								script1->dir = Vector2(Math::RndFloat(0, 2) - 1.0f, Math::RndFloat(0, 2) - 1.0f).Normalize();
								script2->dir = Vector2(Math::RndFloat(0, 2) - 1.0f, Math::RndFloat(0, 2) - 1.0f).Normalize();
							}

							collider->others[other] = other;
							other->others[collider] = collider;
						}*/
					}
				}
			}
			for (auto child : currNode->children)
				CheckCollision(child, collider);
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
			GameState::Get()->GetDebugRenderer()->Add_Rectangle(currNode->aabb, nodeColor);
			for (auto colliderPair : currNode->colliders)
				GameState::Get()->GetDebugRenderer()->Add_Rectangle(colliderPair.second->GetWorldAABB(), colliderColor);
			for (auto child : currNode->children)
				DebugRender(child, collider, nodeColor, colliderColor);
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
			for (auto colliderPair : currNode->colliders)
			{
				GameState::Get()->GetDebugRenderer()->Add_line
				(
					{
						colliderPair.second->GetTransform()->localPosition,
						collider->GetTransform()->localPosition
					},
					color
				);
			}
			for (auto child : currNode->children)
				DebugRender(child, collider, color);
		}
	}

	void SpatialPartition2D::Release()
	{
		for (auto item : nodeManager)
			delete item;
		nodeManager.clear();
		head = nullptr;
		headAABB = AABB{ FLT_MAX, -FLT_MAX, -FLT_MAX, FLT_MAX };
	}

	void UEngine::SpatialPartition2D::EnlargeHead(AABB aabb)
	{
		headAABB = EnlargeGrid(headAABB, aabb);
	}

	void SpatialPartition2D::MakeQuadGrid(SpatialPartition2D::SPACE_PARTITIONING_NODE* node)
	{
		auto width = abs(node->aabb.right - node->aabb.left);
		auto height = abs(node->aabb.top - node->aabb.bottom);
		auto midX = node->aabb.left + width / 2.0f;
		auto midY = node->aabb.bottom + height / 2.0f;

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

		node->children.reserve(4);
		node->children.emplace_back(new SPACE_PARTITIONING_NODE);
		node->children.emplace_back(new SPACE_PARTITIONING_NODE);
		node->children.emplace_back(new SPACE_PARTITIONING_NODE);
		node->children.emplace_back(new SPACE_PARTITIONING_NODE);

		nodeManager.emplace_back(node->children[0]);
		nodeManager.emplace_back(node->children[1]);
		nodeManager.emplace_back(node->children[2]);
		nodeManager.emplace_back(node->children[3]);

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
}

