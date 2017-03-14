#include "D3DMenu.h"
#include "D3DColors.h"
#include "xhacking\API\API.h"

#include <sstream>

struct D3D9Textures
{
	LPDIRECT3DTEXTURE9 Red;
	LPDIRECT3DTEXTURE9 Green;
	LPDIRECT3DTEXTURE9 Blue;
	LPDIRECT3DTEXTURE9 White;
} d3dTextures;

struct D3DColors
{
	D3DCOLOR red;
	D3DCOLOR green;
	D3DCOLOR blue;
	D3DCOLOR white;
	D3DCOLOR black;

	D3DColors()
	{
		red		= D3DCOLOR_ARGB(255, 255, 0, 0);
		green	= D3DCOLOR_ARGB(255, 0, 255, 0);
		blue	= D3DCOLOR_ARGB(255, 0, 0, 255);
		white	= D3DCOLOR_ARGB(255, 255, 255, 255);
		black	= D3DCOLOR_ARGB(255, 0, 0, 0);
	}
} d3dColors;

void D3DMenu::Initialize()
{
	this->gDDC = gDDC;

	// Initialize Menu Items
	// === COMING SOON ==
	/*menuItems.ESPName		= D3DMenuItem("ESP Name",		7);
	menuItems.WallHack		= D3DMenuItem("Wall Hack",		8);
	menuItems.Aimbot		= D3DMenuItem("Aimbot",			9);
	menuItems.Teleport		= D3DMenuItem("Teleport",		10);
	menuItems.Ghostmode		= D3DMenuItem("Ghostmode",		11);
	menuItems.WallName		= D3DMenuItem("Wall Name",		12);
	menuItems.AntiJam		= D3DMenuItem("Anti JAM",		13);
	menuItems.WeaponHack	= D3DMenuItem("Weapon Hack",	14);
	*/

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
	this->fontColor = d3dColors.green;
	this->menuActived = false;

	this->menuPos.x = 55;
	this->menuPos.y = 20;

	this->menuSize.width = 100;
	this->menuSize.height = 200;

	this->menuColor = d3dColors.blue;
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

		for(int i = 0; i < MAX_MENU_ITEMS; i++)
		{
			D3DCOLOR color;

			if(menuItems[i].option)
			{
				color = White;
			}
			else if(menuItems[i].group)
			{
				color = Yellow;
			}
			else
			{
				color = (menuItems[i].actived ? Green : Red);
			}
			
			if(this->menuIndex == i)
			{
				this->DrawBorderBox(d3dDevice, Vector2(45, y + (20 * i) - 2), Size(220, 20), 2, d3dColors.white);
			}

			std::wstringstream wss;
			std::wstring str;
			wss << menuItems[i].name;
			wss >> str;
			LPCWSTR nr = str.c_str();

			if(inGroup)
			{
				this->DrawText((LPCWSTR)nr, Vector2(50, startGroup), color);
				inGroup = false;
			}
			else
			{
				this->DrawText((LPCWSTR)nr, Vector2(50, y + (20 * i)), color);
			}

			// Print value option
			if(menuItems[i].option)
			{
				std::wstringstream wss2;
				std::wstring str2;

				char* ptr = GetPassPointer();
				
				switch(menuItems[i].index)
				{
				case MITEM_WALLOPTN:
					wss2 << GetWallN();
					break;
				case MITEM_WALLOPTP:
					wss2 << GetWallP();
					break;
				case MITEM_WALLOPTC:
					wss2 << GetWallC();
					break;
				case MITEM_PASSOPT:
					wss2 << GetPassHack();
					
					// Print string
					for(int x = 1;x <= 20; x++)
						wss2 << ptr[x];

					break;
				default:
					break;
				}

				wss2 >> str2;

				// Value Option
				LPCWSTR vop = str2.c_str();

				this->DrawText(vop, Vector2(50 + strlen(menuItems[i].name) * 7, y + (20 * i)), White);
			}
			else if(menuItems[i].group && menuItems[i].actived)
			{
				i = i + menuItems[i].groupItems;
				startGroup = 20 * i + 5;
				continue;
			}
		}

		this->DrawText(L"Selecciona usando las flechas", Vector2(50, (30 * MAX_MENU_ITEMS)), White);
	}
}

