#include "camera.h"

Camera::Camera()
{
	_cameraType = AIRCRAFT;

	_cameraPosition = D3DXVECTOR3(0.0f, 0.0f, -50.0f);
	_targetPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	_right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	_up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	_look = D3DXVECTOR3(0.0f, 0.0f, 1.0f);	
}

Camera::Camera(CameraType cameraType)
{
	_cameraType = cameraType;

	_cameraPosition = D3DXVECTOR3(0.0f, 13.0f, -30.0f);
	_targetPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	_right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	_up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	_look = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
}

Camera::~Camera()
{

}

void Camera::getCameraPosition(D3DXVECTOR3* pos)
{
	*pos = _cameraPosition;
}

void Camera::setCameraPosition(D3DXVECTOR3* pos)
{
	_cameraPosition = *pos;
}

void Camera::getRight(D3DXVECTOR3* right)
{
	*right = _right;
}

void Camera::getUp(D3DXVECTOR3* up)
{
	*up = _up;
}

void Camera::getLook(D3DXVECTOR3* look)
{
	*look = _look;
}

void Camera::SetTargetPosition(D3DXVECTOR3 * pLookat)
{
	//先看看pLookat是否为默认值NULL
	if (pLookat != NULL)  _targetPosition = (*pLookat);
	else _targetPosition = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	_look = _targetPosition - _cameraPosition;//观察点位置减摄像机位置，得到观察方向向量
	D3DXVec3Normalize(&_look, &_look);//规范化m_vLookVector向量

	//正交并规范化m_vUpVector和m_vRightVector
	D3DXVec3Cross(&_up, &_look, &_right);
	D3DXVec3Normalize(&_up, &_up);
	D3DXVec3Cross(&_right, &_up, &_look);
	D3DXVec3Normalize(&_right, &_right);
}

void Camera::ThirdPersonRotationY(float angle)
{
	D3DXMATRIX matRotationY;
	D3DXVECTOR3 lookVecTemp;
	D3DXVECTOR3 m_matup = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixRotationAxis(&matRotationY, &m_matup, angle);//求得绕标准上向量旋转angle的矩阵，保持法线向上
	D3DXVec3TransformCoord(&lookVecTemp, &(_cameraPosition - _targetPosition), &matRotationY);

	D3DXVec3TransformCoord(&_right, &_right, &matRotationY); 
	D3DXVec3TransformCoord(&_look, &_look, &matRotationY);

	_cameraPosition = _targetPosition + lookVecTemp;
}

void Camera::ThirdPersonRotationX(float angle)
{
	D3DXMATRIX matRotationX;
	D3DXVECTOR3 vNewEye, vTemp, m_vNewXAxis;

	//D3DXVec3Normalize(&m_vNewZAxis,&(m_vLookat-m_vEye));
	//叉乘求得right向量

	D3DXVec3Cross(&m_vNewXAxis, &(_targetPosition - _cameraPosition), &_up);

	D3DXMatrixRotationAxis(&matRotationX, &m_vNewXAxis, angle);//求得绕right旋转angle角度的矩阵
	D3DXVec3TransformCoord(&vNewEye, &(_cameraPosition - _targetPosition), &matRotationX);

	D3DXVec3TransformCoord(&_look, &_look, &matRotationX);
	D3DXVec3TransformCoord(&_up, &_up, &matRotationX);
	/*vNewEye += _targetPosition;

	D3DXVec3Normalize(&vTemp, &(_targetPosition - vNewEye));
	float fAngle = D3DXVec3Dot(&vTemp, &_up);
	if (fAngle < (-0.95f) || fAngle > 0.95f)
	{
	return;
	}

	_cameraPosition = vNewEye;*/

	_cameraPosition = _targetPosition + vNewEye;
}

void Camera::walk(float units, Character* player)
{
	// move only on xz plane for land object
	if (_cameraType == LANDOBJECT)
	{
		_cameraPosition += D3DXVECTOR3(_look.x, 0.0f, _look.z) * units;
		if (player != nullptr)
		{
			static D3DXVECTOR3 playerPosition;
			playerPosition = player->getTransform()->position;
			playerPosition += D3DXVECTOR3(_look.x, 0.0f, _look.z) * units*10;
			player->getTransform()->position = playerPosition;
		}
	}

	if (_cameraType == AIRCRAFT)
		_cameraPosition += _look * units;
}

