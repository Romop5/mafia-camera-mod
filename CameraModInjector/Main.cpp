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

bool insertDll(DWORD procID, std::string dll);

HMODULE InjectDLL(DWORD ProcessID, char* dllName);

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

HMODULE InjectDLL(DWORD ProcessID, char* dllName)
{
	char buf[50] = { 0 };
	HMODULE result = LoadLibrary(dllname);
	if(result == NULL)
	{
		sprintf_s(buf, "Failed to load library: %d", GetLastError());
		MessageBoxA(NULL, buf, "Loader", NULL);
		return false;
	}
	HANDLE Proc;
	HANDLE Thread;
	LPVOID RemoteString, LoadLibAddy;
	HMODULE hModule = NULL;
	DWORD dwOut;

	if (!ProcessID)
		return false;
	Proc = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, 0, ProcessID);
	//Proc = OpenProcess(PROCESS_ALL_ACCESS, 0, ProcessID);

	if (!Proc)
	{
		sprintf_s(buf, "OpenProcess() failed: %d", GetLastError());
		MessageBoxA(NULL, buf, "Loader", NULL);
		return false;
	}

	LoadLibAddy = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
	if (!LoadLibAddy) {
		sprintf_s(buf, "LoadLibrary GetModule failed");
		MessageBoxA(NULL, buf, "Loader", NULL);
		return false;
	}


	RemoteString = (LPVOID)VirtualAllocEx(Proc, NULL, strlen(dllName), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (!RemoteString) {
		sprintf_s(buf, "VirtualAllocEx failed");
		MessageBoxA(NULL, buf, "Loader", NULL);
		return false;
	}

	if (!WriteProcessMemory(Proc, (LPVOID)RemoteString, dllName, strlen(dllName), NULL)) {
		sprintf_s(buf, "WriteProcessMemory failed");
		MessageBoxA(NULL, buf, "Loader", NULL);
		return false;
	}

	Thread = CreateRemoteThread(Proc, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibAddy, (LPVOID)RemoteString, NULL, NULL);
	if (!Thread) {
		sprintf_s(buf, "CreateRemoteThread failed: %d", GetLastError());
		MessageBoxA(NULL, buf, "Loader", NULL);
		return false;
	}
	else {
		while (GetExitCodeThread(Thread, &dwOut)) {
			if (dwOut != STILL_ACTIVE) {
				hModule = (HMODULE)dwOut;
				break;
			}
		}
	}

	CloseHandle(Thread);
	CloseHandle(Proc);

	return hModule;
}

#define MAXWAIT 10000

bool insertDll(DWORD procID, std::string dll)
{
	//Find the address of the LoadLibrary api, luckily for us, it is loaded in the same address for every process
	HMODULE hLocKernel32 = GetModuleHandle("Kernel32");
	FARPROC hLocLoadLibrary = GetProcAddress(hLocKernel32, "LoadLibraryA");

	//Adjust token privileges to open system processes
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tkp.Privileges[0].Luid);
		tkp.PrivilegeCount = 1;
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		AdjustTokenPrivileges(hToken, 0, &tkp, sizeof(tkp), NULL, NULL);
	}

	//Open the process with all access
	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID);

	//Allocate memory to hold the path to the Dll File in the process's memory
	dll += '\0';
	LPVOID hRemoteMem = VirtualAllocEx(hProc, NULL, dll.size(), MEM_COMMIT, PAGE_READWRITE);

	//Write the path to the Dll File in the location just created
	DWORD numBytesWritten;
	WriteProcessMemory(hProc, hRemoteMem, dll.c_str(), dll.size(), &numBytesWritten);

	//Create a remote thread that starts begins at the LoadLibrary function and is passed are memory pointer
	HANDLE hRemoteThread = CreateRemoteThread(hProc, NULL, 0, (LPTHREAD_START_ROUTINE)hLocLoadLibrary, hRemoteMem, 0, NULL);

	cout << hRemoteThread << endl;

	//Wait for the thread to finish
	bool res = false;
	if (hRemoteThread)
		res = (bool)WaitForSingleObject(hRemoteThread, MAXWAIT) != WAIT_TIMEOUT;

	//Free the memory created on the other process
	VirtualFreeEx(hProc, hRemoteMem, dll.size(), MEM_RELEASE);

	//Release the handle to the other process
	CloseHandle(hProc);

	return res;
}

BOOL Inject(DWORD pID, const char * DLL_NAME)
{
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
