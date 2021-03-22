#pragma once

UENGINE_CLASS(FollowCamera)
{
private:
	Transform* playerTransform;
	Camera* world;
	Camera* follow;

	void Start() override;
	void Update() override;
};

