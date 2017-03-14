#include "Hacks.h"
#include "BypassXigncode.h"
#include <iostream>
#include <cstdint>

#include "DirectxHook.h"

#include "xhacking\xHacking.h"
#include "xhacking\API\API.h"

#pragma comment(lib, "xHacking")

// POINTER VALUES
int		*P_bulletsP		= (int*)ADDRESS_BULLET_P;
int		*P_bulletsS		= (int*)ADDRESS_BULLET_S;
int		*P_fullControlP	= (int*)ADDRESS_FULLCONTROL_P;
int		*P_fullControlS	= (int*)ADDRESS_FULLCONTROL_S;
byte	*P_stamina		= (byte*)ADDRESS_STAMINA;
int		*P_rapidP		= (int*)ADDRESS_RAPID_P;
short	*P_rapidS		= (short*)ADDRESS_RAPID_S;
int		*P_noreoil		= (int*)ADDRESS_NORECOIL;
long	*P_bombs		= (long*)ADDRESS_BOMBS;
int		*P_assasing		= (int*)ADDRESS_ASSASING;
byte	*P_guncrazy		= (byte*)ADDRESS_GUNCRAZY;
byte	*P_munition		= (byte*)ADDRESS_MUNITION;
char	*P_passhack		= (char*)ADDRESS_PASSHACK;

// CACHE VALUES
int C_bulletP;
int C_bulletS;
int C_fullControlP;
int C_fullControlS;
int C_stamina;
int C_rapidP;
short C_rapidS;
byte C_norecoil;

bool bulletActived			= false;
bool fullControlActived		= false;
bool staminaActived			= false;
bool noReoilActived			= false;
bool rapidShootActived		= false;
bool bombsActived			= false;
bool assasingActived		= false;
bool guncrazyActived		= false;
bool wallhackActived		= false;
bool munitionActived		= false;

byte guncrazyIndex = 0;
byte guncrazyMin = 1;
byte guncrazyMax = 15;

// WALL HACK
LPDIRECT3DTEXTURE9 texRed2;
bool texCreated = false;
int wallColorSelected = WALLHACK_COLOR_RED;

// WALLHACK PARAMETERS
UINT N = 300; UINT P = 600;

UINT GetWallN()
{
	return N;
}

UINT GetWallP()
{
	return P;
}

char GetPassHack()
{
	return *P_passhack;
}

char* GetPassPointer()
{
	return P_passhack;
}

void ChangeWallN(int action)
{
	if(action == OPTION_INCREASE) N++;
	else if(action == OPTION_DECREASE) N--;
}

void ChangeWallP(int action)
{
	if(action == OPTION_INCREASE) P++;
	else if(action == OPTION_DECREASE) P--;
}

HRESULT GenerateTexture(IDirect3DDevice9 *pD3Ddev, DWORD colour32)
{
	IDirect3DTexture9 **ppD3Dtex = &texRed2;

    if( FAILED(pD3Ddev->CreateTexture(8, 8, 1, 0, D3DFMT_A4R4G4B4, D3DPOOL_MANAGED, ppD3Dtex, NULL)) )
        return E_FAIL;

    WORD colour16 = ((WORD)((colour32>>28)&0xF)<<12)
            |(WORD)(((colour32>>20)&0xF)<<8)
            |(WORD)(((colour32>>12)&0xF)<<4)
            |(WORD)(((colour32>>4)&0xF)<<0);

    D3DLOCKED_RECT d3dlr;    

    (*ppD3Dtex)->LockRect(0, &d3dlr, 0, 0);

    WORD *pDst16 = (WORD*)d3dlr.pBits;

    for(int xy=0; xy < 8*8; xy++)
        *pDst16++ = colour16;

    (*ppD3Dtex)->UnlockRect(0);

    return S_OK;
}

void UpdateHacks()
{
	if(bulletActived) // F5
	{
		//C_bulletP = (int&)P_bulletsP;
		//C_bulletS = (int&)P_bulletsS;

		//*P_bulletsP = 999;
		//*P_bulletsS = 999;
	}

	if(fullControlActived) // F6
	{
		*P_fullControlP = 6;
		*P_fullControlS = 6;
	}

	if(staminaActived) // F7
	{
		//int C_stamina = (int&)P_stamina;
		*P_stamina = 0x7B;
	}

	if(noReoilActived) // F8
	{
		int C_norecoil = (int&)P_noreoil;
		*P_noreoil = C_norecoil;
	}

	if(rapidShootActived) // F9
	{
		
		//C_rapidS = (short&)P_rapidS;

		*P_rapidP = 20;
		//*P_rapidS = 20;
	}

	if(bombsActived)
	{
		long b = (long&)P_bombs;
		*P_bombs = b;
	}

	if(assasingActived)
	{
		int as = (int&)P_assasing;
		*P_assasing = as;
	}

	if(guncrazyActived)
	{
		DWORD dwOldProtect;
		VirtualProtect((LPVOID)ADDRESS_GUNCRAZY, 1, PAGE_EXECUTE_READWRITE, &dwOldProtect);
		*P_guncrazy = guncrazyIndex;
		VirtualProtect((LPVOID)ADDRESS_GUNCRAZY, 1, dwOldProtect, &dwOldProtect);
	}

	if(munitionActived)
	{
		*P_munition = 0x35;
	}
}

