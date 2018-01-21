#
##  .o88b. db      d888888b d88888b d8b   db d888888b
## d8P  Y8 88        `88'   88'     888o  88 `~~88~~'
## 8P      88         88    88ooooo 88V8o 88    88
## 8b      88         88    88~~~~~ 88 V8o88    88
## Y8b  d8 88booo.   .88.   88.     88  V888    88
##  `Y88P' Y88888P Y888888P Y88888P VP   V8P    YP
#

#find_package(CURL)
#find_package(OpenGL)
#find_package(OpenAL)
#find_package(SDL2)

add_definitions(
    -DUSE_LOCAL_HEADERS
    -DUSE_RESTCLIENT
    -DUSE_CODEC_OPUS
    -DUSE_OPENAL
    -DUSE_OPENAL_DLOPEN
    -DUSE_INTERNAL_JPEG
    -DUSE_RENDERER_DLOPEN
    -DNDEBUG
    -DPRODUCT_VERSION="1.3.0 alpha"
    ${SDL2_DEFINES}
    )

set(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/external)

if(APPLE)
set(SYS_SOURCES ${PARENT_DIR}/sys/sys_osx.mm)
endif(APPLE)

if(WIN32)
set(SYS_SOURCES
    ${PARENT_DIR}/sys/con_win32.cpp
    ${PARENT_DIR}/sys/sys_win32.cpp
    )
else(WIN32)
set(SYS_SOURCES
    ${PARENT_DIR}/sys/con_tty.cpp
    ${PARENT_DIR}/sys/sys_unix.cpp
    ${PARENT_DIR}/asm/snapvector.c
    )
endif(WIN32)

#if(USE_RESTCLIENT)
#    cl_rest.cpp
#endif(USE_RESTCLIENT)
 
add_executable(
    tremulous
    #
    cl_avi.cpp
    cl_cgame.cpp
    cl_cin.cpp
    cl_console.cpp
    cl_curl.cpp
    cl_input.cpp
    cl_keys.cpp
    cl_main.cpp
    cl_net_chan.cpp
    cl_parse.cpp
    cl_scrn.cpp
    cl_ui.cpp
    cl_updates.cpp
    libmumblelink.cpp
    qal.cpp
    snd_adpcm.cpp
    snd_codec.cpp
    snd_codec_ogg.cpp
    snd_codec_opus.cpp
    snd_codec_wav.cpp
    snd_dma.cpp
    snd_main.cpp
    snd_mem.cpp
    snd_mix.cpp
    snd_openal.cpp
    snd_wavelet.cpp
    client.h
    #
    ${PARENT_DIR}/qcommon/cm_load.cpp
    ${PARENT_DIR}/qcommon/cm_patch.cpp
    ${PARENT_DIR}/qcommon/cm_polylib.cpp
    ${PARENT_DIR}/qcommon/cm_test.cpp
    ${PARENT_DIR}/qcommon/cm_trace.cpp
    ${PARENT_DIR}/qcommon/cmd.cpp
    ${PARENT_DIR}/qcommon/cmd.h
    ${PARENT_DIR}/qcommon/common.cpp
    ${PARENT_DIR}/qcommon/crypto.cpp
    ${PARENT_DIR}/qcommon/crypto.h
    ${PARENT_DIR}/qcommon/cvar.cpp
    ${PARENT_DIR}/qcommon/cvar.h
    ${PARENT_DIR}/qcommon/files.cpp
    ${PARENT_DIR}/qcommon/files.h
    ${PARENT_DIR}/qcommon/huffman.cpp
    ${PARENT_DIR}/qcommon/huffman.h
    ${PARENT_DIR}/qcommon/ioapi.cpp
    ${PARENT_DIR}/qcommon/md4.cpp
    ${PARENT_DIR}/qcommon/md5.cpp
    ${PARENT_DIR}/qcommon/msg.cpp
    ${PARENT_DIR}/qcommon/msg.h
    ${PARENT_DIR}/qcommon/net.h
    ${PARENT_DIR}/qcommon/net_chan.cpp
    ${PARENT_DIR}/qcommon/net_ip.cpp
    ${PARENT_DIR}/qcommon/parse.cpp
    ${PARENT_DIR}/qcommon/puff.cpp
    ${PARENT_DIR}/qcommon/q_math.c
    ${PARENT_DIR}/qcommon/q_platform.h
    ${PARENT_DIR}/qcommon/q_shared.c
    ${PARENT_DIR}/qcommon/q_shared.h
    ${PARENT_DIR}/qcommon/q3_lauxlib.cpp
    ${PARENT_DIR}/qcommon/q3_lauxlib.h
    ${PARENT_DIR}/qcommon/qcommon.h
    ${PARENT_DIR}/qcommon/unzip.cpp
    ${PARENT_DIR}/qcommon/vm.cpp
    ${PARENT_DIR}/qcommon/vm.h
    ${PARENT_DIR}/qcommon/vm_interpreted.cpp
    ${PARENT_DIR}/qcommon/vm_x86.cpp

    #
    ${PARENT_DIR}/sdl/sdl_input.cpp
    ${PARENT_DIR}/sdl/sdl_snd.cpp
    #
    ${PARENT_DIR}/server/server.h
    ${PARENT_DIR}/server/sv_ccmds.cpp
    ${PARENT_DIR}/server/sv_client.cpp
    ${PARENT_DIR}/server/sv_game.cpp
    ${PARENT_DIR}/server/sv_init.cpp
    ${PARENT_DIR}/server/sv_main.cpp
    ${PARENT_DIR}/server/sv_net_chan.cpp
    ${PARENT_DIR}/server/sv_snapshot.cpp
    ${PARENT_DIR}/server/sv_world.cpp
    #
    ${PARENT_DIR}/sys/con_log.cpp

    ${PARENT_DIR}/sys/sys_main.cpp
    ${PARENT_DIR}/sys/sys_shared.h
    ${SYS_SOURCES}

    ${EXTERNAL_DIR}/semver/src/lib/semantic_version_v1.cpp
    ${EXTERNAL_DIR}/semver/src/lib/semantic_version_v2.cpp
    )

