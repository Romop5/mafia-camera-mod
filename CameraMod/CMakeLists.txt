add_library(imgui STATIC
	../vendors/imgui/imgui.cpp
	../vendors/imgui/imgui_demo.cpp
	../vendors/imgui/imgui_draw.cpp
	../vendors/imgui/imgui_widgets.cpp
	)


# Disable linking libci
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /NODEFAULTLIB:\"libci\" /SAFESEH:NO")
# Turn off these CRT warnings
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

add_library(CameraMod SHARED 
	CCore.cpp
	CScene.cpp
	game/CGame.cpp
	game/CGraphics.cpp
	game/CRawInput.cpp
	game/CHooks.cpp
	game/CD3D9Accessor.cpp
	game/CImGUIAdaptor.cpp
	modes/CFreecamera.cpp
	modes/CModeManager.cpp
	dllmain.cpp

# Separate cd3dfont
	../vendors/cd3dfont/
	../vendors/cd3dfont/d3dfont.cpp
	../vendors/cd3dfont/d3dfont.h
	../vendors/cd3dfont/d3dutil.cpp
	../vendors/cd3dfont/d3dutil.h
	../vendors/cd3dfont/dxutil.cpp
	../vendors/cd3dfont/dxutil.h

	# imGui
	game/imgui_impl_dx9.cpp
	)
	
target_include_directories(CameraMod PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories(CameraMod PRIVATE ../vendors/ ../vendors/DirectX9SDK2007/Include)
target_include_directories(CameraMod PRIVATE ../vendors/d3d8to9/source)
target_include_directories(CameraMod PRIVATE ../vendors/imgui)
target_link_libraries(CameraMod PRIVATE imgui)
target_link_libraries(CameraMod PRIVATE glm)
target_link_libraries(CameraMod PRIVATE utilslib)

target_link_libraries(CameraMod PRIVATE ${PROJECT_SOURCE_DIR}/vendors/DirectX9SDK2007/Lib/x86/d3dx8.lib)
target_link_libraries(CameraMod PRIVATE winmm)
target_link_libraries(CameraMod PRIVATE legacy_stdio_definitions.lib)




