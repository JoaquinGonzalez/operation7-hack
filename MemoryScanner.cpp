#include "MemoryScanner.h"
#include <iostream>

HANDLE threadScan;
bool runThread = true;

void StartMemoryScan()
{
	threadScan = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ScanMemmory, 0, 0, 0);
}

void ScanMemmory()
{
 
    SYSTEM_INFO sysInfo = { 0 };
    GetSystemInfo(&sysInfo);
 
    auto aStart = (long)sysInfo.lpMinimumApplicationAddress;
    auto aEnd = (long)sysInfo.lpMaximumApplicationAddress;
 
    int found = 0;
 
    do{
 
        while (aStart < aEnd){
            MEMORY_BASIC_INFORMATION mbi = { 0 };
			if (!VirtualQueryEx(GetCurrentProcess(), (LPCVOID)aStart, &mbi, sizeof(mbi))){
                 
                CloseHandle(GetCurrentProcess());
                TerminateThread(threadScan, 1);          
            }
 
            if (mbi.State == MEM_COMMIT && ((mbi.Protect & PAGE_GUARD) == 0) && ((mbi.Protect == PAGE_NOACCESS) == 0)){
 
                auto isWritable = ((mbi.Protect & PAGE_READWRITE) != 0 || (mbi.Protect & PAGE_WRITECOPY) != 0 || (mbi.Protect & PAGE_EXECUTE_READWRITE) != 0 || (mbi.Protect & PAGE_EXECUTE_WRITECOPY) != 0);
                if (isWritable){
 
                    auto dump = new unsigned char[mbi.RegionSize + 1];
                    memset(dump, 0x00, mbi.RegionSize + 1);
					ReadProcessMemory(GetCurrentProcess(), mbi.BaseAddress, dump, mbi.RegionSize, NULL);             
                    for (auto x = 0; x < mbi.RegionSize - 4; x += 4){
 
                        /* FREE */
                         
                        if (free){
                            if (*(DWORD*)(dump + x) == 45000){
                                found++;
                                 
                                BYTE data[] = { 0x0, 0x0, 0x0, 0x0 }; //0
                                DWORD wAddr = (DWORD)mbi.BaseAddress + x;
                                 
                                WriteProcessMemory(GetCurrentProcess(), (BYTE*)wAddr, &data, sizeof(data), NULL);
                                 
                            }
                        }                                          
 
                    }
					std::cout << "DUMP: " << dump << std::endl;
                    delete[] dump;
                }
 
            }
            aStart += mbi.RegionSize;
        }
        runThread = false;
 
    } while (runThread);
 
    if (!runThread){     
		CloseHandle(GetCurrentProcess());        
        TerminateThread(threadScan, 0);      
    }  
}