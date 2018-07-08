#include "transform.h"
#

Transform::Transform(D3DXVECTOR3 p, D3DXVECTOR3 r, D3DXVECTOR3 s) : 
	position(p),rotation(r),scale(s)
{
}

Transform::Transform(D3DXVECTOR3 p) :
	position(p)
{
}

Transform::~Transform()
{
}

void Transform::setPostion(D3DXVECTOR3 pos)
{
	position = pos;
}

D3DXVECTOR3 Transform::getPosition()
{
	return position;
}

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
//��rotation�����ĸ�������������0~360֮��
//
D3DXVECTOR3 Transform::clampAngle(D3DXVECTOR3 r)
{
	const D3DXVECTOR3 minVector = { 0.0f,0.0f,0.0f };
	const D3DXVECTOR3 maxVector = { 360.0f,360.0f,360.0f };
	//�Ҳ����ú���
	//clamp(r.x, minVector.x, maxVector.x);
	D3DXVECTOR3 a;
	return a;
}