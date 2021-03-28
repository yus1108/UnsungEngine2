#include "UEngine.h"
#include "Collider.h"

namespace UEngine
{
    namespace Physics2D
    {
        bool Collider::VectorFind(std::vector<Collider*>* vColliders, Collider* collider)
        {
            for (size_t i = 0; i < vColliders->size(); i++)
            {
                if (collider == (*vColliders)[i])
                {
                    return true;
                }
            }
            return false;
        }

        void Collider::Start()
        {
            others = new std::vector<Collider*>();
            prevOthers = new std::vector<Collider*>();
            collisions = new std::vector<Collider*>();
            prevCollisions = new std::vector<Collider*>();
        }
        void Collider::OnDestroy()
        {
            delete others;
            delete prevOthers;
            delete collisions;
            delete prevCollisions;
        }
        void Collider::OnEnable()
        {
            worldAABB = MakeAABB(localAABB, GetTransform()->GetRTP());
        }

        void Collider::FixedUpdate()
        {
            if (!GetGameObject()->IsStatic)
                worldAABB = MakeAABB(localAABB, GetTransform()->GetRTP());

            auto temp = prevOthers;
            prevOthers = others;
            others = temp;
            others->clear();

            temp = prevCollisions;
            prevCollisions = collisions;
            collisions = temp;
            collisions->clear();

            GetGameObject()->GetScene()->partition2D->EnlargeHead(worldAABB);
        }

        void Collider::PhysicsUpdate()
        {
            GetGameObject()->GetScene()->partition2D->ConstructNode(this);
        }

        void Collider::Update()
        {
            if (prevOthers->size() > 0)
            {
                for (auto prev : *prevOthers)
                {
                    if (VectorFind(others, prev) == false)
                    {
                        auto components = GetGameObject()->GetComponents();
                        for (size_t i = 0; i < components->size(); i++)
                            (*components)[i]->OnTriggerExit(prev);
                    }
                }
                prevOthers->clear();
            }
            if (prevCollisions->size() > 0)
            {
                for (auto prev : *prevCollisions)
                {
                    if (VectorFind(collisions, prev) == false)
                    {
                        auto components = GetGameObject()->GetComponents();
                        for (size_t i = 0; i < components->size(); i++)
                            (*components)[i]->OnCollisionExit(prev);
                    }
                }
                prevCollisions->clear();
            }
        }

        void Collider::OnTrigger(Collider* other)
        {
            if (VectorFind(prevOthers, other) == false)
            {
                auto components = GetGameObject()->GetComponents();
                for (size_t i = 0; i < components->size(); i++)
                    (*components)[i]->OnTriggerEnter(other);
            }
            else
            {
                auto components = GetGameObject()->GetComponents();
                for (size_t i = 0; i < components->size(); i++)
                    (*components)[i]->OnTriggerStay(other);
            }
            others->emplace_back(other);
        }

        void Collider::OnCollision(Collider* other)
        {
            if (VectorFind(prevCollisions, other) == false)
            {
                auto components = GetGameObject()->GetComponents();
                for (size_t i = 0; i < components->size(); i++)
                    (*components)[i]->OnCollisionEnter(other);
            }
            else
            {
                auto components = GetGameObject()->GetComponents();
                for (size_t i = 0; i < components->size(); i++)
                    (*components)[i]->OnCollisionStay(other);
            }

            collisions->emplace_back(other);
        }
    }
}