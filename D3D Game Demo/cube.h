#pragma once
#include "gameObject.h"

class Cube : public GameObject
{
public:
	Cube(D3DXVECTOR3 position,LPDIRECT3DDEVICE9 pDevice);
	//Cube(D3DXVECTOR3 position, D3DXVECTOR3 scale, LPDIRECT3DDEVICE9 pDevice);
	Cube(Transform tf, LPDIRECT3DDEVICE9 pDevice);
	~Cube();

public:
	void init();
	void draw();

private:
	float length;
	float width;
	float height;
};

