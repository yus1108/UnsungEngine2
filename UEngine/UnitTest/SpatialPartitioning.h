#pragma once

class SpatialPartitioning
{
public:
	struct GRID
	{
		int left{ 0 };
		int top{ 0 };
		int right{ 0 };
		int bottom{ 0 };

		GRID() = default;
		GRID(const GRID& grid) : left(grid.left), top(grid.top), right(grid.right), bottom(grid.bottom) {}
	};
private:
	struct SPACE_PARTITIONING_NODE
	{
		GRID grid;
		SPACE_PARTITIONING_NODE* parent{ nullptr };
		std::list<SPACE_PARTITIONING_NODE*> children;
		std::list<UEngine::GameObject*> gameObjects;
	};


public:
	SPACE_PARTITIONING_NODE* head;
	std::list<SPACE_PARTITIONING_NODE*> manager;

	SpatialPartitioning();
	~SpatialPartitioning();

	void DrawGrid(GRID grid, UEngine::Color color);
	void DrawGrid(int x, int y);

	int CompareGrid(GRID grid1, GRID grid2);
	bool IsColliding(GRID grid1, GRID grid2);

	void ConstructNode(UEngine::Math::Physics2D::AABB aabb, UEngine::GameObject* gameObject);
	void AddNode(SPACE_PARTITIONING_NODE* currNode, SPACE_PARTITIONING_NODE* newNode);
	void DebugRender(SPACE_PARTITIONING_NODE* node, UEngine::Color color);
	void Traverse(std::list<UEngine::GameObject*>& out, SPACE_PARTITIONING_NODE* currNode, UEngine::GameObject* gameObject);

	void Release();

	GRID MakeGrid(UEngine::Math::Physics2D::AABB aabb);
	GRID EnlargeGrid(GRID grid1, GRID grid2);
	UEngine::Math::Physics2D::AABB MakeAABB(UEngine::Math::Physics2D::AABB aabb);
};

