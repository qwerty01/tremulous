#Dushan - many of definitions are missing
add_definitions(
    -DUSE_LOCAL_HEADERS
    -DUSE_RESTCLIENT
    #Dushan - we need this
    -DUSE_CURL_DLOPEN
    -DUSE_CODEC_OPUS
    -DUSE_OPENAL
    -DUSE_OPENAL_DLOPEN
    -DUSE_INTERNAL_JPEG
    -DUSE_RENDERER_DLOPEN
    -DNDEBUG
    )
	
if(WIN32 AND MSVC)
	if(CMAKE_CL_64)
		add_definitions( -D_CRT_SECURE_NO_WARNINGS -D__AMD64__ -D_AMD64_ -D__WIN64__ -D_WIN64 )
	else()
		add_definitions( -D_WIN32 -DWIN32 )
	endif()
endif()

#Dushan - I had problems with this on the Windows platform <3
set(ENGINE_DIR ${CMAKE_SOURCE_DIR}/src)
set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/external)
if(APPLE)
  set(SYS_SOURCES ${ENGINE_DIR}/sys/sys_osx.mm)
endif(APPLE)

if(WIN32)
set(SYS_SOURCES
    ${ENGINE_DIR}/sys/con_win32.cpp
    ${ENGINE_DIR}/sys/sys_win32.cpp
    ${ENGINE_DIR}/sys/sys_win32_default_homepath.cpp
    ${ENGINE_DIR}/sys/win_resource.h
    )
else(WIN32)
set(SYS_SOURCES
    ${ENGINE_DIR}/sys/con_tty.cpp
    ${ENGINE_DIR}/sys/sys_unix.cpp
    ${EXTERNAL_DIR}/semver/src/lib/semantic_version_v1.cpp
    ${EXTERNAL_DIR}/semver/src/lib/semantic_version_v2.cpp 
    )
endif(WIN32)

find_package(CURL)
find_package(OpenGL)
find_package(OpenAL)
find_package(SDL2)

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
    cl_rest.cpp
    cl_scrn.cpp
    cl_ui.cpp
    cl_updates.cpp
    ${ENGINE_DIR}/audio/libmumblelink.cpp
    ${ENGINE_DIR}/audio/qal.cpp
    ${ENGINE_DIR}/audio/snd_adpcm.cpp
    ${ENGINE_DIR}/audio/snd_codec.cpp
    ${ENGINE_DIR}/audio/snd_codec_ogg.cpp
    ${ENGINE_DIR}/audio/snd_codec_opus.cpp
    ${ENGINE_DIR}/audio/snd_codec_wav.cpp
    ${ENGINE_DIR}/audio/snd_dma.cpp
    ${ENGINE_DIR}/audio/snd_main.cpp
    ${ENGINE_DIR}/audio/snd_mem.cpp
    ${ENGINE_DIR}/audio/snd_mix.cpp
    ${ENGINE_DIR}/audio/snd_openal.cpp
    ${ENGINE_DIR}/audio/snd_wavelet.cpp
    client.h
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
    ${ENGINE_DIR}/qcommon/md5.cpp
    ${ENGINE_DIR}/qcommon/msg.cpp
    ${ENGINE_DIR}/qcommon/msg.h
    ${ENGINE_DIR}/qcommon/net.h
    ${ENGINE_DIR}/qcommon/net_chan.cpp
    ${ENGINE_DIR}/qcommon/net_ip.cpp
    ${ENGINE_DIR}/qcommon/net.h
    ${ENGINE_DIR}/qcommon/parse.cpp
    ${ENGINE_DIR}/qcommon/puff.cpp
    ${ENGINE_DIR}/qcommon/q_shared.cpp
    ${ENGINE_DIR}/qcommon/q3_lauxlib.cpp
    ${ENGINE_DIR}/qcommon/q3_lauxlib.h
    ${ENGINE_DIR}/qcommon/q_math.cpp
    ${ENGINE_DIR}/qcommon/qcommon.h
    ${ENGINE_DIR}/qcommon/unzip.cpp
    ${ENGINE_DIR}/qcommon/vm.cpp
    ${ENGINE_DIR}/qcommon/vm_interpreted.cpp
    ${ENGINE_DIR}/qcommon/vm_x86.cpp
    #
    ${ENGINE_DIR}/sdl/sdl_input.cpp
    ${ENGINE_DIR}/sdl/sdl_snd.cpp
    #
    ${ENGINE_DIR}/server/sv_ccmds.cpp
    ${ENGINE_DIR}/server/sv_client.cpp
    ${ENGINE_DIR}/server/sv_game.cpp
    ${ENGINE_DIR}/server/sv_init.cpp
    ${ENGINE_DIR}/server/sv_main.cpp
    ${ENGINE_DIR}/server/sv_net_chan.cpp
    ${ENGINE_DIR}/server/sv_snapshot.cpp
    ${ENGINE_DIR}/server/sv_world.cpp 
    #
    ${ENGINE_DIR}/sys/con_log.cpp
    ${ENGINE_DIR}/sys/con_passive.cpp
    ${ENGINE_DIR}/sys/sys_main.cpp
    ${ENGINE_DIR}/sys/sys_shared.h
    ${SYS_SOURCES}
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
    lua
    nettle
    zlib
    ogg
    opus
    opusfile
    script_api
    restclient
    ${FRAMEWORKS}
    ${CURL_LIBRARIES}
    ${SDL2_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${OPENAL_LIBRARY}
    ${SYSLIBS}
    )

include_directories(
    ${ENGINE_DIR}
    ${ENGINE_DIR}/qcommon
    ${EXTERNAL_DIR}/zlib
    ${EXTERNAL_DIR}/restclient
    ${EXTERNAL_DIR}/rapidjson
    ${EXTERNAL_DIR}/SDL2/include
    ${EXTERNAL_DIR}/jpeg-8c
    ${EXTERNAL_DIR}/libogg-1.3.2/include
    ${EXTERNAL_DIR}/lua-5.3.3/include
    ${EXTERNAL_DIR}/sol
    ${EXTERNAL_DIR}/nettle-3.3
    ${EXTERNAL_DIR}/opus-1.1.4/include
    ${EXTERNAL_DIR}/opusfile-0.8/include
    ${EXTERNAL_DIR}/semver/src/include
    ${SDL2_INCLUDE_DIR}
    ${OPENAL_INCLUDE_DIR}
    ${CURL_INCLUDE_DIR}
    )

if (APPLE AND UNIX)
if (USE_RENDERER_DLOPEN)
    add_custom_command(
        TARGET tremulous
        POST_BUILD COMMAND ${CMAKE_COMMAND}
        ARGS -E copy ${CMAKE_BINARY_DIR}/src/renderergl1/librenderer_opengl1${CMAKE_SHARED_LIBRARY_SUFFIX} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/renderer_opengl1${CMAKE_SHARED_LIBRARY_SUFFIX}
        )

    add_custom_command(
        TARGET tremulous
        POST_BUILD COMMAND ${CMAKE_COMMAND}
        ARGS -E copy ${CMAKE_BINARY_DIR}/src/renderergl2/librenderer_opengl2${CMAKE_SHARED_LIBRARY_SUFFIX} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/renderer_opengl2${CMAKE_SHARED_LIBRARY_SUFFIX}
        )
endif(USE_RENDERER_DLOPEN)
endif(APPLE AND UNIX)
