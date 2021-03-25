#include "pch.h"
#include "FollowCamera.h"

void FollowCamera::Start()
{
	playerTransform = FindObjectWithName("player")->GetTransform();
	world = FindObjectWithName("worldCamera")->GetComponent<Camera>();
	follow = FindObjectWithName("followCamera")->GetComponent<Camera>();
}

void FollowCamera::Update()
{
	if (Input::GetKeyDown('1'))
	{
		world->SetMainCamera();
	}
	if (Input::GetKeyDown('2'))
	{
		follow->SetMainCamera();
	}
	if (follow->GetMainCamera() == follow)
	{
		follow->GetTransform()->localPosition = playerTransform->localPosition.value;
		follow->GetTransform()->localPosition.value.y += 64.0f;
		if (follow->GetTransform()->localPosition.value.y < -176.0f)
			follow->GetTransform()->localPosition.value.y = -176.0f;
		else if (follow->GetTransform()->localPosition.value.y > 170.0f)
			follow->GetTransform()->localPosition.value.y = 170.0f;
		if (follow->GetTransform()->localPosition.value.x > 320.0f)
			follow->GetTransform()->localPosition.value.x = 320.0f;
		else if (follow->GetTransform()->localPosition.value.x < -320.0f)
			follow->GetTransform()->localPosition.value.x = -320.0f;
	}
}
