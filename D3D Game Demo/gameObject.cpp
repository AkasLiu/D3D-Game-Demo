#include "gameObject.h"

GameObject::GameObject(Transform tf, LPDIRECT3DDEVICE9 device) : 
	transform(tf),pDevice(device){ }

GameObject::~GameObject()
{
	//todo �ͷ�device ���� ����
}

void GameObject::initWorldTransform()
{
	D3DXMATRIX matWorld, T, R, S;
	
}
