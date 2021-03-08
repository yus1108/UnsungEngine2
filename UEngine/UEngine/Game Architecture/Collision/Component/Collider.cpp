#include "UEngine.h"
#include "Collider.h"

namespace UEngine
{
    void Collider::FixedUpdate()
    {
        worldAABB = MakeAABB(localAABB, GetTransform()->GetRTP());
        others.clear();
        collisions.clear();
        GameState::Get()->GetSpatialPartition2D()->EnlargeHead(worldAABB);
    }

    void Collider::PhysicsUpdate()
	{
        GameState::Get()->GetSpatialPartition2D()->ConstructNode(this);
        
	}
}