#pragma once
#include "transform.h"
#include "vertex.h"

class GameObject
{
public:
	GameObject(Transform tf,LPDIRECT3DDEVICE9 device);
	~GameObject();

public:
	void loadTexture(LPCWSTR filePath);
	virtual void loadXFile(LPCWSTR filePath);
	virtual void setMaterial(D3DMATERIAL9 mtrl);
	virtual void init();
	virtual void draw();

	D3DXVECTOR3 getPosition();
	
protected:
	Transform transform;
	LPDIRECT3DDEVICE9 pDevice = 0;
	LPDIRECT3DTEXTURE9 pTexture = 0;
	D3DMATERIAL9 material;
	LPD3DXMESH pMesh = 0;


	//todo
	void initWorldTransform();
};


