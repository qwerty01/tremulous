
#
##    .dMMMb  dMMMMMP dMMMMb  dMP dMP dMMMMMP dMMMMb
##   dMP" VP dMP     dMP.dMP dMP dMP dMP     dMP.dMP
##   VMMMb  dMMMP   dMMMMK" dMP dMP dMMMP   dMMMMK"
## dP .dMP dMP     dMP"AMF  YMvAP" dMP     dMP"AMF
## VMMMP" dMMMMMP dMP dMP    VP"  dMMMMMP dMP dMP
#

add_definitions(
    -DDEDICATED
    -DUSE_LOCAL_HEADERS
    -DPRODUCT_VERSION="1.2.0 pre-release"
    -DUSE_VOIP
    -DNDEBUG
    )

set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/external)
set(PARENT_DIR ${CMAKE_SOURCE_DIR}/src)

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
    ${PARENT_DIR}/sys/sys_win32.cpp
    ${PARENT_DIR}/sys/sys_unix.cpp
    ${PARENT_DIR}/asm/snapvector.c
    )
endif(WIN32)
 
add_executable(
    tremded
    #
    server.h
    #
    sv_ccmds.cpp
    sv_client.cpp
    sv_game.cpp
    sv_init.cpp
    sv_main.cpp
    sv_net_chan.cpp
    sv_snapshot.cpp
    sv_world.cpp
    #
    ${PARENT_DIR}/game/g_public.h

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
    ${PARENT_DIR}/qcommon/msg.cpp
    ${PARENT_DIR}/qcommon/msg.h
    ${PARENT_DIR}/qcommon/net.h
    ${PARENT_DIR}/qcommon/net_chan.cpp
    ${PARENT_DIR}/qcommon/net_ip.cpp
    ${PARENT_DIR}/qcommon/parse.cpp
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

    ${PARENT_DIR}/null/null_client.cpp
    ${PARENT_DIR}/null/null_input.cpp
    ${PARENT_DIR}/null/null_snddma.cpp
    #
    ${PARENT_DIR}/sys/con_log.cpp
    ${PARENT_DIR}/sys/sys_main.cpp
    ${PARENT_DIR}/sys/sys_shared.h
    ${SYS_SOURCES}
    #
    ${EXTERNAL_DIR}/zlib/adler32.c
    ${EXTERNAL_DIR}/zlib/crc32.c
    ${EXTERNAL_DIR}/zlib/inffast.c
    ${EXTERNAL_DIR}/zlib/inflate.c
    ${EXTERNAL_DIR}/zlib/inftrees.c
    ${EXTERNAL_DIR}/zlib/zutil.c
    )

if(APPLE)
 # FIXME Prefixed with "lua" to prevent cmake from doing "-l-framework Cocoa"
 set(FRAMEWORKS "-framework Cocoa -framework Security -framework OpenAL -framework IOKit")
else(APPLE)
 if(UNIX)
  set(SYSLIBS dl rt)
 endif(UNIX)
endif(APPLE)

if(WIN32)
set(SYSLIBS asm wsock32 ws2_32 winmm psapi)
endif(WIN32)

target_link_libraries(
    tremded
    #
    lua
    script_api
    nettle
    zlib
    ${FRAMEWORKS}
    ${SYSLIBS}
    )
    
include_directories(
    ${PARENT_DIR}
    #${PARENT_DIR}/script
    ${EXTERNAL_DIR}/lua-5.3.3/include
    ${EXTERNAL_DIR}/sol
    ${EXTERNAL_DIR}/script/rapidjson
    ${EXTERNAL_DIR}/nettle-3.3
    ${EXTERNAL_DIR}/zlib
    )
