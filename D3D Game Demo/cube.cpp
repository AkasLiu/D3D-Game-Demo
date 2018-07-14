#include "cube.h"

Cube::Cube(D3DXVECTOR3 position, LPDIRECT3DDEVICE9 pDevice) :
	GameObject(Transform(position), pDevice)
{

}

Cube::Cube(Transform tf, LPDIRECT3DDEVICE9 pDevice) :
	GameObject(tf, pDevice)
{
}

Cube::~Cube()
{
}

void Cube::init()
{
	D3DXCreateBox(pDevice, transform.scale.x, transform.scale.y, transform.scale.z, &pMesh, NULL);
}

void Cube::draw()
{
	initWorldTransform();
	pMesh->DrawSubset(0);
}
