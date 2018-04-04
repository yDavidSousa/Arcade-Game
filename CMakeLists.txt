cmake_minimum_required(VERSION 3.9)
project(Arcade-Game C)

add_definitions(-DDEBUG)

set(plataform "x32")
#set(plataform "x64")

### DEPS
set(deps_dir "${CMAKE_SOURCE_DIR}/deps")
set(deps_include_dir "${CMAKE_SOURCE_DIR}/deps/include")
set(deps_bin_dir "${CMAKE_SOURCE_DIR}/deps/bin/${plataform}")
set(deps_lib_dir "${CMAKE_SOURCE_DIR}/deps/lib/${plataform}")

include_directories("include;${deps_include_dir}")

#-lSDL2_mixer
set(libs "-lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf")
set(SOURCE_FILES include/status.h source/status.c main.c)
add_executable(Arcade-Game ${SOURCE_FILES})
target_link_libraries(Arcade-Game ${libs})

file(COPY ${deps_bin_dir}/ DESTINATION .)
file(COPY content DESTINATION .)