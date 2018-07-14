//////////////////////////////////////////////////////////////////////////
//--------------------------�ļ���������----------------------------------
//
//  transfrom�ౣ����position��rotation��scale������ཫ��gameObject��̳С�
//
//////////////////////////////////////////////////////////////////////////
#pragma once
#include "d3dUtility.h"

struct Transform 
{
	Transform(D3DXVECTOR3 p, D3DXVECTOR3 r, D3DXVECTOR3 s);
	Transform(D3DXVECTOR3 p, D3DXVECTOR3 r);
	Transform(D3DXVECTOR3 p);
	Transform();
	~Transform();

	void setPostion(D3DXVECTOR3* pos);
	void getPosition(D3DXVECTOR3* pos);
	void setRotation(D3DXVECTOR3 r);
	D3DXVECTOR3 getRotation();
	void setScale(D3DXVECTOR3 s);
	D3DXVECTOR3 getScale();

	D3DXVECTOR3 position = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 rotation = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 scale = { 10.0f,10.0f,10.0f };

private:
	D3DXVECTOR3 clampAngle(D3DXVECTOR3 r);

};

//class Transform
//{
//public:
//	
//
//public:
//	
//
//public:
//	
//
//private:
//	
//	//����ûʲô����
//	bool isIdentityPosition(D3DXVECTOR3 vec);
//	bool isIdentityRotation(D3DXVECTOR3 vec);
//	bool isIdentityScale(D3DXVECTOR3 vec);
//};

