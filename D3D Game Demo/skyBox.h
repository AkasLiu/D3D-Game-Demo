#pragma once
#include "d3dUtility.h"

//����һ����պе�FVF�����ʽ
struct SkyBoxVertex
{
	SkyBoxVertex() {}
	SkyBoxVertex(float x, float y, float z, float u, float v)
	{
		_x = x; _y = y; _z = z; _u = u; _v = v;
	}
	float _x, _y, _z;
	float _u, _v;

	static const DWORD FVF = D3DFVF_XYZ | D3DFVF_TEX1;
};

class SkyBox
{
public:
	SkyBox(LPDIRECT3DDEVICE9 device);
	~SkyBox();
	BOOL InitSkyBox(float length);
	BOOL LoadSkyTextureFromFile(wchar_t * pFrontTextureFile, wchar_t * pBackTextureFile, wchar_t * pLeftTextureFile, wchar_t * pRightTextureFile, wchar_t * pTopTextureFile);
	void RenderSkyBox(D3DXMATRIX * pMatWorld, BOOL bRenderFrame);

private:
	LPDIRECT3DDEVICE9			pd3dDevice;				//D3D�豸����
	LPDIRECT3DVERTEXBUFFER9		pVertexBuffer;			//���㻺�����
	LPDIRECT3DTEXTURE9			pTexture[5];			//5���������ӿ�
	float						skyBoxLength;			//��պб߳�
};