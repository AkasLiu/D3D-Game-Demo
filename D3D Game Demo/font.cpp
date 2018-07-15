#include "Font.h"

Font::Font(int fontHeight, LPDIRECT3DDEVICE9 device, HWND hwnd) :fontHeight(fontHeight), pDevice(device), hwnd(hwnd)
{
}

Font::~Font()
{
}

LPD3DXFONT Font::getD3DXFont()
{
	return pFont;
}

HRESULT Font::createFont()
{
	if (FAILED(D3DXCreateFont(pDevice, 36, 0, 0, 1, false, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, _T("微软雅黑"), &pFont)))
		return E_FAIL;
}

void Font::drawText(int h, int w, LPCTSTR text, D3DXCOLOR color)
{
	//定义一个矩形，用于获取主窗口矩形
	RECT formatRect;
	GetClientRect(hwnd, &formatRect);

	if (h >= 0 && w >= 0)
	{
		formatRect.top = h;
		formatRect.left = w;
	}
	pFont->DrawText(0, text, -1, &formatRect, DT_CENTER, color);
}