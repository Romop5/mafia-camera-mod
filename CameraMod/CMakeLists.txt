# Disable linking libci
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /NODEFAULTLIB:\"libci\" /SAFESEH:NO")
# Turn off these CRT warnings
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

add_library(CameraMod SHARED 
	CameraMod.cpp
	CGame.cpp
	CGraphics.cpp
	gui/CGUI.cpp
	gui/CGUIButton.cpp
	gui/CGUICheckbox.cpp
	gui/CGUIEditbox.cpp
	gui/CGUIElement.cpp
	gui/CGUIElementsPool.cpp
	gui/CGUIGroup.cpp
	gui/CGUILabel.cpp
	gui/CGUIScrollbox.cpp
	gui/CGUITableList.cpp
	gui/CGUIWindow.cpp
	CModControl.cpp
	CRawInput.cpp
	CHooks.cpp
	CD3D9Accessor.cpp
	CImGUIAdaptor.cpp
	dllmain.cpp

# Separate cd3dfont
	../vendors/cd3dfont/
	../vendors/cd3dfont/d3dfont.cpp
	../vendors/cd3dfont/d3dfont.h
	../vendors/cd3dfont/d3dutil.cpp
	../vendors/cd3dfont/d3dutil.h
	../vendors/cd3dfont/dxutil.cpp
	../vendors/cd3dfont/dxutil.h

	windowsHack.cpp

# RC for mouse.png
	camera/camera.rc
	list.rc

# imGui
	imgui_impl_dx9.cpp
	)

add_library(imgui 
	../vendors/imgui/imgui.cpp
	../vendors/imgui/imgui_demo.cpp
	../vendors/imgui/imgui_draw.cpp
	../vendors/imgui/imgui_widgets.cpp
	)

target_include_directories(CameraMod PRIVATE ../vendors/ ../vendors/DirectX9SDK2007/Include)
target_include_directories(CameraMod PRIVATE ../vendors/d3d8to9/source)
target_include_directories(CameraMod PRIVATE ../vendors/imgui)
target_link_libraries(CameraMod PRIVATE imgui)

target_link_libraries(CameraMod PRIVATE ${PROJECT_SOURCE_DIR}/vendors/DirectX9SDK2007/Lib/x86/d3dx8.lib)
target_link_libraries(CameraMod PRIVATE winmm)
target_link_libraries(CameraMod PRIVATE legacy_stdio_definitions.lib)




