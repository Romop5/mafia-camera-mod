add_subdirectory(json)

#################################################################
# D3D8
#################################################################
add_library(d3d8sdk INTERFACE)
target_link_libraries(d3d8sdk INTERFACE DirectX9SDK2007/Lib/x86/d3dx8.lib)

#################################################################
# D3D9
#################################################################
add_library(d3d9sdk INTERFACE)
target_include_directories(d3d9sdk INTERFACE DirectX9SDK2007/Include)
target_include_directories(d3d9sdk INTERFACE d3d8to9/source)

#################################################################
# D3D8 Font rendering
#################################################################
add_library(cd3dfont SHARED
    cd3dfont/
    cd3dfont/d3dfont.cpp
    cd3dfont/d3dfont.h
    cd3dfont/d3dutil.cpp
    cd3dfont/d3dutil.h
    cd3dfont/dxutil.cpp
    cd3dfont/dxutil.h
)
target_link_libraries(cd3dfont PRIVATE d3d8sdk d3d9sdk)

#################################################################
# ImGUI
#################################################################
add_library(imgui STATIC
	imgui/imgui.cpp
	imgui/imgui_demo.cpp
	imgui/imgui_draw.cpp
	imgui/imgui_widgets.cpp
)
target_include_directories(imgui PUBLIC imgui)
    
#################################################################
# ImGUI Extensions, providing multilang editor with formatting
#################################################################
add_library(imguieditor STATIC 
    ImGuiColorTextEdit/TextEditor.cpp)
target_link_libraries(imguieditor PUBLIC imgui)
target_include_directories(imguieditor PUBLIC ImGuiColorTextEdit)