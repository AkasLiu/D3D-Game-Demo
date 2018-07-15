#include "boundingVolume.h"

BoundingBox::BoundingBox()
{
	// infinite small 
	_min.x = INFINITY;
	_min.y = INFINITY;
	_min.z = INFINITY;

	_max.x = -INFINITY;
	_max.y = -INFINITY;
	_max.z = -INFINITY;
}

BoundingBox::~BoundingBox()
{
}

bool BoundingBox::isPointInside(D3DXVECTOR3 & p)
{
	if (p.x >= _min.x && p.y >= _min.y && p.z >= _min.z &&
		p.x <= _max.x && p.y <= _max.y && p.z <= _max.z)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//
//碰撞检测
//
bool BoundingBox::isCollided(BoundingBox collision)
{
	if (_min.x < collision._max.x && _max.x > collision._min.x &&
		_min.y < collision._max.y && _max.y > collision._min.y &&
		_min.z < collision._max.z && _max.z > collision._min.z)
	{
		return true;
	}
	else
	{
		return false;
	}
}
//计算出该物体的外接体
bool BoundingBox::ComputeBoundingBox(ID3DXMesh * XFileMesh)
{
	HRESULT hr = 0;

	BYTE* v = 0;
	XFileMesh->LockVertexBuffer(0, (void**)&v);

	hr = D3DXComputeBoundingBox(
		(D3DXVECTOR3*)v,
		XFileMesh->GetNumVertices(),
		D3DXGetFVFVertexSize(XFileMesh->GetFVF()),
		&this->_min,
		&this->_max);

	XFileMesh->UnlockVertexBuffer();

	if (FAILED(hr))
		return false;

	return true;
}

void BoundingBox::initBoudingBox(LPDIRECT3DDEVICE9 pDevice)
{
	D3DXCreateBox(
		pDevice,
		this->_max.x - this->_min.x,
		this->_max.y - this->_min.y,
		this->_max.z - this->_min.z,
		&pBoxMesh,
		0);
}

LPD3DXMESH BoundingBox::getBoxMesh()
{
	return pBoxMesh;
}
