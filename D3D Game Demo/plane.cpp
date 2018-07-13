#include "plane.h"

inline Plane::Plane(D3DXVECTOR3 position, LPDIRECT3DDEVICE9 pDevice) :
	GameObject(position, pDevice)
{
}

Plane::~Plane()
{
}