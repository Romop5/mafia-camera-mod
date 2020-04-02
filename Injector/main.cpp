#include <iostream>
#include <string>

#include "injecting_api.hpp"
#include "config.hpp"
#include "console.hpp"

using namespace std;


int WINAPI WinMain(HINSTANCE _hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	CreateConsole();

	std::wcout << "[INJECTOR] Starting ..." << std::endl;
	auto cfg = ParseCommandLine(GetCommandLineW());
	if(cfg.hasAskedForHelp)
	{
		Sleep(2000);
		return 0;
	}
	if(cfg.dllName == L"" || cfg.processName == L"")
	{
		std::cout << "Stopping. No process name & dll name specified. Use --help" << std::endl;
		
		Sleep(2000);
		return 0;
	}
	
	DWORD pID = GetFirstProcessIDWithName(cfg.processName);

	// Get the dll's full path name 
	wchar_t fullPath[MAX_PATH] = { 0 };

	GetFullPathNameW(cfg.dllName.c_str(), MAX_PATH, fullPath, NULL);
	std::wcout << "Full path to dll: " << fullPath << std::endl;

	auto hasSucceeded = Inject(pID, fullPath);
	std::wstring statusString = ((hasSucceeded) ? L"succeeded" : L"failed");
	std::wcout << "DLL loading has " << statusString << std::endl;

	Sleep(2000);
	return 0;
}