void Camera::strafe(float units, Character* player)
{
	// move only on xz plane for land object
	if (_cameraType == LANDOBJECT)
	{
		_cameraPosition += D3DXVECTOR3(_right.x, 0.0f, _right.z) * units;
		if (player != nullptr)
		{
			static D3DXVECTOR3 playerPosition;
			playerPosition = player->getTransform()->position;
			playerPosition += D3DXVECTOR3(_right.x, 0.0f, _right.z) * units*10;
			player->getTransform()->position = playerPosition;
		}
	}

	if (_cameraType == AIRCRAFT)
		_cameraPosition += _right * units;
}

void Camera::fly(float units)
{
	// move only on y-axis for land object
	if (_cameraType == LANDOBJECT)
		_cameraPosition.y += units;

	if (_cameraType == AIRCRAFT)
		_cameraPosition += _up * units;
}

void Camera::pitch(float angle)
{
	D3DXMATRIX T;
	D3DXMatrixRotationAxis(&T, &_right, angle);

	// rotate _up and _look around _right vector
	//右向量保持不变，上向量和look向量旋转T矩阵代表的角度
	D3DXVec3TransformCoord(&_up, &_up, &T);
	D3DXVec3TransformCoord(&_look, &_look, &T);
	/*D3DXVec3TransformNormal(&_up, &_up, &T);
	D3DXVec3TransformNormal(&_look, &_look, &T);*/
}

void Camera::yaw(float angle)
{
	D3DXMATRIX T;

	// rotate around world y (0, 1, 0) always for land object
	if (_cameraType == LANDOBJECT)
		D3DXMatrixRotationY(&T, angle);

	// rotate around own up vector for aircraft
	if (_cameraType == AIRCRAFT)
		D3DXMatrixRotationAxis(&T, &_up, angle);

	// rotate _right and _look around _up or y-axis
	D3DXVec3TransformCoord(&_right, &_right, &T);
	D3DXVec3TransformCoord(&_look, &_look, &T);
}

void Camera::roll(float angle)
{
	// only roll for aircraft type
	if (_cameraType == AIRCRAFT)
	{
		D3DXMATRIX T;
		D3DXMatrixRotationAxis(&T, &_look, angle);

		// rotate _up and _right around _look vector
		D3DXVec3TransformCoord(&_right, &_right, &T);
		D3DXVec3TransformCoord(&_up, &_up, &T);
	}
}

void Camera::getViewMatrix(D3DXMATRIX* V)
{
	// Keep camera's axes orthogonal to eachother
	D3DXVec3Normalize(&_look, &_look);

	D3DXVec3Cross(&_up, &_look, &_right);
	D3DXVec3Normalize(&_up, &_up);

	D3DXVec3Cross(&_right, &_up, &_look);
	D3DXVec3Normalize(&_right, &_right);

	// Build the view matrix:
	float x = -D3DXVec3Dot(&_right, &_cameraPosition);
	float y = -D3DXVec3Dot(&_up, &_cameraPosition);
	float z = -D3DXVec3Dot(&_look, &_cameraPosition);

	/*(*V)(0,0) = _right.x; (*V)(0, 1) = _up.x; (*V)(0, 2) = _look.x; (*V)(0, 3) = 0.0f;
	(*V)(1,0) = _right.y; (*V)(1, 1) = _up.y; (*V)(1, 2) = _look.y; (*V)(1, 3) = 0.0f;
	(*V)(2,0) = _right.z; (*V)(2, 1) = _up.z; (*V)(2, 2) = _look.z; (*V)(2, 3) = 0.0f;
	(*V)(3,0) = x;        (*V)(3, 1) = y;     (*V)(3, 2) = z;       (*V)(3, 3) = 1.0f;*/

	V->_11 = _right.x;	V->_12 = _up.x;		V->_13 = _look.x;	V->_14 = 0.0f;
	V->_21 = _right.y;	V->_22 = _up.y;		V->_23 = _look.y;	V->_24 = 0.0f;
	V->_31 = _right.z;	V->_32 = _up.z;		V->_33 = _look.z;	V->_34 = 0.0f;
	V->_41 = x;			V->_42 = y;			V->_43 = z;			V->_44 = 1.0f;
}

void Camera::setCameraType(CameraType cameraType)
{
	_cameraType = cameraType;
}