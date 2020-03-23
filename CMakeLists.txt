project(camera)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_REQUIRED_FLAGS -std=c++17)
add_compile_options("/std:c++17")

add_subdirectory(vendors/utilslib)
set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)
add_subdirectory(vendors/glm)
set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)

set(JSON_BuildTests OFF CACHE BOOL "" FORCE)
set(JSON_MultipleHeaders ON CACHE BOOL "" FORCE)

add_subdirectory(vendors)
add_subdirectory(CameraModInjector)
add_subdirectory(HotLoader)
#add_subdirectory(CameraMod)
add_subdirectory(AngeloFramework)
add_subdirectory(Angelo)



