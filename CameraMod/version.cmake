set(SRC_DIR "@CMAKE_CURRENT_SOURCE_DIR@")
set(BIN_DIR "@CMAKE_CURRENT_BINARY_DIR@")
set(FOO "@FOO@")
set(BAR "@BAR@")
string(TIMESTAMP MAFIA_BUILD_TIME "%d/%m/%Y %H:%M")
message("Configuring version.cpp :)")
configure_file(${SRC_DIR}/version.cpp.in
  ${BIN_DIR}/version.cpp @ONLY)