void ToggleRS(int a)
{
	(rapidShootActived ? rapidShootActived = false : rapidShootActived = true);

	if(!rapidShootActived)
	{
		*P_rapidP = 4;
		//*P_rapidS = C_rapidS;
	}
}

void ToggleBullet()
{
	(bulletActived ? bulletActived = false : bulletActived = true);

	if(!bulletActived)
	{
		//*P_bulletsP = C_bulletP;
		//*P_bulletsS = C_bulletS;
	}
}

void ToggleFullControl()
{
	(fullControlActived ? fullControlActived = false : fullControlActived = true);

	if(!fullControlActived)
	{
		*P_fullControlP = 0;
		//*P_fullControlS = C_fullControlS;
	}
	else
	{
		//C_fullControlP = (int&)P_fullControlP;
		//C_fullControlS = (int&)P_fullControlS;
	}
}

void ToggleStamina()
{
	(staminaActived ? staminaActived = false : staminaActived = true);

	if(!staminaActived)
	{
		//*P_stamina = C_stamina;
	}
}

void ToggleNorecoil()
{
	(noReoilActived ? noReoilActived = false : noReoilActived = true);

	if(!noReoilActived)
	{
		//*P_noreoil = C_norecoil;
	}
}

void ToggleBombs()
{
	(bombsActived ? bombsActived = false : bombsActived = true);
}

void ToggleAssasing()
{
	(assasingActived ? assasingActived = false : assasingActived = true);
}

void ToggleWallhack()
{
	(wallhackActived ? wallhackActived = false : wallhackActived = true);
}

void ToggleGuncrazy()
{
	if(guncrazyIndex + 1 < guncrazyMax)
	{
		guncrazyActived = true;
		guncrazyIndex++;
	}
	else
	{
		guncrazyActived = false;
		guncrazyIndex = 0;
	}
}

void ToggleMunition()
{
	(munitionActived ? munitionActived = false : munitionActived = true);
}

void ChangeWallC(int action)
{
	if(action == OPTION_INCREASE && wallColorSelected < 4)
	{
		wallColorSelected++;
		texCreated = false;
	}

	if(action == OPTION_DECREASE && wallColorSelected > 1)
	{
		wallColorSelected--;
		texCreated = false;
	}
}

char* GetWallC()
{
	switch (wallColorSelected)
	{
	case WALLHACK_COLOR_RED:
		return "Red";
	case WALLHACK_COLOR_BLUE:
		return "Blue";
	case WALLHACK_COLOR_GREEN:
		return "Green";
	case WALLHACK_COLOR_BLACK:
		return "Black";
	default:
		break;
	}
}

void GenerateWallTexture(IDirect3DDevice9* d3dDevice)
{
	switch (wallColorSelected)
	{
	case WALLHACK_COLOR_RED:
		GenerateTexture(d3dDevice, Red);
		break;
	case WALLHACK_COLOR_BLUE:
		GenerateTexture(d3dDevice, Blue);
		break;
	case WALLHACK_COLOR_GREEN:
		GenerateTexture(d3dDevice, Green);
		break;
	case WALLHACK_COLOR_BLACK:
		GenerateTexture(d3dDevice, Black);
		break;
	default:
		break;
	}
}

void Wallhack(IDirect3DDevice9* d3dDevice, D3DPRIMITIVETYPE device, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount)
{
	if(!texCreated)
	{
		GenerateWallTexture(d3dDevice);
		texCreated = true;
	}

	if(NumVertices > N && primCount > P &&  wallhackActived == true)
	{
		d3dDevice->SetRenderState(D3DRS_ZENABLE, false);
		d3dDevice->SetTexture(0, texRed2);
		d3dDevice->DrawIndexedPrimitive(device, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
		d3dDevice->SetRenderState(D3DRS_ZENABLE, true);
	}
}

void AttachKeys()
{
	xHacking::API<SHORT, int> api;

	if(GetAsyncKeyState(BULLET_HOTKEY))			ToggleBullet();
	api.Fake(0xC3);

	if(GetAsyncKeyState(FULLCONTROL_HOTKEY))	ToggleFullControl();
	api.Fake(0xC3);

	if(GetAsyncKeyState(STAMINA_HOTKEY))		ToggleStamina();
	api.Fake(0xC3);

	if(GetAsyncKeyState(NORECOIL_HOTKEY))		ToggleNorecoil();
	api.Fake(0xC3);

	//if(GetAsyncKeyState(RAPIDSHOT_HOTKEY))		ToogleRapidShoot();
	api.Fake(0xC3);

	if(GetAsyncKeyState(BOMBS_HOTKEY))			ToggleBombs();
	api.Fake(0xC3);

	if(GetAsyncKeyState(ASSASING_HOYKEY))		ToggleAssasing();
	api.Fake(0xC3);

	if(GetAsyncKeyState(GUNCRAZY_HOTKEY))		ToggleGuncrazy();
	api.Fake(0xC3);
}

void HackLoop()
{
	while(true)
	{
		AttachKeys();
		UpdateHacks();

		Sleep(400);
	}
}

void StartHack()
{
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)HackLoop, 0, 0, 0);

	std::cout << "Hack Thread Started" << std::endl;
}