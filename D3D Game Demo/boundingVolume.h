#pragma once
#include <d3dx9.h>

class BoundingBox
{
public:
	BoundingBox();
	~BoundingBox();

	bool isPointInside(D3DXVECTOR3& p);
	bool isCollided(BoundingBox collision);		//collision 是被碰撞的物体
	bool ComputeBoundingBox(ID3DXMesh* XFileMesh);		
	void initBoudingBox(LPDIRECT3DDEVICE9 pDevice);

	LPD3DXMESH getBoxMesh();

private:
	D3DXVECTOR3 _min;
	D3DXVECTOR3 _max;
	LPD3DXMESH pBoxMesh = 0;
	bool isShow = false;
};
