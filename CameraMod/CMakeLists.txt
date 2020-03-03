add_library(imgui STATIC
	../vendors/imgui/imgui.cpp
	../vendors/imgui/imgui_demo.cpp
	../vendors/imgui/imgui_draw.cpp
	../vendors/imgui/imgui_widgets.cpp
	)

target_include_directories(imgui PUBLIC ../vendors/imgui)
	
add_library(imguieditor STATIC ../vendors/ImGuiColorTextEdit/TextEditor.cpp)
target_link_libraries(imguieditor PUBLIC imgui)
target_include_directories(imguieditor PUBLIC ../vendors/ImGuiColorTextEdit)


# Disable linking libci
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /NODEFAULTLIB:\"libci\" /SAFESEH:NO")
# Turn off these CRT warnings
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

#set(FOO "Some text to show up in json_file.json")
#set(BAR "More text")
configure_file(version.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/version.cmake @ONLY)

  
# Dummy output which is never actually produced. Anything that depends on
# this will always be rebuilt.
add_custom_command(
    OUTPUT always_rebuild
    COMMAND cmake -E echo
	)
	
add_custom_command(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/version.cpp
  COMMAND ${CMAKE_COMMAND} -P
	${CMAKE_CURRENT_BINARY_DIR}/version.cmake
  DEPENDS
  	always_rebuild
  COMMENT "Setting up C++ unit with version information"
  VERBATIM)

add_custom_target(configure_version ALL
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/version.cmake)


add_library(CameraMod SHARED 
	CCore.cpp
	CScene.cpp
	game/CScript.cpp
	game/CGame.cpp
	game/CGraphics.cpp
	game/CRawInput.cpp
	game/CHooks.cpp
	game/CD3D9Accessor.cpp
	game/CImGUIAdaptor.cpp
	modes/CAbout.cpp
	modes/CFreecamera.cpp
	modes/CModeManager.cpp
	modes/sandbox/CScriptInspector.cpp
	${CMAKE_CURRENT_BINARY_DIR}/version.cpp
	dllmain.cpp
	json_impl.cpp

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
add_dependencies(CameraMod configure_version)
target_include_directories(CameraMod PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories(CameraMod PRIVATE ../vendors/ ../vendors/DirectX9SDK2007/Include)
target_include_directories(CameraMod PRIVATE ../vendors/d3d8to9/source)
target_link_libraries(CameraMod PRIVATE nlohmann_json)
target_link_libraries(CameraMod PRIVATE imgui)
target_link_libraries(CameraMod PRIVATE glm)
target_link_libraries(CameraMod PRIVATE utilslib)
target_link_libraries(CameraMod PRIVATE imguieditor)


target_link_libraries(CameraMod PRIVATE ${PROJECT_SOURCE_DIR}/vendors/DirectX9SDK2007/Lib/x86/d3dx8.lib)
target_link_libraries(CameraMod PRIVATE winmm)
target_link_libraries(CameraMod PRIVATE legacy_stdio_definitions.lib)




