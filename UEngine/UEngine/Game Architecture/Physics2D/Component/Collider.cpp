#include "UEngine.h"
#include "Collider.h"

namespace UEngine
{
    namespace Physics2D
    {
        void Collider::Start()
        {
            others = new std::unordered_set<Collider*>();
            prevOthers = new std::unordered_set<Collider*>();
            collisions = new std::unordered_set<Collider*>();
            prevCollisions = new std::unordered_set<Collider*>();
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
                    if (others->find(prev) == others->end())
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
                    if (collisions->find(prev) == collisions->end())
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
            if (prevOthers->find(other) == prevOthers->end())
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

            others->emplace(other);
        }

        void Collider::OnCollision(Collider* other)
        {
            if (prevCollisions->find(other) == prevCollisions->end())
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

            collisions->emplace(other);
        }
    }
}