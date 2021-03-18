#include "pch.h"
#include "Boost.h"

void Boost::Start()
{
	material = GetComponent<Material>();
}

void Boost::Update()
{
	auto flight = FindObjectWithName("flight");
	if (flight == nullptr) return;
	GetTransform()->localPosition.value = flight->GetTransform()->localPosition.value;
	GetTransform()->localPosition.value.y -= 50;
	if (timer >= delay)
	{
		index++;
		material->uv = UV
		{
			(frameSize * index) / textureSize,
			0.25f,
			(frameSize * (index + 1)) / textureSize,
			1
		};
		timer = 0;
		if (index == 10)
		{
			index = 0;
		}
	}
	timer += Utility::UTime::Get()->DeltaTimeF();
}
