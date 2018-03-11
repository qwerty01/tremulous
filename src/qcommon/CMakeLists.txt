add_library (
    common STATIC
    ${ENGINE_DIR}/cm/cm_load.cpp
    ${ENGINE_DIR}/cm/cm_local.h
    ${ENGINE_DIR}/cm/cm_patch.cpp
    ${ENGINE_DIR}/cm/cm_patch.h
    ${ENGINE_DIR}/cm/cm_polylib.cpp
    ${ENGINE_DIR}/cm/cm_polylib.h
    ${ENGINE_DIR}/cm/cm_public.h
    ${ENGINE_DIR}/cm/cm_test.cpp
    ${ENGINE_DIR}/cm/cm_trace.cpp
    cmd.cpp
    common.cpp
    cvar.cpp
    dialog.h
    files.cpp
    huffman.cpp
    ioapi.cpp
    ioapi.h
    json.h
    md4.cpp
    md5.cpp
    msg.cpp
    net_chan.cpp
    net_ip.cpp
    parse.cpp
    puff.cpp
    puff.h
    q3_lauxlib.cpp
    q3_lauxlib.h
    q_math.cpp
    q_platform.h
    q_shared.cpp
    q_shared.h
    qcommon.h
    qfiles.h
    surfaceflags.h
	unistd.h
    unzip.cpp
    unzip.h
    vm.cpp
    vm_interpreted.cpp
    vm_local.hpp
    vm_none.cpp
    vm_powerpc.cpp
    vm_powerpc_asm.cpp
    vm_powerpc_asm.h
    vm_sparc.cpp
    vm_sparc.h
    vm_x86.cpp
)

if(WIN32 AND MSVC)
	if(CMAKE_CL_64)
		add_definitions( -D_CRT_SECURE_NO_WARNINGS -D__AMD64__ -D_AMD64_ -D__WIN64__ -D_WIN64 )
	else()
		add_definitions( -D_WIN32 -DWIN32 )
	endif()
endif()

include_directories( ${RESTCLIENT_INCLUDES_DIR} ${ENGINE_DIR} )

set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14" )
