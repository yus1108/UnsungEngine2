#pragma once
#include "UEngine.h"

namespace UEngine
{
	namespace Physics2D
	{
		inline std::string Type_Circle = typeid(UEngine::Physics2D::CircleCollider*).raw_name();
		inline std::string Type_Rectangle = typeid(UEngine::Physics2D::RectCollider*).raw_name();

	}
}