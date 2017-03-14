#ifndef DIRECTXHOOK_HEADER
#define DIRECTXHOOK_HEADER

IDirect3D9* WINAPI Direct3DCreate9Callback(UINT SDKVersion);

int InitializeHook(void);
void UpdateHack();

#endif