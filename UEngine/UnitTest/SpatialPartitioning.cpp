#include "framework.h"
#include "SpatialPartitioning.h"

using namespace UEngine;
SpatialPartitioning::SpatialPartitioning() : head(nullptr)
{
}

SpatialPartitioning::~SpatialPartitioning()
{
	for (auto item : manager)
		delete item;
}

void SpatialPartitioning::DrawGrid(GRID grid, UEngine::Color color)
{
	UEngine::Math::Physics2D::AABB aabb;
	aabb.left = static_cast<float>(grid.left);
	aabb.right = static_cast<float>(grid.right);
	aabb.top = static_cast<float>(grid.top);
	aabb.bottom = static_cast<float>(grid.bottom);
	GameState::Get()->debugRenderer.Add_Rectangle(aabb, color);
}

void SpatialPartitioning::DrawGrid(int x, int y)
{
	auto color = Color{ 1, 0, 0, 1 };
	auto worldMatrix = MatrixIdentity();
	worldMatrix.r[3] = DirectX::XMVectorSet(x + 0.5f, y + 0.5f, 0, 1);
	auto aabb = Math::Physics2D::MakeAABB(worldMatrix);
	GameState::Get()->debugRenderer.Add_Rectangle(aabb, color);
}

int SpatialPartitioning::CompareGrid(SpatialPartitioning::GRID grid1, SpatialPartitioning::GRID grid2)
{
	if (grid2.left < grid1.left && grid1.right < grid2.right
		&& grid2.bottom < grid1.bottom && grid1.top < grid2.top)
		return -1;
	else if (grid1.left < grid2.left && grid2.right < grid1.right
		&& grid1.bottom < grid2.bottom && grid2.top < grid1.top)
		return 1;
	else if (grid1.left == grid2.left && grid2.right == grid1.right
		&& grid1.bottom == grid2.bottom && grid2.top == grid1.top)
		return 0;
	return 2;
}

bool SpatialPartitioning::IsColliding(GRID grid1, GRID grid2)
{
	if (grid1.right < grid2.left || grid1.left > grid2.right) return false;
	if (grid1.top < grid2.bottom || grid1.bottom > grid2.top) return false;
	return true;
}

void SpatialPartitioning::ConstructNode(UEngine::Math::Physics2D::AABB aabb, UEngine::GameObject* gameObject)
{
	auto newNode = new SPACE_PARTITIONING_NODE;
	manager.emplace_back(newNode);
	newNode->parent = nullptr;
	newNode->grid = MakeGrid(aabb);
	newNode->gameObjects.emplace_back(gameObject);
	if (head == nullptr)
	{
		head = newNode;
		return;
	}

	AddNode(head, newNode);
}

