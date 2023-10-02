cmake_minimum_required(VERSION 3.13)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING
        "Build type options: Debug Release RelWithDebInfo MinSizeRel" FORCE)
endif()

project(wibo LANGUAGES C CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(Filesystem REQUIRED)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32 -Wall -fno-pie -no-pie")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32 -Wall -fno-pie -no-pie")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Ttext-segment=0x80000000")

include_directories(.)
add_executable(wibo
    dll/advapi32.cpp
    dll/bcrypt.cpp
    dll/crt.cpp
    dll/kernel32.cpp
    dll/lmgr.cpp
    dll/mscoree.cpp
    dll/msvcrt.cpp
    dll/ntdll.cpp
    dll/ole32.cpp
    dll/user32.cpp
    dll/vcruntime.cpp
    dll/version.cpp
    tlsf/tlsf.c
    alloc.cpp
    files.cpp
    handles.cpp
    loader.cpp
    main.cpp
    processes.cpp
    strutil.cpp
)
target_link_libraries(wibo PRIVATE std::filesystem)
install(TARGETS wibo DESTINATION bin)
