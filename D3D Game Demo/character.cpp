#include "character.h"

Character::Character(Transform tf, LPDIRECT3DDEVICE9 pDevice) :
	GameObject(tf,pDevice)
{
}

Character::~Character()
{
}

void Character::init()
{
	boundingBox.ComputeBoundingBox(pMesh);
	boundingBox.initBoudingBox(pDevice);
}

void Character::draw()
{
	initWorldTransform();

	for (int i = 0; i < Mtrls.size(); ++i)
	{
		pDevice->SetMaterial(&Mtrls[i]);
		pDevice->SetTexture(0, Textures[i]);
		pMesh->DrawSubset(i);
	}

	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	boundingBox.getBoxMesh()->DrawSubset(0);
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	pDevice->SetTexture(0, 0);
}
