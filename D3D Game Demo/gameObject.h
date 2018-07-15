#pragma once
#include "transform.h"
#include "vertex.h"
#include "boundingVolume.h"
#include<vector>

class GameObject
{
public:
	GameObject(Transform tf,LPDIRECT3DDEVICE9 device);
	~GameObject();

public:
	void loadTexture(LPCWSTR filePath);
	virtual bool loadXFile(LPCWSTR filePath);
	virtual void setMaterial(D3DMATERIAL9 mtrl);
	virtual void init();
	virtual void draw() = 0;

	//D3DXVECTOR3 getPosition();
	Transform* getTransform();
	void setTransform(Transform *tf);
	BoundingBox* getBoundingBox();
	
protected:
	Transform transform;
	LPDIRECT3DDEVICE9 pDevice = 0;
	LPDIRECT3DTEXTURE9 pTexture = 0;
	D3DMATERIAL9 material = d3d::WHITE_MTRL;
	LPD3DXMESH pMesh = 0;
	LPD3DXBUFFER ppAdjacency;
	std::vector<D3DMATERIAL9> Mtrls;
	std::vector<LPDIRECT3DTEXTURE9> Textures;
	DWORD numMaterials;
	BoundingBox boundingBox;

	void initWorldTransform();
};


