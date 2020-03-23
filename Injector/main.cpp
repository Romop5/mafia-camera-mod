#include <iostream>
#include <string>

#include "injecting_api.hpp"
#include "config.hpp"

using namespace std;

int WINAPI WinMain(HINSTANCE _hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	std::wcout << "[INJECTOR] Starting ..." << std::endl;
	auto cfg = ParseCommandLine(GetCommandLineW());
	if(cfg.hasAskedForHelp)
		return 0;
	if(cfg.dllName == L"" || cfg.processName == L"")
	{
		std::cout << "Stopping. No process name & dll name specified. Use --help" << std::endl;
	}
	
	DWORD pID = GetFirstProcessIDWithName(cfg.processName);

	// Get the dll's full path name 
	wchar_t fullPath[MAX_PATH] = { 0 };
	GetFullPathNameW(cfg.dllName.c_str(), MAX_PATH, fullPath, NULL);
	std::wcout << "Full path to dll: " << fullPath << std::endl;

	auto hasSucceeded = Inject(pID, fullPath);
	std::cout << "DLL loading has " << ((hasSucceeded)?L"succeeded":L"failed") << std::endl;

	system("pause");
	return 0;
}


