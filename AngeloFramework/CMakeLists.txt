cmake_minimum_required(VERSION 3.11)
project(AngeloFramework)

add_library(AngeloFramework STATIC 
    src/angeloframework/angelo.cpp
    src/angeloframework/angelo.hpp
    src/angeloframework/core/CD3D9Accessor.cpp
    src/angeloframework/core/CD3D9Accessor.h
    src/angeloframework/core/manager.hpp
    src/angeloframework/core/CRawInput.cpp
    src/angeloframework/core/CRawInput.h
    src/angeloframework/core/direct8hookdevice.hpp
    src/angeloframework/core/imgui/CImGUIAdaptor.cpp
    src/angeloframework/core/imgui/CImGUIAdaptor.hpp
    src/angeloframework/core/imgui/imgui_impl_dx9.cpp
    src/angeloframework/core/imgui/imgui_impl_dx9.h
    src/angeloframework/core/utils.cpp
    src/angeloframework/core/utils.hpp
    src/angeloframework/game/offsets.hpp
    src/angeloframework/game/utils.hpp
    src/angeloframework/utils/game_hooks.hpp
    src/angeloframework/utils/helpers.cpp
    src/angeloframework/utils/helpers.hpp
    src/angeloframework/utils/IDirect3DDevice8Proxy.h
    src/angeloframework/utils/memory.cpp
    src/angeloframework/utils/memory.hpp
)
# Include "src" to allow "angeloframework/..." includes
target_include_directories(AngeloFramework PUBLIC "src")

target_link_libraries(AngeloFramework PUBLIC d3d9sdk d3d8sdk)
target_link_libraries(AngeloFramework PUBLIC utilslib)

target_link_libraries(AngeloFramework PRIVATE nlohmann_json)
target_link_libraries(AngeloFramework PRIVATE imgui imguieditor)
target_link_libraries(AngeloFramework PRIVATE glm)


target_link_libraries(AngeloFramework PRIVATE winmm)
target_link_libraries(AngeloFramework PRIVATE legacy_stdio_definitions.lib)


target_link_libraries(AngeloFramework PUBLIC cd3dfont)
target_include_directories(AngeloFramework PUBLIC src/)
