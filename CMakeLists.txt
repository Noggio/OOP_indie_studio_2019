cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-O3 -g3 -ldl")
set(CMAKE_CXX_STANDARD_REQUIRED True)
SET(GCC_COVERAGE_COMPILE_FLAGS "-mno-ms-bitfields")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_required")
if(WIN32)
    set(SFML_ROOT "C:\\SFML")
endif(WIN32)

project(bomberman)

link_directories(/usr/local/lib)

if (WIN32)
    find_package(Irrlicht REQUIRED)
    find_package(SFML REQUIRED network audio graphics window system)
else (WIN32)
    if (UNIX AND NOT APPLE)
        find_package(Irrlicht REQUIRED)
    endif(UNIX AND NOT APPLE)
    if(APPLE)
        find_path(Irrlicht_INCLUDE_DIRS irrlicht/irrlicht.h)
        find_library(IRRLICHT_LIBRARY Irrlicht REQUIRED)
    endif(APPLE)
endif(WIN32)

include_directories(
    ${Irrlicht_INCLUDE_DIRS}
    ${SFML_INCLUDE_DIR}
)

if (WIN32)    
    file(GLOB_RECURSE GAME_SRCS "./SRC/*.cpp" "./lua/*.c")
else(WIN32)
    file(GLOB_RECURSE GAME_SRCS "./SRC/*.cpp")
endif(WIN32)

add_executable(${PROJECT_NAME} ${GAME_SRCS})

set(HEADERS_PROJECT "")
MACRO(HEADER_DIRECTORIES return_list)
    if (WIN32)
        file(GLOB_RECURSE new_list "./SRC/*.hpp" "./lua/*.h")
    else (WIN32)
        file(GLOB_RECURSE new_list "./SRC/*.hpp")
    endif(WIN32)
    set(dir_list "")
    FOREACH(file_path ${new_list})
        get_filename_component(dir_path ${file_path} PATH)

        set(dir_list ${dir_list} " " ${dir_path})

    ENDFOREACH()
    list(REMOVE_DUPLICATES dir_list)
    set(${return_list} ${dir_list})
ENDMACRO()
HEADER_DIRECTORIES(HEADERS_PROJECT)

target_include_directories(${PROJECT_NAME} PRIVATE
            ${HEADERS_PROJECT}
            ${Irrlicht_INCLUDE_DIRS}
            ${SFML_INCLUDE_DIR}
)

if (WIN32)
    set(LINK_LIBRARIES ${Irrlicht_LIBRARIES} ${SFML_LIBRARIES})
else (WIN32)
    if (UNIX AND NOT APPLE)
        set(LINK_LIBRARIES
            ${Irrlicht_LIBRARIES} ${CMAKE_DL_LIBS} sfml-graphics sfml-window sfml-system sfml-audio
            )
    endif(UNIX AND NOT APPLE)
    if (APPLE)
        set(LINK_LIBRARIES
            ${IRRLICHT_LIBRARY} ${CMAKE_DL_LIBS} sfml-graphics sfml-window sfml-system sfml-audio)
    endif (APPLE)
endif(WIN32)

target_link_libraries(${PROJECT_NAME} ${LINK_LIBRARIES})