#define WIN32_LEAN_AND_MEAN 
#include <Windows.h>
#include <string>

#define CREATE_THREAD_ACCESS (PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ) 
#define MAXWAIT 10000

BOOL Inject(DWORD processID, std::wstring dllName);

bool CheckFileExist(char filename[255]);

bool IsGameRunning();

HMODULE GetHandleToModuleInProcess(DWORD processID,std::wstring moduleName);

bool TryReloadingDLL(DWORD processID,std::wstring dllName);

DWORD GetFirstProcessIDWithName(const std::wstring procName);