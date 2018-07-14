#pragma once
#include <d3dx9.h>

struct BoundingBox
{
	BoundingBox();
	bool isPointInside(D3DXVECTOR3& p);
	bool isCollided(BoundingBox collision);		//collision �Ǳ���ײ������
	D3DXVECTOR3 _min;
	D3DXVECTOR3 _max;
};