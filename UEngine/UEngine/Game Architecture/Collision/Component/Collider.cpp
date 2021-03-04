#include "UEngine.h"
#include "Collider.h"

namespace UEngine
{
    void Collider::FixedUpdate()
    {
        isAABBColliding = false;
        worldAABB = MakeAABB(localAABB, GetTransform()->GetWorld());
        others.clear();
        GameState::Get()->GetSpatialPartition2D()->EnlargeHead(worldAABB);
    }

    void Collider::PhysicsUpdate()
	{
        GameState::Get()->GetSpatialPartition2D()->ConstructNode(this);
	}
}