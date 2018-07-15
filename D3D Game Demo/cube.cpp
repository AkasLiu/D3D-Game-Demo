#include "cube.h"

Cube::Cube(D3DXVECTOR3 position, LPDIRECT3DDEVICE9 pDevice) :
	GameObject(Transform(position), pDevice)
{

}

Cube::Cube(D3DXVECTOR3 position, D3DXVECTOR3 size, LPDIRECT3DDEVICE9 pDevice) :
	size(size),GameObject(Transform(position),pDevice)
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
	D3DXCreateBox(pDevice, size.x, size.y, size.z, &pMesh, NULL);
}

void Cube::draw()
{
	pDevice->SetMaterial(&material);

	initWorldTransform();
	pMesh->DrawSubset(0);

	pDevice->SetMaterial(&d3d::WHITE_MTRL);
}

//void Cube::setMaterial(D3DMATERIAL9 mtrl)
//{
//	material = mtrl;
//}


