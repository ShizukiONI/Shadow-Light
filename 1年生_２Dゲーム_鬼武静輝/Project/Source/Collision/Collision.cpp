#include "DxLib.h"
#include "Collision.h"

bool CheckCollisionBox(float leftA, float topA, float rightA, float bottomA, float leftB, float topB, float rightB, float bottomB)
{
	if (rightA <= leftB ||
		leftA >= rightB ||
		bottomA <= topB ||
		topA >= bottomB)
	{
		return false;
	}

	//重なっている
	return true;
}
