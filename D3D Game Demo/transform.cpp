#include "transform.h"
#

Transform::Transform(D3DXVECTOR3 p, D3DXVECTOR3 r, D3DXVECTOR3 s) : 
	position(p),rotation(r),scale(s){ }

Transform::Transform(D3DXVECTOR3 p, D3DXVECTOR3 r) :
	position(p), rotation(r){ }

Transform::Transform(D3DXVECTOR3 p) :
	position(p){ }

Transform::Transform()
{ }

Transform::~Transform()
{ }

void Transform::setPostion(D3DXVECTOR3* pos)
{
	position = *pos;
}

void Transform::getPosition(D3DXVECTOR3 *pos)
{
	*pos = position;
}

//todo
void Transform::setRotation(D3DXVECTOR3 r)
{

}

D3DXVECTOR3 Transform::getRotation()
{
	return rotation;
}

void Transform::setScale(D3DXVECTOR3 s)
{
	scale = s;
}

D3DXVECTOR3 Transform::getScale()
{
	return scale;
}

//todo
//将rotation向量的各个分量限制在0~360之间
//
D3DXVECTOR3 Transform::clampAngle(D3DXVECTOR3 r)
{
	const D3DXVECTOR3 minVector = { 0.0f,0.0f,0.0f };
	const D3DXVECTOR3 maxVector = { 360.0f,360.0f,360.0f };
	//找不到该函数
	//clamp(r.x, minVector.x, maxVector.x);
	D3DXVECTOR3 a;
	return a;
}

//bool Transform::isIdentityPosition(D3DXVECTOR3 vec)
//{
//	D3DXVECTOR3 normal;
//	if (normal == vec)
//	{
//		return true;
//	}
//	return false;
//}


