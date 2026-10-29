#pragma once

//四角形同士の当たり判定
bool CheckCollisionBox(
	float leftA, float topA,
	float rightA, float bottomA,
	float leftB, float topB,
	float rightB, float bottomB
);