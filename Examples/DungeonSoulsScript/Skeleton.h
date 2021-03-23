#pragma once

enum SKELETON_ANIMATION_STATE
{
	SKELETON_ANIMATION_STATE_IDLE,
	SKELETON_ANIMATION_STATE_MOVE,
	SKELETON_ANIMATION_STATE_ATTACK,
	SKELETON_ANIMATION_STATE_DAMAGED,
	SKELETON_ANIMATION_STATE_DEAD,
	SKELETON_ANIMATION_STATE_COUNT
};

UENGINE_CLASS_INHERIT(Skeleton, Enemy)
{
	Animation skeleton_animation_map[SKELETON_ANIMATION_STATE_COUNT];
	Animation animation = skeleton_animation_map[SKELETON_ANIMATION_STATE_IDLE];

	virtual void Start() override;
	virtual void Update() override;
	virtual void LateUpdate() override;

};

