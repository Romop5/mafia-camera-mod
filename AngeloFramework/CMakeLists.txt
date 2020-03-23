cmake_minimum_required(VERSION 3.11)
project(AngeloFramework)

add_library(AngeloFramework SHARED 
    src/angeloframework/angelo.hpp
    src/angeloframework/game/CGame.cpp
    src/angeloframework/game/CGame.h
    src/angeloframework/game/CScript.cpp
    src/angeloframework/game/CScript.h
    src/angeloframework/game/MafiaAddresses.txt
    src/angeloframework/imgui/CImGUIAdaptor.cpp
    src/angeloframework/imgui/CImGUIAdaptor.hpp
    src/angeloframework/imgui/imgui_impl_dx9.cpp
    src/angeloframework/imgui/imgui_impl_dx9.h
    src/angeloframework/internal/CD3D9Accessor.cpp
    src/angeloframework/internal/CD3D9Accessor.h
    src/angeloframework/internal/CDirect3DDevice8Proxy.h
    src/angeloframework/internal/CGraphics.cpp
    src/angeloframework/internal/CGraphics.h
    src/angeloframework/internal/CHooks.cpp
    src/angeloframework/internal/CHooks.h
    src/angeloframework/internal/CRawInput.cpp
    src/angeloframework/internal/CRawInput.h
    src/angeloframework/utils/tool.cpp
    src/angeloframework/utils/tools.h


)
target_link_libraries(AngeloFramework PUBLIC d3d9sdk)
target_link_libraries(AngeloFramework PRIVATE nlohmann_json)
target_link_libraries(AngeloFramework PRIVATE imgui imguieditor)
target_link_libraries(AngeloFramework PRIVATE glm)
target_link_libraries(AngeloFramework PRIVATE utilslib)


target_link_libraries(AngeloFramework PRIVATE winmm)
target_link_libraries(AngeloFramework PRIVATE legacy_stdio_definitions.lib)


target_link_libraries(AngeloFramework PUBLIC cd3dfont)
target_include_directories(AngeloFramework PUBLIC src/)