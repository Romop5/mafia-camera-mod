# Turn off these CRT warnings
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

add_library(HotReloader SHARED 
	dllmain.cpp)
