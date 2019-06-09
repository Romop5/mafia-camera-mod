#include <windows.h> 
#include <tlhelp32.h> 
#include <shlwapi.h> 
#include <conio.h> 
#include <stdio.h> 
#include <iostream>
#include <fstream>

using namespace std;


#pragma comment (lib, "Shlwapi.lib")

#define WIN32_LEAN_AND_MEAN 
#define CREATE_THREAD_ACCESS (PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ) 

BOOL Inject(DWORD pID, const char * DLL_NAME);
DWORD GetTargetThreadIDFromProcName(const char * ProcName);
HINSTANCE g_hIstance = NULL;
bool CheckFileExist(char[255]);
bool IsGameRunning();
char path[MAX_PATH];
char exename[MAX_PATH];
char dllname[MAX_PATH];

#define exe_name "game.exe"
//#define dll_name "CameraMod.dll"
#define dll_name "HotReloader.dll"


int WINAPI WinMain(HINSTANCE _hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	DWORD pID = GetTargetThreadIDFromProcName("Game.exe");


	// Get the dll's full path name 
	char buf[MAX_PATH] = { 0 };
	GetFullPathName(dll_name, MAX_PATH, buf, NULL);
	printf(buf);
	printf("\n");

	// Inject our main dll 
	if (!Inject(pID, buf))
	{

		printf("DLL Not Loaded!");
	}
	else{
		printf("DLL Loaded!");
	}

	_getch();
	return 0;
}


bool CheckFileExist(char filename[255])
{
	using namespace std;
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
#define MAXWAIT 10000
BOOL Inject(DWORD pID, const char * DLL_NAME)
{
        // TODO: refactor this mess
        HMODULE dllModule = NULL;
        // At first, detect if .DLL is already loaded
        auto moduleSnapshop = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, pID);
        MODULEENTRY32 entry;
        bool status = Module32First(moduleSnapshop, &entry);
        while(status != false)
        {
            if(strcmp(entry.szModule, dll_name) == 0)
            {
                dllModule = entry.hModule;
                std::cout << "Found it: " << entry.szModule << std::endl;
            }
            status = Module32Next(moduleSnapshop, &entry);
        }
        CloseHandle(moduleSnapshop);
        

        //if(GetModuleHandle(dll_name) != NULL)
        if(dllModule)
        {
            SetErrorMode(0);
            // It seems like DLL must be reloaded in order to get address using GetProcAddress
            dllModule = LoadLibrary(dll_name);
            if(dllModule == NULL)
            {
                std::cout << "Error: " << GetLastError() << std::endl;
                return false;
            }
            std::cout << "DLL is already there, trying to reload() - HMODULE: " << dllModule << std::endl;
            // if .dll is already inject, trigger reload
            using reloadFunction_t = void (*)(void);
            LPVOID reloadFunction = (LPVOID) (GetProcAddress(dllModule, TEXT("reload")));
            if(reloadFunction)
            {

                auto Proc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);
                if (Proc)
                {
                    CreateRemoteThread(Proc, NULL, NULL, (LPTHREAD_START_ROUTINE)reloadFunction, NULL, NULL, NULL);
                    std::cout << "reload() called" << std::endl;
                }
                CloseHandle(Proc);
            }
            else 
            {
                std::cout << "Last: err " << GetLastError() << std::endl;
            }
            return false;
        } else {
            std::cout << "No DLL found in process, injecting ..." << std::endl;
        }
	
	HANDLE Proc;
	HMODULE hLib;
	char buf[50] = { 0 };
	LPVOID RemoteString, LoadLibAddy;

	if (!pID)
		return false;

	Proc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);
	if (!Proc)
	{
		sprintf(buf, "OpenProcess() failed: %d", GetLastError());
		//MessageBox(NULL, buf, "Loader", MB_OK); 
		printf(buf);
		return false;
	}

	LoadLibAddy = (LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");

	// Allocate space in the process for our DLL 
	RemoteString = (LPVOID)VirtualAllocEx(Proc, NULL, strlen(DLL_NAME), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	// Write the string name of our DLL in the memory allocated 
	WriteProcessMemory(Proc, (LPVOID)RemoteString, DLL_NAME, strlen(DLL_NAME), NULL);

	// Load our DLL 
	CreateRemoteThread(Proc, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibAddy, (LPVOID)RemoteString, NULL, NULL);

	CloseHandle(Proc);
	return true;
}

DWORD GetTargetThreadIDFromProcName(const char * ProcName)
{
	PROCESSENTRY32 pe;
	HANDLE thSnapShot;
	BOOL retval, ProcFound = false;

	thSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (thSnapShot == INVALID_HANDLE_VALUE)
	{
		//MessageBox(NULL, "Error: Unable <strong class="highlight">to</strong> create toolhelp snapshot!", "2MLoader", MB_OK); 
		printf("Error: Unable <strong class='highlight'>to</strong> create toolhelp snapshot!");
		return false;
	}

	pe.dwSize = sizeof(PROCESSENTRY32);

	retval = Process32First(thSnapShot, &pe);
	while (retval)
	{
		if (StrStrI(pe.szExeFile, ProcName))
		{
			return pe.th32ProcessID;
		}
		retval = Process32Next(thSnapShot, &pe);
	}
	return 0;
}
