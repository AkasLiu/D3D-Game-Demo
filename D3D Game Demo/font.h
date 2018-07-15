#pragma once

#include "d3dUtility.h"

class Font
{
public:
	Font(int fontHeight, LPDIRECT3DDEVICE9 device, HWND hwnd);
	~Font();
	LPD3DXFONT getD3DXFont();
	void drawText(int h, int w, LPCTSTR text, D3DXCOLOR color);
	HRESULT createFont();

private:
	LPDIRECT3DDEVICE9 pDevice;
	LPD3DXFONT pFont;
	D3DXCOLOR fontColor;
	int fontHeight = 0;
	int fontWidth = 0;
	HWND hwnd;
};