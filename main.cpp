#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1

#include <windows.h>
#include <iostream>
#include <cstdint>

#include "BypassXigncode.h"
#include "MemoryScanner.h"
//#include "Hacks.h"
//#include "D3DMenu.h"
#include "DirectxHook.h"

#include "Protection.h"

#define DEBUG_MODE FALSE
#define BYPASS_XIGNCODE FALSE

BOOL APIENTRY DllMain(__in HINSTANCE hInstance, __in DWORD fdwReason, __in __reserved LPVOID lpReserved) {
	UNREFERENCED_PARAMETER(lpReserved);
	DisableThreadLibraryCalls(hInstance);
	//AntiHeaders(hInstance);
	AntiDebug();
	AntiDumping();
	AntiRevers(hInstance);
	//HideModule(hInstance);

	if(DEBUG_MODE)
	{
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
	}

	if (fdwReason == DLL_PROCESS_ATTACH) {
		std::cout << "Hack Injected By TCL-100K" << std::endl;

		if(BYPASS_XIGNCODE)
		{
			BypassXigncode();
		}

		//BypassInstance();

		InitializeHook();
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)KeyWall, 0, 0, 0);

		//StartHack();
		//StartMemoryScan();
	}
	/*else if(fdwReason == DLL_PROCESS_DETACH)
	{
		RemoveXignRegister();
	}*/
	return TRUE;
}