#include "D3DMenu.h"
#include "D3DColors.h"
#include "xhacking\API\API.h"

void D3DMenu::Initialize()
{
	menuItems[MGROUP_D3D]			= MenuItem("D3D", 0, false, false, true, 4);
	menuItems[MITEM_WALLHACK]		= MenuItem("Wallhack",	1);
	menuItems[MITEM_WALLOPTN]		= MenuItem("Wallhack_N:", 2, false, true);
	menuItems[MITEM_WALLOPTP]		= MenuItem("Wallhack_P:", 3, false, true);
	menuItems[MITEM_WALLOPTC]		= MenuItem("Wallhack_Color:", 4, false, true);

	menuItems[MGROUP_ARMA]			= MenuItem("Arma", 5, false, false, true, 4);
	menuItems[MITEM_FULLCONTROL]	= MenuItem("FullControl",	6);
	menuItems[MITEM_MUNITION]		= MenuItem("Munition",	7);
	menuItems[MITEM_RAPIDSHOT]		= MenuItem("RapidShot",	8);
	menuItems[MITEM_GUNCRAZY]		= MenuItem("GunCrazy",	9);

	menuItems[MGROUP_CHEATS]		= MenuItem("Cheats", 10, false, false, true, 3);
	menuItems[MITEM_STAMINA]		= MenuItem("Stamina",		11);
	menuItems[MITEM_ASSASING]		= MenuItem("Assasing",		12);
	menuItems[MITEM_BOMBS]			= MenuItem("Bombs",			13);
	
	menuItems[MITEM_PASSOPT]		= MenuItem("Password:", 14, false, true);

	// Menu Config
	this->menuIndex = 0;
	this->menuActived = false;
}

void D3DMenu::Clear()
{
	if(this->sprite)
	{
		this->sprite->OnLostDevice();
		this->sprite->OnResetDevice();
	}
}

void D3DMenu::Toggle()
{
	(this->menuActived ? this->menuActived = false : this->menuActived = true);
}

void D3DMenu::CreateFont(IDirect3DDevice9* d3dDevice, char* fontName)
{
	if(!this->font)
	{
		D3DXCreateFont(d3dDevice, 15, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		(LPCWSTR)fontName, &this->font);
	}
}

void D3DMenu::CreateSprite(IDirect3DDevice9* d3dDevice)
{
	if(!this->sprite)
	{
		D3DXCreateSprite(d3dDevice, &this->sprite);
	}
}

void D3DMenu::DrawText(LPCWSTR text, Vector2 pos, D3DCOLOR color)
{
	RECT rc;
    SetRect( &rc, pos.x, pos.y, 0, 0 );

	this->sprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	this->font->DrawTextW( this->sprite, text, -1, &rc, DT_CALCRECT, color );
	this->font->DrawTextW( this->sprite, text, -1, &rc, DT_NOCLIP, color );

	this->sprite->End();
}

void D3DMenu::DrawItems(IDirect3DDevice9* d3dDevice)
{

}

void D3DMenu::DrawMenu(IDirect3DDevice9* d3dDevice)
{
	if(this->menuActived)
	{
		int menuStartPosX = 45;
		int menuStartPosY = 15;
		int menuHeaderH = 20;
		int menuMargen = 5;
		int borderThiwn = 2;
		int menuBoxX = menuStartPosX;
		int menuBoxY = menuStartPosY + menuHeaderH + menuMargen;

		// Menu Header
		FillRect(d3dDevice, Vector2(menuStartPosX, menuStartPosY), Size(220, 20), Black);

		DrawBorderBox(d3dDevice, Vector2(menuStartPosX - 2, menuStartPosY - 2), Size(220, 20), 2, GOLD2);

		// Menu Title
		DrawText(L"TCL100K & Raitx V1.0", Vector2(menuStartPosX + 2, menuStartPosY + 1), White);

		// Menu Box
		FillRect(d3dDevice, Vector2(menuBoxX, menuBoxY), Size(220, 30 * MAX_MENU_ITEMS + 30), Black);

		// Menu Border
		DrawBorderBox(d3dDevice, Vector2(menuBoxX - borderThiwn, menuBoxY - borderThiwn), Size(220, 30 * MAX_MENU_ITEMS + 30), 2, GOLD2);

		// Draw Items
		int y = menuBoxY + 1;
		bool inGroup = false;
		int startGroup = 0;
	}
}