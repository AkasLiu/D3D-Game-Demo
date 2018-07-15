#include "gameObject.h"

GameObject::GameObject(Transform tf, LPDIRECT3DDEVICE9 device) : 
	transform(tf),pDevice(device){ }

GameObject::~GameObject()
{
	//todo 释放device 应该不用？
	d3d::Release<LPDIRECT3DTEXTURE9>(pTexture);
	d3d::Release<LPD3DXMESH>(pMesh);
	d3d::Release<LPDIRECT3DDEVICE9>(pDevice);
}

void GameObject::loadTexture(LPCWSTR filePath)
{
	HRESULT hr = D3DXCreateTextureFromFile(pDevice, filePath, &pTexture);
	if (FAILED(hr))
	{
		::MessageBox(0, L"gameObjectTexture - FAILED", 0, 0);
	}
}

bool GameObject::loadXFile(LPCWSTR filePath)
{
	LPD3DXBUFFER mtrlBuffer = 0;

	HRESULT hr = D3DXLoadMeshFromX(
		filePath,
		D3DXMESH_MANAGED,
		pDevice,
		&ppAdjacency,
		&mtrlBuffer,
		0,
		&numMaterials,
		&pMesh
	);

	if (FAILED(hr))
	{
		::MessageBox(0, L"XFileLoad - FAILED", 0, 0);
		return false;
	}

	if (mtrlBuffer != 0 && numMaterials != 0)
	{
		D3DXMATERIAL *materials = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();

		for (unsigned int i = 0; i < numMaterials; i++)
		{
			materials[i].MatD3D.Ambient = materials[i].MatD3D.Diffuse;
			Mtrls.push_back(materials[i].MatD3D);

			if (materials[i].pTextureFilename != 0)
			{
				LPDIRECT3DTEXTURE9 tex = 0;
				//LPCWSTR textureFilePath = materials[i].pTextureFilename;

				//路径问题暂时无法解决
				D3DXCreateTextureFromFileA(
					pDevice,
					materials[i].pTextureFilename,
					&tex);

				Textures.push_back(tex);
			}
			else
			{
				Textures.push_back(nullptr);
			}
		}
	}

	d3d::Release<LPD3DXBUFFER>(mtrlBuffer);

	return true;
}

void GameObject::setMaterial(D3DMATERIAL9 mtrl)
{
	material = mtrl;
}

void GameObject::init()
{
}

Transform* GameObject::getTransform()
{
	return &transform;
}

void GameObject::setTransform(Transform * tf)
{
	transform = *tf;
}

BoundingBox* GameObject::getBoundingBox()
{
	return &boundingBox;
}

//void GameObject::draw()
//{
//}

//D3DXVECTOR3 GameObject::getPosition()
//{
//	return transform.position;
//}

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
