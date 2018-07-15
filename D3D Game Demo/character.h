#pragma once
#include "gameObject.h"

class Character : public GameObject
{
public:
	Character(Transform tf,LPDIRECT3DDEVICE9 pDevice);
	~Character();

	void init();
	void draw();

private:

};

