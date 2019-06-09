project(camera)
add_subdirectory(vendors/utilslib)
set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)
add_subdirectory(vendors/glm)
set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)

add_subdirectory(CameraModInjector)
add_subdirectory(HotLoader)
add_subdirectory(CameraMod)



