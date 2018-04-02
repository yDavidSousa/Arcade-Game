cmake_minimum_required(VERSION 3.9)
project(First_Code C)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/content)
include_directories(${PROJECT_SOURCE_DIR}/source)

set(SOURCE_FILES main.c)

configure_file(content/image.png content/image.png COPYONLY)
configure_file(content/oldHero.png content/oldHero.png COPYONLY)

add_executable(First_Code ${SOURCE_FILES})
target_link_libraries(First_Code ${SDL2_LIBRARY} ${SDL2_TTF_LIBRARY} ${SDL2_IMAGE_LIBRARY})