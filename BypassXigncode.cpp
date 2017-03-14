#include "BypassXigncode.h"
#include <string>

void RemoveXignRegister()
{
	if(RegDelnode(HKEY_LOCAL_MACHINE, L"SYSTEM\\ControlSet001\\Services\\xhunter1"))
	{
		std::cout << "xhunter1 Removed Success" << std::endl;
	}
	else
	{
		std::cout << "Fail to remove xhunter1" << std::endl;
	}
}

BOOL StopXignInit() {
	/*
	Modify code to:
	nop
	nop
	nop
	nop
	nop

	lo que modifica es un call carga el x3.xem
	*/
	return WriteBytes(0x006C0521, (PBYTE)"\x90\x90\x90\x90\x90", 5);
}

void BypassInstance()
{
	if(WriteBytes(0x006C04D9, (PBYTE)"\0x73\0x29", 2))
	{
		std::cout << "Multi-Account Actived" << std::endl;
	}
	else
	{
		std::cout << "Multi-Account Failed" << std::endl;
	}
}

void BypassXigncode()
{
	if (StopXignInit()) {
		std::cout << "Successfully stopped XIGNCODE3 from initializing!" << std::endl;
	}
	else {
		std::cout << "Failed to stop XIGNCODE3 from initializing!" << std::endl;
	}
}

void StopWatchDog()
{
	std::string name("xxd-0.xem");
	std::wstring processName(name.begin(), name.end());
	suspend(FindProcessId(processName));
}

BOOL WriteBytes(DWORD dwAddress, LPBYTE lpBuffer, DWORD nBytes) {
	if (nBytes == 0) {
		std::cout << "WriteBytes:: Tried to write 0 bytes to memory." << std::endl;
		return FALSE;
	}
	DWORD dwOldProtect;
	if (!VirtualProtect((LPVOID)dwAddress, nBytes, PAGE_EXECUTE_READWRITE, &dwOldProtect)) {
		std::cout << "WriteBytes:: Failed to change access protection on memory address." << std::endl;
		return FALSE;
	}
	memcpy((LPVOID)dwAddress, lpBuffer, nBytes);
	if (!VirtualProtect((LPVOID)dwAddress, nBytes, dwOldProtect, &dwOldProtect)) {
		std::cout << "WriteBytes:: Failed to change access protection on memory address." << std::endl;
		return FALSE;
	}

	return TRUE;
}

void suspend(DWORD processId)
{
	HANDLE processHandle = OpenProcess(PROCESS_SUSPEND_RESUME, FALSE, processId);

    NtSuspendProcess pfnNtSuspendProcess = (NtSuspendProcess)GetProcAddress(
        GetModuleHandle(L"ntdll"), "NtSuspendProcess");

    pfnNtSuspendProcess(processHandle);
    CloseHandle(processHandle);
}

DWORD FindProcessId(const std::wstring& processName)
{
	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(processInfo);

	HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if ( processesSnapshot == INVALID_HANDLE_VALUE )
		return 0;

	Process32First(processesSnapshot, &processInfo);
	if ( !processName.compare(processInfo.szExeFile) )
	{
		CloseHandle(processesSnapshot);
		return processInfo.th32ProcessID;
	}

	while ( Process32Next(processesSnapshot, &processInfo) )
	{
		if ( !processName.compare(processInfo.szExeFile) )
		{
			CloseHandle(processesSnapshot);
			return processInfo.th32ProcessID;
		}
	}
	
	CloseHandle(processesSnapshot);
	return 0;
}


//*************************************************************
//
//  RegDelnodeRecurse()
//
//  Purpose:    Deletes a registry key and all its subkeys / values.
//
//  Parameters: hKeyRoot    -   Root key
//              lpSubKey    -   SubKey to delete
//
//  Return:     TRUE if successful.
//              FALSE if an error occurs.
//
//*************************************************************

BOOL RegDelnodeRecurse (HKEY hKeyRoot, LPTSTR lpSubKey)
{
    LPTSTR lpEnd;
    LONG lResult;
    DWORD dwSize;
    TCHAR szName[MAX_PATH];
    HKEY hKey;
    FILETIME ftWrite;

    // First, see if we can delete the key without having
    // to recurse.

    lResult = RegDeleteKey(hKeyRoot, lpSubKey);

    if (lResult == ERROR_SUCCESS) 
        return TRUE;

    lResult = RegOpenKeyEx (hKeyRoot, lpSubKey, 0, KEY_READ, &hKey);

    if (lResult != ERROR_SUCCESS) 
    {
        if (lResult == ERROR_FILE_NOT_FOUND) {
            printf("Key not found.\n");
            return TRUE;
        } 
        else {
            printf("Error opening key.\n");
            return FALSE;
        }
    }

    // Check for an ending slash and add one if it is missing.

    lpEnd = lpSubKey + lstrlen(lpSubKey);

    if (*(lpEnd - 1) != TEXT('\\')) 
    {
        *lpEnd =  TEXT('\\');
        lpEnd++;
        *lpEnd =  TEXT('\0');
    }

    // Enumerate the keys

    dwSize = MAX_PATH;
    lResult = RegEnumKeyEx(hKey, 0, szName, &dwSize, NULL,
                           NULL, NULL, &ftWrite);

    if (lResult == ERROR_SUCCESS) 
    {
        do {

            StringCchCopy (lpEnd, MAX_PATH*2, szName);

            if (!RegDelnodeRecurse(hKeyRoot, lpSubKey)) {
                break;
            }

            dwSize = MAX_PATH;

            lResult = RegEnumKeyEx(hKey, 0, szName, &dwSize, NULL,
                                   NULL, NULL, &ftWrite);

        } while (lResult == ERROR_SUCCESS);
    }

    lpEnd--;
    *lpEnd = TEXT('\0');

    RegCloseKey (hKey);

    // Try again to delete the key.

    lResult = RegDeleteKey(hKeyRoot, lpSubKey);

    if (lResult == ERROR_SUCCESS) 
        return TRUE;

    return FALSE;
}

//*************************************************************
//
//  RegDelnode()
//
//  Purpose:    Deletes a registry key and all its subkeys / values.
//
//  Parameters: hKeyRoot    -   Root key
//              lpSubKey    -   SubKey to delete
//
//  Return:     TRUE if successful.
//              FALSE if an error occurs.
//
//*************************************************************

BOOL RegDelnode (HKEY hKeyRoot, LPTSTR lpSubKey)
{
    TCHAR szDelKey[MAX_PATH*2];

    StringCchCopy (szDelKey, MAX_PATH*2, lpSubKey);
    return RegDelnodeRecurse(hKeyRoot, szDelKey);

}