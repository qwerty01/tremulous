set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/external)

add_library( granger_lua STATIC
    ${EXTERNAL_DIR}/lua-5.3.3/src/lapi.c
    ${EXTERNAL_DIR}/lua-5.3.3/src/lcode.c
    ${EXTERNAL_DIR}/lua-5.3.3/src/lctype.c
    ${EXTERNAL_DIR}/lua-5.3.3/src/ldebug.c
    ${EXTERNAL_DIR}/lua-5.3.3/src/ldo.c
    ${EXTERNAL_DIR}/lua-5.3.3/src/ldump.c
    ${EXTERNAL_DIR}/lua-5.3.3/src/lfunc.c
    ${EXTERNAL_DIR}/lua-5.3.3/src/lgc.c
    ${EXTERNAL_DIR}/lua-5.3.3/src/llex.c
    ${EXTERNAL_DIR}/lua-5.3.3/src/lmem.c
    ${EXTERNAL_DIR}/lua-5.3.3/src/lobject.c
    ${EXTERNAL_DIR}/lua-5.3.3/src/lopcodes.c
    ${EXTERNAL_DIR}/lua-5.3.3/src/lparser.c
    ${EXTERNAL_DIR}/lua-5.3.3/src/lstate.c
    ${EXTERNAL_DIR}/lua-5.3.3/src/lstring.c
    ${EXTERNAL_DIR}/lua-5.3.3/src/ltable.c
    ${EXTERNAL_DIR}/lua-5.3.3/src/ltm.c
    ${EXTERNAL_DIR}/lua-5.3.3/src/lundump.c
    ${EXTERNAL_DIR}/lua-5.3.3/src/lvm.c
    ${EXTERNAL_DIR}/lua-5.3.3/src/lzio.c
    ${EXTERNAL_DIR}/lua-5.3.3/src/lauxlib.c
    ${EXTERNAL_DIR}/lua-5.3.3/src/lbaselib.c
    ${EXTERNAL_DIR}/lua-5.3.3/src/lbitlib.c
    ${EXTERNAL_DIR}/lua-5.3.3/src/lcorolib.c
    ${EXTERNAL_DIR}/lua-5.3.3/src/ldblib.c
    ${EXTERNAL_DIR}/lua-5.3.3/src/liolib.c
    ${EXTERNAL_DIR}/lua-5.3.3/src/lmathlib.c
    ${EXTERNAL_DIR}/lua-5.3.3/src/loslib.c
    ${EXTERNAL_DIR}/lua-5.3.3/src/lstrlib.c
    ${EXTERNAL_DIR}/lua-5.3.3/src/ltablib.c
    ${EXTERNAL_DIR}/lua-5.3.3/src/lutf8lib.c
    ${EXTERNAL_DIR}/lua-5.3.3/src/loadlib.c
    ${EXTERNAL_DIR}/lua-5.3.3/src/linit.c
    )

if(APPLE)
  add_definitions(-DLUA_USE_MACOSX)
endif(APPLE)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  add_definitions(-DLUA_USE_LINUX)
endif()

add_definitions (
    -DLUA_COMPAT_5_2
    -DNDEBUG
    -mfpmath=sse
    -ffast-math
    -DGRANGER
    )

include_directories (
    include
    )
