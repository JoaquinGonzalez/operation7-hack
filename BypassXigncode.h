#ifndef BYPASS_XIGNCODE_HEADER
#define BYPASS_XIGNCODE_HEADER

#include <windows.h>
#include <strsafe.h>
#include <tlhelp32.h>

BOOL RegDelnode (HKEY hKeyRoot, LPTSTR lpSubKey);
BOOL RegDelnodeRecurse (HKEY hKeyRoot, LPTSTR lpSubKey);

BOOL WriteBytes(DWORD dwAddress, LPBYTE lpBuffer, DWORD nBytes);

// Bypass Functions
void BypassInstance();
void RemoveXignRegister();
BOOL StopXignInit();
void BypassXigncode();

#endif