#include "framework.h"
#include "DebugRenderScript.h"

void DebugRenderScript::OnPreRender()
{
	auto sp = GameState::Get()->GetSpatialPartition2D();
	auto collider = GetComponent<Collider>();
	sp->DebugRender(sp->head, collider, Color{ 1, 0, 0, 1 }, Color{ 0, 0, 1, 1 });
}