void SpatialPartitioning::AddNode(SPACE_PARTITIONING_NODE* currNode, SPACE_PARTITIONING_NODE* newNode)
{
	if (IsColliding(currNode->grid, newNode->grid))
	{
		if (currNode->children.size() == 0)
		{
			auto comp = CompareGrid(newNode->grid, currNode->grid);
			if (comp == -1)
			{
				currNode->children.emplace_back(newNode);
				newNode->parent = currNode;
			}
			else if (comp == 1)
			{
				auto parent = currNode->parent;
				newNode->children.emplace_back(currNode);
				newNode->parent = parent;
				currNode->parent = newNode;
				if (parent == nullptr) head = newNode;
				else
				{
					for (auto node = parent->children.begin(); node != parent->children.end(); node++)
					{
						if (*node == currNode)
						{
							currNode->parent->children.erase(node);
							break;
						}
					}
					auto node = parent;
					while (node != nullptr)
					{
						node->grid = EnlargeGrid(node->grid, newNode->grid);
						node = node->parent;
					}
				}
			}
			else
			{
				auto parent = currNode->parent;
				SPACE_PARTITIONING_NODE* upperNode = new SPACE_PARTITIONING_NODE;
				manager.emplace_back(upperNode);
				upperNode->children.emplace_back(newNode);
				upperNode->children.emplace_back(currNode);
				upperNode->parent = parent;
				upperNode->grid = newNode->grid;
				newNode->parent = upperNode;
				currNode->parent = upperNode;
				if (parent == nullptr) head = upperNode;
				else
				{
					for (auto node = parent->children.begin(); node != parent->children.end(); node++)
					{
						if (*node == currNode)
						{
							parent->children.erase(node);
							break;
						}
					}
				}
				if (comp == 2)
				{
					upperNode->grid = EnlargeGrid(currNode->grid, upperNode->grid);
					auto node = upperNode->parent;
					while (node != nullptr)
					{
						node->grid = EnlargeGrid(node->grid, upperNode->grid);
						node = node->parent;
					}
				}
			}
		}
		else
		{
			auto parent = currNode;
			bool noCollision = true;
			SPACE_PARTITIONING_NODE* enlargedNode = nullptr;
			std::queue<std::list<SPACE_PARTITIONING_NODE*>::iterator> reservedDeletion;
			for (auto childNode = currNode->children.begin(); childNode != currNode->children.end(); childNode++)
			{
				if (IsColliding((*childNode)->grid, newNode->grid))
				{
					noCollision = false;
					auto comp = CompareGrid(newNode->grid, (*childNode)->grid);
					if (comp == -1)
					{
						AddNode((*childNode), newNode);
						return;
					}
					else if (comp == 1)
					{
						newNode->parent = parent;
						newNode->children.emplace_back((*childNode));
						(*childNode)->parent = newNode;
						parent->children.emplace_back(newNode);
						parent->children.erase(childNode);
						auto node = parent;
						while (node != nullptr)
						{
							node->grid = EnlargeGrid(node->grid, newNode->grid);
							node = node->parent;
						}
						return;
					}
					else
					{
						if (enlargedNode == nullptr)
						{
							enlargedNode = new SPACE_PARTITIONING_NODE;
							manager.emplace_back(enlargedNode);
							enlargedNode->grid = newNode->grid;
							enlargedNode->parent = parent;
							enlargedNode->children.emplace_back(newNode);
							newNode->parent = enlargedNode;
						}
						(*childNode)->parent = enlargedNode;
						enlargedNode->children.emplace_back((*childNode));
						reservedDeletion.push(childNode);

						if (comp == 2) enlargedNode->grid = EnlargeGrid(currNode->grid, enlargedNode->grid);
					}
				}
			}
			if (enlargedNode != nullptr)
			{
				parent->children.emplace_back(enlargedNode);
				auto node = enlargedNode->parent;
				while (node != nullptr)
				{
					node->grid = EnlargeGrid(node->grid, enlargedNode->grid);
					node = node->parent;
				}
			}
			while (!reservedDeletion.empty())
			{
				currNode->children.erase(reservedDeletion.front());
				reservedDeletion.pop();
			}
			if (noCollision)
			{
				currNode->children.emplace_back(newNode);
				newNode->parent = currNode;
				currNode->grid = EnlargeGrid(currNode->grid, newNode->grid);
				auto node = currNode->parent;
				while (node != nullptr)
				{
					node->grid = EnlargeGrid(node->grid, newNode->grid);
					node = node->parent;
				}
			}
		}
	}
	else
	{
		auto parent = currNode->parent;
		SPACE_PARTITIONING_NODE* upperNode = new SPACE_PARTITIONING_NODE;
		manager.emplace_back(upperNode);
		upperNode->children.emplace_back(newNode);
		upperNode->children.emplace_back(currNode);
		upperNode->parent = parent;
		upperNode->grid = EnlargeGrid(currNode->grid, newNode->grid);
		newNode->parent = upperNode;
		currNode->parent = upperNode;
		if (parent == nullptr) head = upperNode;
		else
		{
			for (auto node = parent->children.begin(); node != parent->children.end(); node++)
			{
				if (*node == currNode)
				{
					parent->children.erase(node);
					break;
				}
			}
			auto node = upperNode->parent;
			while (node != nullptr)
			{
				node->grid = EnlargeGrid(node->grid, upperNode->grid);
				node = node->parent;
			}
		}
	}
}

void SpatialPartitioning::DebugRender(SPACE_PARTITIONING_NODE* node, UEngine::Color color)
{
	if (node != nullptr)
	{
		DrawGrid(node->grid, color);
		auto childrenColor = UEngine::Color{ UEngine::Math::RndFloat(), UEngine::Math::RndFloat(), UEngine::Math::RndFloat(), 1 };
		for (auto childPair : node->children)
			DebugRender(childPair, childrenColor);
	}
}

void SpatialPartitioning::Traverse
(
	std::list<UEngine::GameObject*>& out, 
	SPACE_PARTITIONING_NODE* currNode, 
	UEngine::GameObject* gameObject
)
{
	auto grid = MakeGrid(gameObject->GetComponent<ScriptComponent>()->aabb);
	if (IsColliding(grid, currNode->grid))
	{
		for (auto obj : currNode->gameObjects)
			if (obj != gameObject) out.emplace_back(obj);
		for (auto child : currNode->children)
			Traverse(out, child, gameObject);
	}
}

void SpatialPartitioning::Release()
{
	for (auto item : manager)
		delete item;
	manager.clear();
	head = nullptr;
}

SpatialPartitioning::GRID SpatialPartitioning::MakeGrid(UEngine::Math::Physics2D::AABB aabb)
{
	GRID grid;
	grid.left = static_cast<int>(floorf(aabb.left));
	grid.right = static_cast<int>(ceil(aabb.right));
	grid.bottom = static_cast<int>(floorf(aabb.bottom));
	grid.top = static_cast<int>(ceil(aabb.top));
	return grid;
}

SpatialPartitioning::GRID SpatialPartitioning::EnlargeGrid(GRID grid1, GRID grid2)
{
	GRID newGrid;
	newGrid.left = min(grid1.left, grid2.left);
	newGrid.right = max(grid1.right, grid2.right);
	newGrid.bottom = min(grid1.bottom, grid2.bottom);
	newGrid.top = max(grid1.top, grid2.top);
	return newGrid;
}

UEngine::Math::Physics2D::AABB SpatialPartitioning::MakeAABB(UEngine::Math::Physics2D::AABB aabb)
{
	aabb.left = floorf(aabb.left);
	aabb.right = ceil(aabb.right);
	aabb.bottom = floorf(aabb.bottom);
	aabb.top = ceil(aabb.top);
	return aabb;
}
