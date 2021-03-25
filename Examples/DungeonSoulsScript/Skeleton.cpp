#include "pch.h"
#include "Skeleton.h"

void Skeleton::Start()
{
	Enemy::Start();

	skeleton_animation_map[SKELETON_ANIMATION_STATE_IDLE] = Animation(true, 0.2f, { 0, 0 }, { 10, 0 }, { 32.0f, 32.0f }, { 320.0f, 320.0f }, { 0, 0 });
	skeleton_animation_map[SKELETON_ANIMATION_STATE_MOVE] = Animation(true, 0.1f, { 0, 1 }, { 5, 1 }, { 32.0f, 32.0f }, { 320.0f, 320.0f }, { 0, 0 });
	skeleton_animation_map[SKELETON_ANIMATION_STATE_ATTACK] = Animation(false, 0.1f, { 0, 2 }, { 10, 2 }, { 32.0f, 32.0f }, { 320.0f, 320.0f }, { 0, 0 });
	skeleton_animation_map[SKELETON_ANIMATION_STATE_DAMAGED] = Animation(false, 0.1f, { 0, 3 }, { 5, 3 }, { 32.0f, 32.0f }, { 320.0f, 320.0f }, { 0, 0 });
	skeleton_animation_map[SKELETON_ANIMATION_STATE_DEAD] = Animation(false, 0.1f, { 0, 4 }, { 10, 4 }, { 32.0f, 32.0f }, { 320.0f, 320.0f }, { 0, 0 });

	animation = skeleton_animation_map[PLAYER_ANIMATION_STATE_IDLE];
	animation.Play();
}

void Skeleton::Update()
{
	if (player == nullptr) return;
	if (player->EditorMode) return;

	Enemy::Update();

	if (health->Dead)
	{
		animation.Change(skeleton_animation_map[SKELETON_ANIMATION_STATE_DEAD]);
		return;
	}

	if (attackCooldown > 0)
	{
		animation.Change(skeleton_animation_map[SKELETON_ANIMATION_STATE_ATTACK]);
	}
	else
	{
		if (movable)
		{
			if (velocity.x == 0)
				animation.Change(skeleton_animation_map[PLAYER_ANIMATION_STATE_IDLE]);
			else
				animation.Change(skeleton_animation_map[PLAYER_ANIMATION_STATE_MOVE]);
		}
	}
}

void Skeleton::LateUpdate()
{
	if (player == nullptr) return;
	if (player->EditorMode) return;

	Enemy::LateUpdate();
	material->uv = animation.Update();
}