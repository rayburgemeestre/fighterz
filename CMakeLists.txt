cmake_minimum_required(VERSION 3.10.2)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

set(CMAKE_CXX_STANDARD 20)

project(tachyon-fighterz VERSION 0.1 LANGUAGES C)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set(CLANGFORMAT_EXECUTABLE clang-format-12)

include(ClangFormat)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_link_options(-fuse-ld=gold)
    add_compile_options(-stdlib=libstdc++)
else()
    add_link_options(-Wl,-z,notext)
endif()

add_compile_options(-fno-omit-frame-pointer -mssse3)

# allegro4 has a bunch of deprecated stuff that we don't care about
add_compile_options(-Wno-deprecated-declarations)

if (SANITIZER)
    add_compile_options(-fsanitize=address)
endif()

#if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_compile_options(-O0 -g)
#else()
#    add_compile_options(-O2)
#endif()

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/Fighterz/client"
    "${CMAKE_CURRENT_SOURCE_DIR}/Fighterz/mapmaker"
    "${CMAKE_CURRENT_SOURCE_DIR}/Fighterz/server"
    "${CMAKE_CURRENT_SOURCE_DIR}/Fighterz/common"
)

link_directories("/usr/lib/llvm-12/lib/clang/12.0.0/lib/linux/")

# client

file(GLOB_RECURSE client_SOURCES "Fighterz/client/**.h" "Fighterz/client/**.c" "Fighterz/common/**.h" "Fighterz/common/**.c")

add_executable(client ${client_SOURCES})
target_link_libraries(client PRIVATE -lalleg)
target_link_libraries(client PRIVATE -lalfont)
target_link_libraries(client PRIVATE -lfreetype)
target_link_libraries(client PRIVATE -lm)

# mapmaker

file(GLOB_RECURSE mapmaker_SOURCES "Fighterz/mapmaker/**.h" "Fighterz/mapmaker/**.c" "Fighterz/common/**.h" "Fighterz/common/**.c")
add_executable(mapmaker ${mapmaker_SOURCES})
target_link_libraries(mapmaker PRIVATE -lalleg)

# server

file(GLOB_RECURSE server_SOURCES "Fighterz/server/**.h" "Fighterz/server/**.c" "Fighterz/common/**.h" "Fighterz/common/**.c")
add_executable(server ${server_SOURCES})
target_link_libraries(server PRIVATE -lm)
target_link_libraries(server PRIVATE -lalleg) # depends on DEBUG2

if (SANITIZER)
    target_link_libraries(client PRIVATE -lclang_rt.asan-x86_64)
    target_link_libraries(mapmaker PRIVATE -lclang_rt.asan-x86_64)
    target_link_libraries(server PRIVATE -lclang_rt.asan-x86_64)
endif()

install (TARGETS client DESTINATION bin)
install (TARGETS mapmaker DESTINATION bin)
install (TARGETS server DESTINATION bin)

set(all_sources ${client_SOURCES})
list(APPEND all_sources ${mapmaker_SOURCES})
list(APPEND all_sources ${server_SOURCES})
list(REMOVE_DUPLICATES all_sources)
clangformat_setup(${all_sources})
