#include "plane.h"

inline Plane::Plane(D3DXVECTOR3 position, LPDIRECT3DDEVICE9 pDevice) :
	GameObject(Transform(position), pDevice)
{
}

Plane::Plane(float l, float w, D3DXVECTOR3 position, LPDIRECT3DDEVICE9 pDevice) :
	length(l),width(w), GameObject(Transform(position), pDevice)
{
}

Plane::~Plane()
{
	d3d::Release<IDirect3DVertexBuffer9*>(vb);
}

void Plane::init()
{
	//创建顶点缓存
	pDevice->CreateVertexBuffer(
		6 * sizeof(Vertex),
		0,
		Vertex::FVF,
		D3DPOOL_MANAGED,
		&vb,
		0);
	vb->Lock(0, 0, (void**)&vertices, 0);

	//建立两个三角形，并为它填充光照和纹理坐标
	vertices[0] = Vertex(-length / 2, 0.0f, -width / 2, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	vertices[1] = Vertex(-length / 2, 0.0f, width / 2, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	vertices[2] = Vertex(length / 2, 0.0f, width / 2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);

	vertices[3] = Vertex(-length / 2, 0.0f, -width / 2, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	vertices[4] = Vertex(length / 2, 0.0f, width / 2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	vertices[5] = Vertex(length / 2, 0.0f, -width / 2, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);

	vb->Unlock();
}

void Plane::draw()
{
	pDevice->SetTexture(0, pTexture);
	pDevice->SetRenderState(D3DRS_LIGHTING, false);

	//线性纹理过滤
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	//多级渐进纹理
	pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	/*重复寻址模式
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	*/

	initWorldTransform();

	pDevice->SetStreamSource(0, vb, 0, sizeof(Vertex));
	pDevice->SetFVF(Vertex::FVF);
	pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

	pDevice->SetRenderState(D3DRS_LIGHTING, true);
	pDevice->SetTexture(0, 0);
}

//void Plane::loadTexture(LPCWSTR filePath)
//{
//	HRESULT hr = D3DXCreateTextureFromFile(pDevice, filePath, &pTexture);
//	if (FAILED(hr))
//	{
//		::MessageBox(0, L"gameObjectTexture - FAILED", 0, 0);
//	}
//}
