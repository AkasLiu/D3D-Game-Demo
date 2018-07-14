#pragma once
#include <d3dx9.h>

struct BoundingBox
{
	BoundingBox();
	bool isPointInside(D3DXVECTOR3& p);
	bool isCollided(BoundingBox collision);		//collision 是被碰撞的物体
	D3DXVECTOR3 _min;
	D3DXVECTOR3 _max;
};