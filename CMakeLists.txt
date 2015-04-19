cmake_minimum_required (VERSION 2.8)

project (Diaballik)

set (EXECUTABLE_OUTPUT_PATH "${Diaballik_BINARY_DIR}/bin")

find_package (Qt4 REQUIRED)
include (${QT_USE_FILE})
add_definitions (${QT_DEFINITIONS})

add_subdirectory (src)
