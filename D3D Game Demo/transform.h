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

	/*void setPostion(D3DXVECTOR3* pos);
	D3DXVECTOR3 getPosition();
	void setRotation(D3DXVECTOR3 r);
	D3DXVECTOR3 getRotation();
	void setScale(D3DXVECTOR3 s);
	D3DXVECTOR3 getScale();*/

	D3DXVECTOR3 position = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 rotation = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 scale = { 1.0f,1.0f,1.0f };

private:
	D3DXVECTOR3 clampAngle(D3DXVECTOR3 r);

};