if(APPLE)
 # FIXME Prefixed with "lua" to prevent cmake from doing "-l-framework Cocoa"
 set(SYSLIBS "-framework Cocoa -framework Security -framework OpenAL -framework IOKit")
else(APPLE)
 if(UNIX)
  set(SYSLIBS dl rt)
 endif(UNIX)
endif(APPLE)

if(WIN32)
set(SYSLIBS asm wsock32 ws2_32 winmm psapi)
endif(WIN32)

if(NOT USE_RENDERER_DLOPEN)
  if(USE_OPENGL1)
  set(RENDERER_LIBRARY renderergl1)
  endif(USE_OPENGL1)
  set(RENDERER_LIBRARY renderergl2)
endif(NOT USE_RENDERER_DLOPEN)

target_link_libraries(
    tremulous
    #
    script_api
    #restclient
    SDL2::SDL2
    SDL2::SDL2main
    CURL::libcurl
    OpenAL::OpenAL
    lua
    nettle
    zlib
    ogg
    opus
    opusfile
    ${SYSLIBS}
    ${SDL2_LIBRARIES}
    ${OPENGL_LIBRARIES}
    )

include_directories(
    ${PARENT_DIR}
    ${EXTERNAL_DIR}/jpeg-8c
    ${EXTERNAL_DIR}/libogg-1.3.2/include
    ${EXTERNAL_DIR}/lua-5.3.3/include
    ${EXTERNAL_DIR}/nettle-3.3
    ${EXTERNAL_DIR}/opus-1.1.4/include
    ${EXTERNAL_DIR}/opusfile-0.8/include
    ${EXTERNAL_DIR}/rapidjson
    #${EXTERNAL_DIR}/restclient
    ${EXTERNAL_DIR}/SDL2/include
    ${EXTERNAL_DIR}/semver/src/include
    ${EXTERNAL_DIR}/sol
    ${EXTERNAL_DIR}/zlib
    ${OPENAL_INCLUDE_DIR}
    ${CURL_INCLUDE_DIRS}
	)

add_dependencies( tremulous renderergl1 renderer_opengl2 )
