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
//Åö×²¼ì²â
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
