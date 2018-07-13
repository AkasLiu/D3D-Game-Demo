#include "gameObject.h"

GameObject::GameObject(Transform tf, LPDIRECT3DDEVICE9 device) : 
	transform(tf),pDevice(device){ }

GameObject::~GameObject()
{
	//todo  Õ∑≈device ≤ƒ÷  Œ∆¿Ì
}

void GameObject::initWorldTransform()
{
	D3DXMATRIX matWorld, T, R, S;
	
}
