#include "skyBox.h"

//const DWORD SkyBoxVertex::FVF = D3DFVF_XYZ | D3DFVF_TEX1;

SkyBox::SkyBox(LPDIRECT3DDEVICE9 device)
{
	pd3dDevice = device;
	pVertexBuffer = nullptr;
	for (int i = 0; i < 5; ++i)
	{
		pTexture[i] = nullptr;
	}
	skyBoxLength = 0.0f;
}

//-------------------------------------------------------------------------------------------------
// Name:  SkyBoxClass::InitSkyBox( )
// Desc: ��պг�ʼ�����������㻺�����ĸ�ֵ
//-------------------------------------------------------------------------------------------------
BOOL SkyBox::InitSkyBox(float length)
{
	skyBoxLength = length;

	pd3dDevice->CreateVertexBuffer(20 * sizeof(SkyBoxVertex), 0, SkyBoxVertex::FVF, D3DPOOL_MANAGED, &pVertexBuffer, 0);

	SkyBoxVertex vertices[] =
	{
		//ǰ����ĸ�����
		{ -skyBoxLength / 2, 0.0f,    skyBoxLength / 2, 0.0f, 1.0f, },
		{ -skyBoxLength / 2, skyBoxLength / 2,   skyBoxLength / 2, 0.0f, 0.0f, },
		{ skyBoxLength / 2, 0.0f,    skyBoxLength / 2, 1.0f, 1.0f, },
		{ skyBoxLength / 2, skyBoxLength / 2,   skyBoxLength / 2, 1.0f, 0.0f, },

		//������ĸ�����
		{ skyBoxLength / 2, 0.0f,   -skyBoxLength / 2, 0.0f, 1.0f, },
		{ skyBoxLength / 2, skyBoxLength / 2,  -skyBoxLength / 2, 0.0f, 0.0f, },
		{ -skyBoxLength / 2, 0.0f,   -skyBoxLength / 2, 1.0f, 1.0f, },
		{ -skyBoxLength / 2, skyBoxLength / 2,  -skyBoxLength / 2, 1.0f, 0.0f, },

		//������ĸ�����
		{ -skyBoxLength / 2, 0.0f,   -skyBoxLength / 2, 0.0f, 1.0f, },
		{ -skyBoxLength / 2, skyBoxLength / 2,  -skyBoxLength / 2, 0.0f, 0.0f, },
		{ -skyBoxLength / 2, 0.0f,    skyBoxLength / 2, 1.0f, 1.0f, },
		{ -skyBoxLength / 2, skyBoxLength / 2,   skyBoxLength / 2, 1.0f, 0.0f, },

		//������ĸ�����
		{ skyBoxLength / 2, 0.0f,   skyBoxLength / 2, 0.0f, 1.0f, },
		{ skyBoxLength / 2, skyBoxLength / 2,  skyBoxLength / 2, 0.0f, 0.0f, },
		{ skyBoxLength / 2, 0.0f,  -skyBoxLength / 2, 1.0f, 1.0f, },
		{ skyBoxLength / 2, skyBoxLength / 2, -skyBoxLength / 2, 1.0f, 0.0f, },

		//������ĸ�����
		{ skyBoxLength / 2, skyBoxLength / 2, -skyBoxLength / 2, 1.0f, 0.0f, },
		{ skyBoxLength / 2, skyBoxLength / 2,  skyBoxLength / 2, 1.0f, 1.0f, },
		{ -skyBoxLength / 2, skyBoxLength / 2, -skyBoxLength / 2, 0.0f, 0.0f, },
		{ -skyBoxLength / 2, skyBoxLength / 2,  skyBoxLength / 2, 0.0f, 1.0f, },
	};

	//׼����䶥������
	void* pVertices;
	//2.����
	pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);
	//3.���ʡ��ѽṹ���е�����ֱ�ӿ������㻺������
	memcpy(pVertices, vertices, sizeof(vertices));
	//4.����
	pVertexBuffer->Unlock();

	return TRUE;
}

//-------------------------------------------------------------------------------------------------
// Name:  SkyBoxClass::LoadSkyTextureFromFile( )
// Desc: ��պ�������غ���
//-------------------------------------------------------------------------------------------------
BOOL SkyBox::LoadSkyTextureFromFile(wchar_t *pFrontTextureFile, wchar_t *pBackTextureFile, wchar_t *pLeftTextureFile, wchar_t *pRightTextureFile, wchar_t *pTopTextureFile)
{
	//���ļ�������������
	D3DXCreateTextureFromFile(pd3dDevice, pFrontTextureFile, &pTexture[0]);  //ǰ��
	D3DXCreateTextureFromFile(pd3dDevice, pBackTextureFile, &pTexture[1]);  //����
	D3DXCreateTextureFromFile(pd3dDevice, pLeftTextureFile, &pTexture[2]);  //����
	D3DXCreateTextureFromFile(pd3dDevice, pRightTextureFile, &pTexture[3]);  //����
	D3DXCreateTextureFromFile(pd3dDevice, pTopTextureFile, &pTexture[4]);  //����	
	return TRUE;
}

//--------------------------------------------------------------------------------------
// Name: SkyBoxClass::RenderSkyBox()
// Desc: ���Ƴ���պУ�����ͨ���ڶ�������ѡ���Ƿ���Ƴ��߿�
//--------------------------------------------------------------------------------------
void SkyBox::RenderSkyBox(D3DXMATRIX *pMatWorld, BOOL bRenderFrame)
{
	pd3dDevice->SetRenderState(D3DRS_LIGHTING, false);

	pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);  //��������ɫ��ϵĵ�һ����������ɫֵ�������
	pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);   //������ɫ��ϵĵ�һ��������ֵ��ȡ������ɫֵ
	pd3dDevice->SetTransform(D3DTS_WORLD, pMatWorld);  //�����������
	pd3dDevice->SetStreamSource(0, pVertexBuffer, 0, sizeof(SkyBoxVertex));    //�Ѱ����ļ�������Ϣ�Ķ��㻺�����Ⱦ��ˮ�������  
	pd3dDevice->SetFVF(SkyBoxVertex::FVF);  //����FVF�����ʽ

	//һ��forѭ������5������Ƴ���
	for (int i = 0; i < 5; i++)
	{
		pd3dDevice->SetTexture(0, pTexture[i]);
		pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
	}

	//���Ƿ���Ⱦ�߿�Ĵ������
	if (bRenderFrame)  //���Ҫ��Ⱦ���߿�Ļ�
	{
		pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME); //�����ģʽ��Ϊ�߿����

																	   //һ��forѭ������5������߿���Ƴ���
		for (int i = 0; i < 5; i++)
		{
			pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);	//���ƶ��� 
		}

		pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);	//�����ģʽ����ʵ�����
	}

	pd3dDevice->SetRenderState(D3DRS_LIGHTING, true);
}

SkyBox::~SkyBox()
{
	pVertexBuffer->Release();
	for (int i = 0; i < 5; i++)
	{
		pTexture[i]->Release();
	}
}