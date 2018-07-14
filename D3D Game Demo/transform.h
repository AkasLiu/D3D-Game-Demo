//////////////////////////////////////////////////////////////////////////
//--------------------------文件功能描述----------------------------------
//
//  transfrom类保存了position，rotation和scale。这个类将被gameObject类继承。
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
//	//好像并没什么卵用
//	bool isIdentityPosition(D3DXVECTOR3 vec);
//	bool isIdentityRotation(D3DXVECTOR3 vec);
//	bool isIdentityScale(D3DXVECTOR3 vec);
//};

