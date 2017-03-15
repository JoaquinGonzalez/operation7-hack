#pragma once

class CDirectDraw
{
private:
	IDirect3DDevice9* m_pDevice;
	ID3DXFont* m_pFont;
	ID3DXSprite* m_pSprite;

public:
	CDirectDraw(IDirect3DDevice9* pDevice);
	~CDirectDraw(void);

public:
	void CDirectDraw::DrawTextB(const char* Format, int nSize, int x, int y, D3DCOLOR color);
	HRESULT DrawText(LPCWSTR lpszText, int nSize, int x, int y, D3DCOLOR color);
	void  CDirectDraw::Draw_Box(int x, int y, int w, int h, D3DCOLOR color);
};
