#ifndef HACKS_HEADER
#define HACKS_HEADER

#include "directx.h"
#include <Windows.h>
#include "D3DColors.h"

#define BULLET_HOTKEY		VK_F5
#define FULLCONTROL_HOTKEY	VK_F6
#define STAMINA_HOTKEY		VK_F7
#define NORECOIL_HOTKEY		VK_F8
#define RAPIDSHOT_HOTKEY	VK_F9
#define ASSASING_HOYKEY		VK_F10
#define BOMBS_HOTKEY		VK_F11
#define GUNCRAZY_HOTKEY		VK_F12

#define ADDRESS_BULLET_P		0x00CD9358
#define ADDRESS_BULLET_S		0x00CD93A0
#define ADDRESS_FULLCONTROL_P	0x00CE5DB4
#define ADDRESS_FULLCONTROL_S	0x00CE5DB4
#define ADDRESS_STAMINA			0x00CC88B0
#define ADDRESS_RAPID_P			0x00CD933C
#define ADDRESS_RAPID_S			0x00CD9384
#define ADDRESS_NORECOIL		0x00CE5DB0
#define ADDRESS_ASSASING		0x00CC882C
#define ADDRESS_BOMBS			0x00CD83F8
#define ADDRESS_GUNCRAZY		0x00CD85B0
#define ADDRESS_MUNITION		0x00CD935C
#define ADDRESS_PASSHACK		0x00BC28C0

#define OPTION_INCREASE 1
#define OPTION_DECREASE 2

#define WALLHACK_COLOR_RED		1
#define WALLHACK_COLOR_BLUE		2
#define WALLHACK_COLOR_GREEN	3
#define WALLHACK_COLOR_BLACK	4

// ======= DIRECTX HACKS ===========
void Wallhack(IDirect3DDevice9* d3dDevice, D3DPRIMITIVETYPE device, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount);

HRESULT GenerateTexture(IDirect3DDevice9 *pD3Ddev, DWORD colour32);

UINT GetWallN();
UINT GetWallP();
void ChangeWallN(int);
void ChangeWallP(int);
void ChangeWallC(int);
char* GetWallC();

char GetPassHack();
char* GetPassPointer();

void ToggleRS(int a);
void ToggleBullet();
void ToggleFullControl();
void ToggleStamina();
void ToggleNorecoil();
void ToggleAssasing();
void ToggleBombs();
void ToggleGuncrazy();
void ToggleWallhack();
void ToggleMunition();

void AttachKeys();
void UpdateHacks();
void HackLoop();
void StartHack();

#endif