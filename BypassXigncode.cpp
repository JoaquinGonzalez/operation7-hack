#include "BypassXigncode.h"


void RemoveXignRegister()
{
	if(RegDelnode(HKEY_LOCAL_MACHINE, L"SYSTEM\\ControlSet001\\Services\\xhunter1"))
		OutputDebugString(L"xhunter1 Removed Success");
	else
		OutputDebugString(L"xhunter1 Removed Failed");
}

BOOL StopXignInit() {
	return WriteBytes(0x006C0521, (PBYTE)"\x90\x90\x90\x90\x90", 5);
}

void BypassInstance()
{
	if(WriteBytes(0x006C04D9, (PBYTE)"\0x73\0x29", 2))
		OutputDebugString(L"Multi-Account Actived");
	else
		OutputDebugString(L"Multi-Account Failed");
}

void BypassXigncode()
{
	if (StopXignInit()) {
		OutputDebugString(L"Successfully stopped XIGNCODE3 from initializing!");
	}
	else {
		OutputDebugString(L"Failed to stop XIGNCODE3 from initializing!");
	}
}

BOOL WriteBytes(DWORD dwAddress, LPBYTE lpBuffer, DWORD nBytes) {
	if (nBytes == 0)
		return FALSE;

	DWORD dwOldProtect;

	if (!VirtualProtect((LPVOID)dwAddress, nBytes, PAGE_EXECUTE_READWRITE, &dwOldProtect))
		return FALSE;

	memcpy((LPVOID)dwAddress, lpBuffer, nBytes);

	if (!VirtualProtect((LPVOID)dwAddress, nBytes, dwOldProtect, &dwOldProtect))
		return FALSE;

	return TRUE;
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