void D3DMenu::FillRect(IDirect3DDevice9* d3dDevice, Vector2 pos, Size size, D3DCOLOR color)
{
	struct Vertex
	{
		float x, y, z, ht;
		DWORD Color;
	}
	V[4] = { { pos.x, pos.y + size.height, 0.0f, 0.0f, color }, { pos.x, pos.y, 0.0f, 0.0f, color }, { pos.x + size.width, pos.y + size.height, 0.0f, 0.0f, color }, { pos.x + size.width, pos.y, 0.0f, 0.0f, color } };
	d3dDevice->SetTexture(0, NULL);
	d3dDevice->SetPixelShader(0);
	d3dDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	d3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	d3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	d3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, V, sizeof(Vertex));
}

void D3DMenu::DrawBorderBox(IDirect3DDevice9* d3dDevice, Vector2 pos, Size size, int thickness, D3DCOLOR color)
{
	this->FillRect(d3dDevice, pos, Size(size.width, thickness), color); // TOP
	this->FillRect(d3dDevice, pos, Size(thickness, size.height), color); // LEFT
	this->FillRect(d3dDevice, Vector2(pos.x + size.width, pos.y), Size(thickness, size.height + thickness), color); // RIGHT
	this->FillRect(d3dDevice, Vector2(pos.x, pos.y + size.height), Size(size.width, thickness), color); // BOTTOM
}

void ToggleHack(int id)
{
	switch(id)
	{
	case MITEM_FULLCONTROL:
		ToggleFullControl();
		break;
	case MITEM_STAMINA:
		ToggleStamina();
		break;
	case MITEM_MUNITION:
		ToggleMunition();
		break;
	case MITEM_RAPIDSHOT:
		ToggleRS(10);
		break;
	case MITEM_ASSASING:
		ToggleAssasing();
		break;
	case MITEM_BOMBS:
		ToggleBombs();
		break;
	case MITEM_GUNCRAZY:
		ToggleGuncrazy();
		break;
	case MITEM_WALLHACK:
		ToggleWallhack();
		break;
	default:
		break;
	}
}

void ChangeOption(int index, int action)
{
	switch (index)
	{
	case MITEM_WALLOPTN:
		ChangeWallN(action);
		break;
	case MITEM_WALLOPTP:
		ChangeWallP(action);
		break;
	case MITEM_WALLOPTC:
		ChangeWallC(action);
		break;
	default:
		break;
	}
}

void D3DMenu::KeyInput()
{
	xHacking::API<SHORT, INT> api;

	if(GetAsyncKeyState(VK_UP) && this->menuIndex >= 0) this->menuIndex--;
	api.Fake(0xC3);

	if(GetAsyncKeyState(VK_DOWN) && this->menuIndex <= MAX_MENU_ITEMS) this->menuIndex++;
	api.Fake(0xC3);

	if(GetAsyncKeyState(VK_RIGHT) && 0x8000) 
	{
		if(menuItems[this->menuIndex].option)
		{
			ChangeOption(menuItems[this->menuIndex].index, OPTION_INCREASE);
		}
		else if(!menuItems[this->menuIndex].actived)
		{
			ToggleHack(menuItems[this->menuIndex].index);
			menuItems[this->menuIndex].actived = true;
		}
	}
	api.Fake(0xC3);

	if(GetAsyncKeyState(VK_LEFT) && 0x8000)
	{
		if(menuItems[this->menuIndex].option)
		{
			ChangeOption(menuItems[this->menuIndex].index, OPTION_DECREASE);
		}
		else if(menuItems[this->menuIndex].actived)
		{
			ToggleHack(menuItems[this->menuIndex].index);
			menuItems[this->menuIndex].actived = false;
		}
	}
	api.Fake(0xC3);

	UpdateHacks();
}