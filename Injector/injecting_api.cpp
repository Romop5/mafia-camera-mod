#include <windows.h> 
#include <tlhelp32.h> 
#include <shlwapi.h> 
#include <conio.h> 
#include <stdio.h> 
#include <iostream>
#include <fstream>

#include "injecting_api.hpp"


#pragma comment (lib, "Shlwapi.lib")

using namespace std;

bool CheckFileExist(char filename[255])
{
	fstream file;
	file.open(filename, ios::in);
	if (file)
		return true;
	return false;
	file.close();

}

bool IsGameRunning()
{
	HWND hWnd = false;
	HWND hWnd2 = false;
	hWnd = FindWindowA(NULL, "Mafia");
	hWnd2 = FindWindowA(NULL, "Mafia ..zzZZ");
	if (hWnd2 != NULL)
	{
		return true;
	}
	else if (hWnd != NULL)
	{
		return true;
	}
	else
	{
		return false;
	}
}


HMODULE GetHandleToModuleInProcess(DWORD processID,std::wstring moduleName)
{
    HMODULE resultHandle = NULL;

    auto moduleSnapshop = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, processID);
    MODULEENTRY32W entry;
    bool status = Module32FirstW(moduleSnapshop, &entry);
    while(status != false)
    {
        std::wstring entryName = entry.szModule;
        if(entryName == moduleName)
        {
            resultHandle = entry.hModule;
            break;
        }
        status = Module32NextW(moduleSnapshop, &entry);
    }
    CloseHandle(moduleSnapshop);
    return resultHandle;
}

bool TryReloadingDLL(DWORD processID, std::wstring dllName)
{
    auto dllModule = LoadLibraryW(dllName.c_str());
    if(dllModule == NULL)
    {
        std::cout << "Error: " << GetLastError() << std::endl;
        return false;
    }
    using reloadFunction_t = void (*)(void);
    LPVOID reloadFunction = (LPVOID) (GetProcAddress(dllModule, "reload"));
    if(reloadFunction)
    {
        auto hasStartedReloadRoutine = false;
        auto processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
        if (processHandle)
        {
            auto threadID = CreateRemoteThread(processHandle, NULL, NULL, (LPTHREAD_START_ROUTINE)reloadFunction, NULL, NULL, NULL);
            hasStartedReloadRoutine = (threadID != NULL);
        }
        CloseHandle(processHandle);
        return hasStartedReloadRoutine;
    }
    return false;
}

BOOL Inject(DWORD processID, std::wstring dllName)
{
    if (!processID)
    {
        std::cout << "Invalid process ID detected" << std::endl;
        return false;
    }

    // I. At first, detect if .DLL is already loaded
    auto dllModule = GetHandleToModuleInProcess(processID, dllName);   
    if(dllModule != NULL)
    {
        std::cout << "DLL is already there, trying to reload() - HMODULE: " << dllModule << std::endl;
        auto reloadingResult = TryReloadingDLL(processID,dllName);
        std::cout << "Reloading has " << ((reloadingResult)?"succeed":"failed") << "." << std::endl;
    } 
    // II. if .dll is not already loaded, then load & call it's entry function
    std::cout << "No DLL found in process => injecting ..." << std::endl;
  
	auto processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
	if (!processHandle)
	{
        std::cout << "Failed to OpenProcess() while injecting ..." << std::endl;
		return false;
	}
    /*
        In order to load a library in a remote process, a new thread must be started using CreateRemoteThread.
        This thread will call LoadLibrary() function. However, this function must have the parameter prepared,
        and therefore we need to allocate & write string to process' memory prior to starting the thread.
    */
	auto LoadLibraryFunctionAddress = (LPVOID)GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "LoadLibraryA");

	// Allocate space in the process for our DLL 
	auto RemoteString = (LPVOID)VirtualAllocEx(processHandle, NULL, dllName.size()+1, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	// Write the string name of our DLL in the memory allocated 
	WriteProcessMemory(processHandle, (LPVOID)RemoteString, dllName.c_str(), dllName.size(), NULL);

	// Load our DLL 
	CreateRemoteThread(processHandle, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibraryFunctionAddress, (LPVOID)RemoteString, NULL, NULL);

	CloseHandle(processHandle);
	return true;
}

DWORD GetFirstProcessIDWithName(const std::wstring processName)
{
	auto snapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapshotHandle == INVALID_HANDLE_VALUE)
	{
		printf("Error: Unable <strong class='highlight'>to</strong> create toolhelp snapshot!");
		return 0;
	}

	PROCESSENTRY32W processEntry;
	processEntry.dwSize = sizeof(PROCESSENTRY32W);

    DWORD resultID = 0;

	auto hasNextProcessEntry = Process32FirstW(snapshotHandle, &processEntry);
	while (hasNextProcessEntry)
	{
		if (StrStrIW(processEntry.szExeFile, processName.c_str()))
		{
			resultID = processEntry.th32ProcessID;
		}
		hasNextProcessEntry = Process32NextW(snapshotHandle, &processEntry);
	}
    CloseHandle(snapshotHandle);
	return resultID;
}