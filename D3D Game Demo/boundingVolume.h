#pragma once
#include <d3dx9.h>

class BoundingBox
{
public:
	BoundingBox();
	~BoundingBox();

	bool isPointInside(D3DXVECTOR3& p);
	bool isCollided(BoundingBox* collision);		//collision �Ǳ���ײ������	
	bool ComputeBoundingBox(ID3DXMesh * XFileMesh, const D3DXVECTOR3 * p = nullptr);
	void initBoudingBox(LPDIRECT3DDEVICE9 pDevice);

	LPD3DXMESH getBoxMesh();
	void boudingBoxMove(D3DXVECTOR3 vec);
	D3DXVECTOR3* getBoudingBoxMin();
	D3DXVECTOR3* getBoudingBoxMax();

private:
	D3DXVECTOR3 _min;
	D3DXVECTOR3 _max;
	D3DXVECTOR3 _pos;			//������λ�ã�һ����gameObjectλ����ͬ
	LPD3DXMESH pBoxMesh = 0;
	bool isShow = false;
};
