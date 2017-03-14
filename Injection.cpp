#include <Windows.h>

BOOL InjectLibrary(HANDLE hProcess, LPCWSTR lpszDllFullpath)
{
	const DWORD dwMaxWaitTime = 5000;
	SIZE_T dwSize;
	LPVOID lpStartAddress = NULL;
	LPVOID lpParameter = NULL;
	HANDLE hThread = NULL;
	DWORD dwResult;
	BOOL result;

	dwSize = strlen((const char*)lpszDllFullpath) * sizeof(WCHAR);
	lpStartAddress = LoadLibraryW;
	lpParameter = VirtualAllocEx(hProcess, NULL, dwSize, MEM_COMMIT, PAGE_READWRITE);
	if(!lpParameter)
	{
		result = FALSE;
		goto exit;
	}

	if(!WriteProcessMemory(hProcess, lpParameter, lpszDllFullpath, dwSize, NULL))
	{
		result = FALSE;
		goto exit;
	}

	hThread = CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)lpStartAddress, lpParameter, NULL, NULL);    
	if(!hThread)
	{
		result = FALSE;
		goto exit;
	}

	dwResult = WaitForSingleObject(hThread, dwMaxWaitTime);
	switch(dwResult)
	{
	case WAIT_OBJECT_0:
		{
			// this might just have worked, pick up the result!
			result = TRUE;
		}
		break;
	case WAIT_TIMEOUT:
	case WAIT_FAILED:
	default:
		{
			result = FALSE;
			goto exit;
		}
	}

exit:
	if(lpParameter)
		VirtualFreeEx(hProcess, lpParameter, 0, MEM_RELEASE);
	if(hThread)
		CloseHandle(hThread);

	return result;
}

BOOL InjectLibrary(DWORD dwProcID, LPCWSTR lpszDllFullpath)
{
	BOOL result;
	HANDLE hProcess = NULL;

	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcID);
	if(!hProcess)
	{
		result = FALSE;
		goto exit;
	}

	result = InjectLibrary(hProcess, lpszDllFullpath);

exit:
	if(hProcess)
		CloseHandle(hProcess);
	return result;
}