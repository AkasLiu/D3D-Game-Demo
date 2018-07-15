#pragma once
#include "gameObject.h"

class Cube : public GameObject
{
public:
	Cube(D3DXVECTOR3 position,LPDIRECT3DDEVICE9 pDevice);
	Cube(D3DXVECTOR3 position, D3DXVECTOR3 size, LPDIRECT3DDEVICE9 pDevice);
	Cube(Transform tf, LPDIRECT3DDEVICE9 pDevice);
	~Cube();

public:
	void init();
	void draw();
	void setMaterial(D3DMATERIAL9 mtrl);

private:
	D3DXVECTOR3 size = { 1.0f,1.0f,1.0f };
};

