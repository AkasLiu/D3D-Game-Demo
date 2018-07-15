#pragma once
#include <d3dx9.h>
#include "character.h"

class Camera
{
public:
	enum CameraType { LANDOBJECT, AIRCRAFT };

	Camera();
	Camera(CameraType cameraType);
	~Camera();

	//void strafe(float units); // left/right
	void fly(float units);    // up/down
	//void walk(float units);   // forward/backward

	void pitch(float angle); // rotate on right vector
	void yaw(float angle);   // rotate on up vector
	void roll(float angle);  // rotate on look vector

	void getViewMatrix(D3DXMATRIX* V);
	void setCameraType(CameraType cameraType);
	void getCameraPosition(D3DXVECTOR3* pos);
	void setCameraPosition(D3DXVECTOR3* pos);

	void getRight(D3DXVECTOR3* right);
	void getUp(D3DXVECTOR3* up);
	void getLook(D3DXVECTOR3* look);

	void SetTargetPosition(D3DXVECTOR3 *pLookat = NULL);  //设置摄像机的目标观察位置向量

	void ThirdPersonRotationX(float angle);
	void walk(float units, Character* player = nullptr);
	void strafe(float units, Character * player = nullptr);
	//第三人称摄像头绕右分量向量旋转
	void ThirdPersonRotationY(float angle);//第三人称摄像头绕上分量向量旋转

private:
	CameraType  _cameraType;
	D3DXVECTOR3 _right;
	D3DXVECTOR3 _up;
	D3DXVECTOR3 _look;
	D3DXVECTOR3 _cameraPosition;
	D3DXVECTOR3 _targetPosition;
};