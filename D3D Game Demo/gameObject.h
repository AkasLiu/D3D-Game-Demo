#pragma once
#include "transform.h"

class GameObject
{
public:
	GameObject(Transform tf,LPDIRECT3DDEVICE9 device);
	~GameObject();

public:
	virtual void loadTexture(LPCWSTR filePath)=0;
	virtual void loadXFile(LPCWSTR filePath) = 0;
	virtual void setMaterial(D3DMATERIAL9 mtrl) = 0;
	virtual void init() = 0;
	virtual void draw() = 0;
	
protected:
	Transform transform;
	LPDIRECT3DDEVICE9 pDevice = 0;
	LPDIRECT3DTEXTURE9 pTexture = 0;
	D3DMATERIAL9 material;
	LPD3DXMESH pMesh = 0;


	//todo
	void initWorldTransform();
};


