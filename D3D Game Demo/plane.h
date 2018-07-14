#pragma once
#include "gameObject.h"

class Plane : public GameObject
{
public:
	Plane(D3DXVECTOR3 position,LPDIRECT3DDEVICE9 pDevice);
	Plane(float l, float w,D3DXVECTOR3 position, LPDIRECT3DDEVICE9 pDevice);
	~Plane();
	void init();
	void draw();
	void setMaterial();

private:
	float length = 1.0f;
	float width = 1.0f;
	Vertex* vertices;
	IDirect3DVertexBuffer9* vb;
};
