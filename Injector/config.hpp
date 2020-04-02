#include <Windows.h>
#include <shellapi.h>
#include <string>
#include <vector>
#include <functional>
#include <iostream>

struct InjectorConfig
{
    std::wstring processName;
    std::wstring dllName;
    bool hasAskedForHelp = false;
};

InjectorConfig ParseCommandLine(std::wstring commandLine)
{
    int argumentCount = 0;
    auto argument = CommandLineToArgvW(commandLine.c_str(),&argumentCount);

    InjectorConfig result;

    auto DLLNAME_HANDLER = [&](std::wstring opt)
    {
        result.dllName = opt;
    };

    auto PROCNAME_HANDLER = [&](std::wstring opt)
    {
        result.processName = opt;
    };

    std::vector<std::pair<std::vector<std::wstring>, std::function<void(std::wstring)>>> optionsWithArgument = 
    {
        {{L"-process",L"-proc",L"-processName", L"--p"},PROCNAME_HANDLER},
        {{L"-dll", L"-dllname", L"--d"},DLLNAME_HANDLER},
    };

    
    std::vector<std::pair<std::wstring, std::function<void()>>> singleOptions;
    auto PRINT_HELP = [&]()
    {
        result.hasAskedForHelp = true;
        std::cout << "Program usage:" << std::endl;
        for(const auto& optionEntry: optionsWithArgument)
        {
            const auto& names = optionEntry.first;
            for(auto& name:names)
            {
                std::wcout << name << " <ARG> | ";
            }
            std::cout << std::endl;
        }
        for(const auto& optionEntry: singleOptions)
        {
            const auto& name = optionEntry.first;
            
            std::wcout << name << std::endl;
        }
    };
    singleOptions =
    {
        {L"-help", PRINT_HELP}
    };

    std::wstring defaultArgument = L"UNDEFINED";
    for(int i = 1; i < argumentCount; i++)
    {
        auto command = argument[i];
        auto hasArgument = (i+1 < argumentCount);
        // Parse single options
        for (auto& optionEntry : singleOptions)
        {
            const auto& alias = optionEntry.first;
            if (command == alias)
            {
                optionEntry.second();
                break;
            }
        }
        // Parse options with arguments
        for(auto& optionEntry: optionsWithArgument)
        {
            const auto& aliases = optionEntry.first;
            for(auto& alias: aliases)
            {
                if(command == alias)
                {
                    optionEntry.second((hasArgument)?argument[++i]:defaultArgument);
                    break;
                }
            }
        }
    }
    return result;
}

InjectorConfig LoadFromIni()
{
    InjectorConfig cfg;
    
    wchar_t dllName[201];
    GetPrivateProfileStringW(L"injector",L"dllName",L"",dllName,200,L"injector.ini");
    cfg.dllName = dllName;
    wchar_t processName[201];
    GetPrivateProfileStringW(L"injector",L"processName",L"",processName,200,L"injector.ini");
    cfg.processName = processName;
    return cfg;
}
