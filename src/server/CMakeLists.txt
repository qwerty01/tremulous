set(ENGINE_DIR ${CMAKE_SOURCE_DIR}/src)
set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/external)
if(APPLE)
set(SYS_SOURCES ${PARENT_DIR}/sys/sys_osx.mm)
endif(APPLE)

add_definitions(
    -DDEDICATED
    -DUSE_LOCAL_HEADERS
    -DNDEBUG
    )
	
if(WIN32 AND MSVC)
	if(CMAKE_CL_64)
		add_definitions( -D_CRT_SECURE_NO_WARNINGS -D__AMD64__ -D_AMD64_ -D__WIN64__ -D_WIN64 )
	else()
		add_definitions( -D_WIN32 -DWIN32 )
	endif()
endif()


if(WIN32)
set(SYS_SOURCES
    ${ENGINE_DIR}/sys/con_passive
pp
    ${ENGINE_DIR}/sys/sys_win32.cpp
	${ENGINE_DIR}/sys/sys_win32_default_homepath.cpp
	${ENGINE_DIR}/sys/win_resource.h
    )
else(WIN32)
set(SYS_SOURCES
    ${ENGINE_DIR}/sys/con_tty.cpp
    ${ENGINE_DIR}/sys/sys_unix.cpp
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
	
	${ENGINE_DIR}/game/g_public.h
    #
    ${ENGINE_DIR}/cm/cm_load.cpp
    ${ENGINE_DIR}/cm/cm_patch.cpp
    ${ENGINE_DIR}/cm/cm_polylib.cpp
    ${ENGINE_DIR}/cm/cm_test.cpp
    ${ENGINE_DIR}/cm/cm_trace.cpp
    ${ENGINE_DIR}/qcommon/cmd.cpp
	${ENGINE_DIR}/qcommon/cmd.h
    ${ENGINE_DIR}/qcommon/common.cpp
    ${ENGINE_DIR}/qcommon/crypto.cpp
	${ENGINE_DIR}/qcommon/crypto.h
    ${ENGINE_DIR}/qcommon/cvar.cpp
	${ENGINE_DIR}/qcommon/cvar.h
    ${ENGINE_DIR}/qcommon/files.cpp
	${ENGINE_DIR}/qcommon/files.h
    ${ENGINE_DIR}/qcommon/huffman.cpp
    ${ENGINE_DIR}/qcommon/huffman.h
    ${ENGINE_DIR}/qcommon/ioapi.cpp
    ${ENGINE_DIR}/qcommon/md4.cpp
    ${ENGINE_DIR}/qcommon/msg.h
    ${ENGINE_DIR}/qcommon/msg.cpp
    ${ENGINE_DIR}/qcommon/net.h
    ${ENGINE_DIR}/qcommon/net_chan.cpp
    ${ENGINE_DIR}/qcommon/net_ip.cpp
    ${ENGINE_DIR}/qcommon/parse.cpp
    ${ENGINE_DIR}/qcommon/q3_lauxlib.cpp
    ${ENGINE_DIR}/qcommon/q_math.cpp
	${ENGINE_DIR}/qcommon/q_platform.h
    ${ENGINE_DIR}/qcommon/q_shared.cpp
    ${ENGINE_DIR}/qcommon/q_shared.h
    ${ENGINE_DIR}/qcommon/q3_lauxlib.cpp
    ${ENGINE_DIR}/qcommon/q3_lauxlib.h
    ${ENGINE_DIR}/qcommon/qcommon.h
    ${ENGINE_DIR}/qcommon/unzip.cpp
    ${ENGINE_DIR}/qcommon/vm.cpp
	${ENGINE_DIR}/qcommon/vm.h
    ${ENGINE_DIR}/qcommon/vm_interpreted.cpp
    ${ENGINE_DIR}/qcommon/vm_x86.cpp
    #
    ${ENGINE_DIR}/null/null_client.cpp
    ${ENGINE_DIR}/null/null_input.cpp
    ${ENGINE_DIR}/null/null_snddma.cpp
	#
    ${ENGINE_DIR}/sys/con_log.cpp
	${ENGINE_DIR}/sys/con_passive.cpp
    ${ENGINE_DIR}/sys/sys_main.cpp
    ${ENGINE_DIR}/sys/sys_shared.h
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
    
include_directories (
	${ENGINE_DIR}
    ${ENGINE_DIR}/script
    ${EXTERNAL_DIR}/lua-5.3.3/include
    ${EXTERNAL_DIR}/sol
    ${EXTERNAL_DIR}/script/rapidjson
    ${EXTERNAL_DIR}/nettle-3.3
    ${EXTERNAL_DIR}/zlib
)
