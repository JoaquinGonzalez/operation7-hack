#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1

#include <Windows.h>

#include "Protection.h"
#include "BypassXigncode.h"
#include "D3DHook.h"

#define DEBUG_MODE		FALSE
#define BYPASS_XIGNCODE FALSE

BOOL APIENTRY DllMain(__in HINSTANCE hInstance, __in DWORD fdwReason, __in __reserved LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);
	DisableThreadLibraryCalls(hInstance);

	// Protection
	AntiDebug();
	AntiDumping();
	AntiRevers(hInstance);

	if(DEBUG_MODE)
	{
		AllocConsole();
	}

	switch(fdwReason)
	{
	case DLL_PROCESS_ATTACH:

		// Start directx hooking
		InitializeHook();

		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)UpdateHack, 0, 0, 0);
		break;
	case DLL_PROCESS_DETACH:
		// Destroy directx hooks
		DestroyHooks();
		break;
	default:
		break;
	}
}