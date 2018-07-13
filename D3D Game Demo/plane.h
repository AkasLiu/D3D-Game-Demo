#pragma once
#include "gameObject.h"

class Plane : public GameObject
{
public:
	Plane(D3DXVECTOR3 position,LPDIRECT3DDEVICE9 pDevice);
	~Plane();

private:

};
