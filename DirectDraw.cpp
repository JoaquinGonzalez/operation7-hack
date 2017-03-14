#include "directx.h"
#include "DirectDraw.h"


CDirectDraw::CDirectDraw(IDirect3DDevice9* pDevice) :
	m_pDevice(pDevice),
	m_pFont(NULL),
	m_pSprite(NULL)

{
	D3DXCreateFontW( pDevice, 15, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
                              OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                              L"Consolas", &m_pFont );
	D3DXCreateSprite(pDevice, &m_pSprite);
}

CDirectDraw::~CDirectDraw(void)
{
	if(m_pFont)
		m_pFont->Release();
	if(m_pSprite)
		m_pSprite->Release();
}

void  CDirectDraw::Draw_Box(int x, int y, int w, int h, D3DCOLOR color)
{
	struct Vertex
	{
		float x, y, z, ht;
		DWORD Color;
	}
	V[4] = { { x, y + h, 0.0f, 0.0f, color }, { x, y, 0.0f, 0.0f, color }, { x + w, y + h, 0.0f, 0.0f, color }, { x + w, y, 0.0f, 0.0f, color } };
	m_pDevice->SetTexture(0, NULL);
	m_pDevice->SetPixelShader(0);
	m_pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, V, sizeof(Vertex));
	return;
}

HRESULT CDirectDraw::DrawText(LPCWSTR lpszText, int nSize, int x, int y, D3DCOLOR color)
{
	HRESULT hr;

    RECT rc;
    SetRect( &rc, x, y, 0, 0 );

	if(m_pSprite)
		 hr = m_pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	if(m_pFont)
	{
		hr = m_pFont->DrawTextW( m_pSprite, lpszText, nSize, &rc, DT_CALCRECT, color );
		hr = m_pFont->DrawTextW( m_pSprite, lpszText, nSize, &rc, DT_NOCLIP, color );
	}

	if(m_pSprite)
		hr = m_pSprite->End();

	return hr;
}
