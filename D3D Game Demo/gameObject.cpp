#include "gameObject.h"

GameObject::GameObject(Transform tf, LPDIRECT3DDEVICE9 device) : 
	transform(tf),pDevice(device){ }

GameObject::~GameObject()
{
	//todo 释放device 应该不用？
	d3d::Release<LPDIRECT3DTEXTURE9>(pTexture);
	d3d::Release<LPD3DXMESH>(pMesh);
}

void GameObject::loadTexture(LPCWSTR filePath)
{
	HRESULT hr = D3DXCreateTextureFromFile(pDevice, filePath, &pTexture);
	if (FAILED(hr))
	{
		::MessageBox(0, L"gameObjectTexture - FAILED", 0, 0);
	}
}

void GameObject::loadXFile(LPCWSTR filePath)
{
}

void GameObject::setMaterial(D3DMATERIAL9 mtrl)
{
	pDevice->SetMaterial(&d3d::WHITE_MTRL);
}

void GameObject::init()
{
}

void GameObject::draw()
{
}

D3DXVECTOR3 GameObject::getPosition()
{
	return transform.position;
}

//
//设置世界变换
//
void GameObject::initWorldTransform()
{
	D3DXMATRIX matWorld, T, R, S;
	D3DXMatrixScaling(&S, transform.scale.x, transform.scale.y, transform.scale.z);
	D3DXMatrixTranslation(&T, transform.position.x, transform.position.y, transform.position.z);
	matWorld = T*S;
	pDevice->SetTransform(D3DTS_WORLD, &matWorld);
}
