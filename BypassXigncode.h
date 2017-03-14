#ifndef BYPASS_XIGNCODE_HEADER
#define BYPASS_XIGNCODE_HEADER

#include <windows.h>
#include <stdio.h>
#include <strsafe.h>
#include <iostream>
#include <tlhelp32.h>

BOOL RegDelnode (HKEY hKeyRoot, LPTSTR lpSubKey);
BOOL RegDelnodeRecurse (HKEY hKeyRoot, LPTSTR lpSubKey);

BOOL WriteBytes(DWORD dwAddress, LPBYTE lpBuffer, DWORD nBytes);
DWORD FindProcessId(const std::wstring& processName);

typedef LONG (NTAPI *NtSuspendProcess)(IN HANDLE ProcessHandle);

void suspend(DWORD processId);

void BypassInstance();
void RemoveXignRegister();
BOOL StopXignInit();
void BypassXigncode();
void StopWatchDog();

#endif