cmake_minimum_required(VERSION 3.10.2)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

set(CMAKE_CXX_STANDARD 20)

project(tachyon-fighterz VERSION 0.1 LANGUAGES C)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(DEBUG on)  # this is the define that we set for debug
endif()

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=gold")
else()
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-z,notext -Wno-unknown-pragmas")
endif()

if (SANITIZER)
    add_definitions("-fsanitize=address")
endif()

if (DEBUG)
    message("This is a DEBUG build.")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        add_definitions("-std=c++20 -O0 -g -stdlib=libstdc++ -fno-omit-frame-pointer -Wall -mssse3")
    else()
        add_definitions("-std=c++2a -O0 -g                   -fno-omit-frame-pointer -Wall -mssse3 -Wno-unknown-pragmas -Wno-literal-suffix")
    endif()
else()
    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        add_definitions("-std=c++20 -O2 -funsigned-char -stdlib=libstdc++ -Wall -fno-omit-frame-pointer -pedantic -pthread -mssse3 -Wno-reserved-user-defined-literal")
    else()
        add_definitions("-std=c++2a -O2 -funsigned-char                   -Wall -fno-omit-frame-pointer -pedantic -pthread -mssse3 -Wno-reserved-user-defined-literal -Wno-unknown-pragmas -Wno-literal-suffix")
    endif()
endif()

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/Fighterz/client"
    "${CMAKE_CURRENT_SOURCE_DIR}/Fighterz/common"
)

file(GLOB_RECURSE client_SOURCES "Fighterz/client/**.h" "Fighterz/client/**.cpp" "Fighterz/client/**.c" "Fighterz/common/**.h" "Fighterz/common/**.c" "Fighterz/common/**.cpp")

add_executable(client ${client_SOURCES})

target_link_libraries(client PRIVATE -lalleg)
target_link_libraries(client PRIVATE -lalfont)
target_link_libraries(client PRIVATE -lfreetype)
target_link_libraries(client PRIVATE -lm)

install (TARGETS client DESTINATION bin)