#include "stdafx.h"
#include "Puzzle.h"

void Puzzle::Start()
{

}

void Puzzle::Update()
{
	if (!start)
	{
		for (size_t i = 0; i < 100; i++)
		{
			Direction dir = static_cast<Direction>(Math::RndInt(0, 4));
			Swap(dir);
		}
		pieces[currPos.x][currPos.y]->GetComponent<RenderComponent>()->SetEnable(false);
		start = true;
	}
	if (WinInput::Get()->GetKeyDown('1'))
	{
		start = false;
		return;
	}
	if (WinInput::Get()->GetKeyDown('2'))
	{
		pieces[currPos.x][currPos.y]->GetComponent<RenderComponent>()->SetEnable(true);
	}
	if (WinInput::Get()->GetKeyDown('3'))
	{
		pieces[currPos.x][currPos.y]->GetComponent<RenderComponent>()->SetEnable(false);
	}
	if (WinInput::Get()->GetKeyDown(VK_RIGHT))
	{
		Swap(Direction::right);
	}
	if (WinInput::Get()->GetKeyDown(VK_LEFT))
	{
		Swap(Direction::left);
	}
	if (WinInput::Get()->GetKeyDown(VK_UP))
	{
		Swap(Direction::top);
	}
	if (WinInput::Get()->GetKeyDown(VK_DOWN))
	{
		Swap(Direction::bottom);
	}
	if (WinInput::Get()->GetKey(VK_ESCAPE))
	{
		if (!stack.empty())
		{
			Direction dir = Direction::left;
			switch (stack.top())
			{
			case Puzzle::Direction::left:
				dir = Direction::right;
				break;
			case Puzzle::Direction::top:
				dir = Direction::bottom;
				break;
			case Puzzle::Direction::right:
				dir = Direction::left;
				break;
			case Puzzle::Direction::bottom:
				dir = Direction::top;
				break;
			}
			stack.pop();
			Swap(dir);
			stack.pop();
		}
	}
}

bool Puzzle::Swap(Direction dir)
{
	auto newPos = currPos;
	switch (dir)
	{
	case Puzzle::Direction::left:
		if (currPos.x == 0)	return false;
		newPos.x--;
		break;
	case Puzzle::Direction::top:
		if (currPos.y == 2)	return false;
		newPos.y++;
		break;
	case Puzzle::Direction::right:
		if (currPos.x == 2)	return false;
		newPos.x++;
		break;
	case Puzzle::Direction::bottom:
		if (currPos.y == 0)	return false;
		newPos.y--;
		break;
	default:
		return false;
	}

	auto tempPos = pieces[currPos.x][currPos.y]->GetTransform()->localPosition;
	pieces[currPos.x][currPos.y]->GetTransform()->localPosition = pieces[newPos.x][newPos.y]->GetTransform()->localPosition;
	pieces[newPos.x][newPos.y]->GetTransform()->localPosition = tempPos;

	auto tempObj = pieces[currPos.x][currPos.y];
	pieces[currPos.x][currPos.y] = pieces[newPos.x][newPos.y];
	pieces[newPos.x][newPos.y] = tempObj;

	currPos = newPos;
	stack.push(dir);
	return true;
}
