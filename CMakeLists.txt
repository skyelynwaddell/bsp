cmake_minimum_required(VERSION 3.10)
project(game)

set(OPENGL_VERSION "4.3")
set(CMAKE_CXX_STANDARD 23)

add_subdirectory(lib/raylib)

add_executable(game main.cpp)

target_link_options(game PRIVATE -static-libgcc -static-libstdc++ -static)

target_link_libraries(game raylib winmm ws2_32)

target_compile_definitions(game PRIVATE
	VS_PATH="${CMAKE_SOURCE_DIR}/shaders/lighting.vert"
	FS_PATH="${CMAKE_SOURCE_DIR}/shaders/lighting.frag")