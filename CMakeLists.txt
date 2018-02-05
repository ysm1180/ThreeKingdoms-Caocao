cmake_minimum_required(VERSION 3.9)
project(newjojogame)

if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    set( PROJECT_ARCH "x64" )
else( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    set( PROJECT_ARCH "x86" )
endif( CMAKE_SIZEOF_VOID_P EQUAL 8 )

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/jojogame)

include_directories(
        ./Library/
        ./Library/ffmpeg/${PROJECT_ARCH}/include)
add_definitions(-DUNICODE)

#WIN SDK LIBS
link_directories(
        "C:/Program Files (x86)/Windows Kits/10/lib/10.0.16299.0/ucrt/x86"
        "C:/Program Files (x86)/Windows Kits/10/lib/10.0.16299.0/um/x86"
        ./Library/ffmpeg/${PROJECT_ARCH}/lib)

add_library(BaseLib STATIC
        Library/BaseLib/ConsoleOutput.cpp
        Library/BaseLib/ConsoleOutput.h
        Library/BaseLib/MemoryPool.cpp
        Library/BaseLib/MemoryPool.h)
add_library(CommonLib STATIC
        Library/CommonLib/GameManager.cpp
        Library/CommonLib/GameManager.h
        Library/CommonLib/GameScriptManager.cpp
        Library/CommonLib/GameScriptManager.h
        Library/CommonLib/MenuManager.cpp
        Library/CommonLib/MenuManager.h)
add_library(LuaLib STATIC
        Library/LuaLib/lapi.c
        Library/LuaLib/lapi.h
        Library/LuaLib/lauxlib.c
        Library/LuaLib/lauxlib.h
        Library/LuaLib/lbaselib.c
        Library/LuaLib/lbitlib.c
        Library/LuaLib/lcode.c
        Library/LuaLib/lcode.h
        Library/LuaLib/lcorolib.c
        Library/LuaLib/lctype.c
        Library/LuaLib/lctype.h
        Library/LuaLib/ldblib.c
        Library/LuaLib/ldebug.c
        Library/LuaLib/ldebug.h
        Library/LuaLib/ldo.c
        Library/LuaLib/ldo.h
        Library/LuaLib/ldump.c
        Library/LuaLib/lfunc.c
        Library/LuaLib/lfunc.h
        Library/LuaLib/lgc.c
        Library/LuaLib/lgc.h
        Library/LuaLib/linit.c
        Library/LuaLib/liolib.c
        Library/LuaLib/llex.c
        Library/LuaLib/llex.h
        Library/LuaLib/llimits.h
        Library/LuaLib/lmathlib.c
        Library/LuaLib/lmem.c
        Library/LuaLib/lmem.h
        Library/LuaLib/loadlib.c
        Library/LuaLib/lobject.c
        Library/LuaLib/lobject.h
        Library/LuaLib/lopcodes.c
        Library/LuaLib/lopcodes.h
        Library/LuaLib/loslib.c
        Library/LuaLib/lparser.c
        Library/LuaLib/lparser.h
        Library/LuaLib/lprefix.h
        Library/LuaLib/lstate.c
        Library/LuaLib/lstate.h
        Library/LuaLib/lstring.c
        Library/LuaLib/lstring.h
        Library/LuaLib/lstrlib.c
        Library/LuaLib/ltable.c
        Library/LuaLib/ltable.h
        Library/LuaLib/ltablib.c
        Library/LuaLib/ltm.c
        Library/LuaLib/ltm.h
        Library/LuaLib/lua.c
        Library/LuaLib/lua.h
        Library/LuaLib/lua.hpp
        Library/LuaLib/lua_tinker.cpp
        Library/LuaLib/lua_tinker.h
        Library/LuaLib/luac.c
        Library/LuaLib/luaconf.h
        Library/LuaLib/lualib.h
        Library/LuaLib/LuaTinker.cpp
        Library/LuaLib/LuaTinker.h
        Library/LuaLib/lundump.c
        Library/LuaLib/lundump.h
        Library/LuaLib/lutf8lib.c
        Library/LuaLib/lvm.c
        Library/LuaLib/lvm.h
        Library/LuaLib/lzio.c
        Library/LuaLib/lzio.h)
add_library(UILib STATIC Library/UILib/BaseControl.cpp
        Library/UILib/BaseControl.h
        Library/UILib/ButtonControl.cpp
        Library/UILib/ButtonControl.h
        Library/UILib/ControlManager.cpp
        Library/UILib/ControlManager.h
        Library/UILib/MenuControl.cpp
        Library/UILib/MenuControl.h
        Library/UILib/MoviePlayerControl.cpp
        Library/UILib/MoviePlayerControl.h
        Library/UILib/TextFont.cpp
        Library/UILib/TextFont.h
        Library/UILib/WindowControl.cpp
        Library/UILib/WindowControl.h)

add_executable(newjojogame WIN32
        jojogame/Application.cpp
        jojogame/Application.h
        jojogame/LuaConsole.cpp
        jojogame/LuaConsole.h
        jojogame/main.cpp
        jojogame/resource.h
        jojogame/jojogame.rc)

set(FFmpeg_ROOT "Library/ffmpeg")
foreach (lib "avformat" "avcodec" "avutil" "swscale")
    # Library files
    find_library(${lib}_LIB_RELEASE "${lib}" PATH ${FFmpeg_ROOT}/${PROJECT_ARCH}/lib/)
    if (NOT ${lib}_LIB_RELEASE)
        message(FATAL_ERROR "Could not find release version of library \"${lib}\"")
    endif()
    find_library(${lib}_LIB_DEBUG "${lib}" PATH ${FFmpeg_ROOT}/${PROJECT_ARCH}/lib/)
    if (NOT ${lib}_LIB_DEBUG)
        message(FATAL_ERROR "Could not find debug version of library \"${lib}\"")
    endif()
    target_link_libraries(${PROJECT_NAME} PUBLIC
            $<$<CONFIG:Release>:${${lib}_LIB_RELEASE}>
            $<$<CONFIG:Debug>:${${lib}_LIB_DEBUG}>
            )

    # Header file
    find_path(${lib}_INCLUDE "lib${lib}/${lib}.h" PATH ${FFmpeg_ROOT}/${PROJECT_ARCH}/include/)
    if (NOT ${lib}_INCLUDE)
        message(FATAL_ERROR "Could not find header file of library \"${lib}\"")
    endif()
    target_include_directories(${PROJECT_NAME} PRIVATE ${${lib}_INCLUDE})
endforeach()

target_link_libraries(UILib vfw32.lib)
target_link_libraries(${PROJECT_NAME} PUBLIC comctl32.lib BaseLib CommonLib UILib LuaLib)