set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /NODEFAULTLIB:\"libci\"")
add_library(CameraMod SHARED 
	CameraMod.cpp
	CGame.cpp
	CGraphics.cpp
	CGUI.cpp
	CGUIButton.cpp
	CGUICheckbox.cpp
	CGUIEditbox.cpp
	CGUIElement.cpp
	CGUIElementsPool.cpp
	CGUIGroup.cpp
	CGUILabel.cpp
	CGUIScrollbox.cpp
	CGUITableList.cpp
	CGUIWindow.cpp
	CModControl.cpp
	CRawInput.cpp
	CHooks.cpp
	dllmain.cpp

	../vendors/cd3dfont/
	../vendors/cd3dfont/d3dfont.cpp
	../vendors/cd3dfont/d3dfont.h
	../vendors/cd3dfont/d3dutil.cpp
	../vendors/cd3dfont/d3dutil.h
	../vendors/cd3dfont/dxutil.cpp
	../vendors/cd3dfont/dxutil.h

	windowsHack.cpp
	)


target_include_directories(CameraMod PRIVATE ../vendors/ ../vendors/DirectX9SDK2007/Include)
target_link_libraries(CameraMod PRIVATE ${PROJECT_SOURCE_DIR}/vendors/DirectX9SDK2007/Lib/x86/d3dx8.lib)
target_link_libraries(CameraMod PRIVATE winmm)
target_link_libraries(CameraMod PRIVATE legacy_stdio_definitions.lib)




