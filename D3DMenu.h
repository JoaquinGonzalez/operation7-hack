#ifndef D3DMENU_HEADER
#define D3DMENU_HEADER

#include "Hacks.h"

#include "directx.h"
#include "Vector2.h"
#include "Size.h"
#include "MenuItem.h"

#define MAX_MENU_ITEMS 15

#define MGROUP_D3D			0
#define MITEM_WALLHACK		1
#define MITEM_WALLOPTN		2
#define MITEM_WALLOPTP		3
#define MITEM_WALLOPTC		4

#define MGROUP_ARMA			5
#define MITEM_FULLCONTROL	6
#define MITEM_MUNITION		7
#define MITEM_RAPIDSHOT		8
#define MITEM_GUNCRAZY		9

#define MGROUP_CHEATS		10
#define MITEM_STAMINA		11
#define MITEM_ASSASING		12
#define MITEM_BOMBS			13

#define MITEM_PASSOPT		14

class D3DMenu
{
private:
	MenuItem menuItems[MAX_MENU_ITEMS];

	LPD3DXFONT font;
	ID3DXSprite* sprite;
	D3DCOLOR fontColor;

	int menuIndex;
	bool menuActived;

	Vector2 menuPos;
	Size menuSize;

	D3DCOLOR menuColor;

	void DrawItems(IDirect3DDevice9* d3dDevice);
public:
	void Initialize();

	void CreateFont(IDirect3DDevice9* d3dDevice, char* fontName); 
	void CreateSprite( IDirect3DDevice9* d3dDevice);
	void DrawText(LPCWSTR text, Vector2 pos, D3DCOLOR color);
	void DrawMenu(IDirect3DDevice9* d3dDevice);
	void FillRect(IDirect3DDevice9* d3dDevice, Vector2 pos, Size size, D3DCOLOR color);
	void DrawBorderBox(IDirect3DDevice9* d3dDevice, Vector2 pos, Size size, int thickness, D3DCOLOR color);
	void KeyInput();
	void Clear();
	void Toggle();
};

#endif