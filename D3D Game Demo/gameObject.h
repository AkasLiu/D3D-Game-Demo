#pragma once

#include "transform.h"

class GameObject
{
public:
	GameObject(Transform tf,LPDIRECT3DDEVICE9 device);
	GameObject();
	~GameObject();

public:
	void loadTexture(LPCWSTR filePath);
	//真的写这里吗
	void loadXFile(LPCWSTR filePath);

	void setMaterial(D3DMATERIAL9 mtrl);
	virtual void init();
	virtual void draw();

protected:
	Transform transform;
	LPDIRECT3DDEVICE9 pDevice = 0;
	LPDIRECT3DTEXTURE9 pTexture = 0;
	D3DMATERIAL9 material;
};

