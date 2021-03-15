#include "UEngine.h"
#include "Collider.h"

namespace UEngine
{
    namespace Physics2D
    {
        void Collider::FixedUpdate()
        {
            worldAABB = MakeAABB(localAABB, GetTransform()->GetRTP());
            others.clear();
            collisions.clear();
            GetGameObject()->GetScene()->partition2D->EnlargeHead(worldAABB);
        }

        void Collider::PhysicsUpdate()
        {
            GetGameObject()->GetScene()->partition2D->ConstructNode(this);
        }
    }
}