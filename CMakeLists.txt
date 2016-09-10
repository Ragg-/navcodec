cmake_minimum_required(VERSION 3.6)
project(navcodec)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


include_directories(AFTER
    /usr/include
    /usr/local/include/node
    /usr/local/lib/node_modules/nan
)

set(SOURCE_FILES
    src/navaudiofifo.cpp
    src/navaudiofifo.h
    src/navcodec.cpp
    src/navcodec.h
    src/navcodeccontext.cpp
    src/navcodeccontext.h
    src/navcodecid.cpp
    src/navcodecid.h
    src/navdictionary.cpp
    src/navdictionary.h
    src/navformat.cpp
    src/navformat.h
    src/navframe.cpp
    src/navframe.h
    src/navoutformat.cpp
    src/navoutformat.h
    src/navpixformat.cpp
    src/navpixformat.h
    src/navresample.cpp
    src/navresample.h
    src/navstream.cpp
    src/navstream.h
    src/navsws.cpp
    src/navsws.h
    src/navthumbnail.cpp
    src/navthumbnail.h
    src/navutils.h
    src/relocatemoov.cpp
    src/relocatemoov.h)

add_executable(navcodec ${SOURCE_FILES})