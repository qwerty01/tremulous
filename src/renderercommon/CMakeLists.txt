include(${CMAKE_SOURCE_DIR}/cmake/SDL2.cmake)

set(ENGINE_DIR ${CMAKE_SOURCE_DIR}/src)
set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/external)

include_directories( ${EXTERNAL_DIR}/jpeg-8c ${SDL2_INCLUDE_DIRS} ${ENGINE_DIR} )

add_library(
    renderercommon STATIC
    iqm.h
    qgl.h
    tr_common.h
    tr_font.cpp
    tr_image_bmp.cpp
    tr_image_jpg.cpp
    tr_image_pcx.cpp
    tr_image_png.cpp
    tr_image_tga.cpp
    tr_noise.cpp
    tr_public.h
    tr_types.h
    ${ENGINE_DIR}/sdl/sdl_gamma.cpp
    ${ENGINE_DIR}/sdl/sdl_glimp.cpp
    ${EXTERNAL_DIR}/jpeg-8c/jaricom.c
    ${EXTERNAL_DIR}/jpeg-8c/jcapimin.c
    ${EXTERNAL_DIR}/jpeg-8c/jcapistd.c
    ${EXTERNAL_DIR}/jpeg-8c/jcarith.c
    ${EXTERNAL_DIR}/jpeg-8c/jccoefct.c
    ${EXTERNAL_DIR}/jpeg-8c/jccolor.c
    ${EXTERNAL_DIR}/jpeg-8c/jcdctmgr.c
    ${EXTERNAL_DIR}/jpeg-8c/jchuff.c
    ${EXTERNAL_DIR}/jpeg-8c/jcinit.c
    ${EXTERNAL_DIR}/jpeg-8c/jcmainct.c
    ${EXTERNAL_DIR}/jpeg-8c/jcmarker.c
    ${EXTERNAL_DIR}/jpeg-8c/jcmaster.c
    ${EXTERNAL_DIR}/jpeg-8c/jcomapi.c
    ${EXTERNAL_DIR}/jpeg-8c/jcparam.c
    ${EXTERNAL_DIR}/jpeg-8c/jcprepct.c
    ${EXTERNAL_DIR}/jpeg-8c/jcsample.c
    ${EXTERNAL_DIR}/jpeg-8c/jctrans.c
    ${EXTERNAL_DIR}/jpeg-8c/jdapimin.c
    ${EXTERNAL_DIR}/jpeg-8c/jdapistd.c
    ${EXTERNAL_DIR}/jpeg-8c/jdarith.c
    ${EXTERNAL_DIR}/jpeg-8c/jdatadst.c
    ${EXTERNAL_DIR}/jpeg-8c/jdatasrc.c
    ${EXTERNAL_DIR}/jpeg-8c/jdcoefct.c
    ${EXTERNAL_DIR}/jpeg-8c/jdcolor.c
    ${EXTERNAL_DIR}/jpeg-8c/jddctmgr.c
    ${EXTERNAL_DIR}/jpeg-8c/jdhuff.c
    ${EXTERNAL_DIR}/jpeg-8c/jdinput.c
    ${EXTERNAL_DIR}/jpeg-8c/jdmainct.c
    ${EXTERNAL_DIR}/jpeg-8c/jdmarker.c
    ${EXTERNAL_DIR}/jpeg-8c/jdmaster.c
    ${EXTERNAL_DIR}/jpeg-8c/jdmerge.c
    ${EXTERNAL_DIR}/jpeg-8c/jdpostct.c
    ${EXTERNAL_DIR}/jpeg-8c/jdsample.c
    ${EXTERNAL_DIR}/jpeg-8c/jdtrans.c
    ${EXTERNAL_DIR}/jpeg-8c/jerror.c
    ${EXTERNAL_DIR}/jpeg-8c/jfdctflt.c
    ${EXTERNAL_DIR}/jpeg-8c/jfdctfst.c
    ${EXTERNAL_DIR}/jpeg-8c/jfdctint.c
    ${EXTERNAL_DIR}/jpeg-8c/jidctflt.c
    ${EXTERNAL_DIR}/jpeg-8c/jidctfst.c
    ${EXTERNAL_DIR}/jpeg-8c/jidctint.c
    ${EXTERNAL_DIR}/jpeg-8c/jmemmgr.c
    ${EXTERNAL_DIR}/jpeg-8c/jmemnobs.c
    ${EXTERNAL_DIR}/jpeg-8c/jquant1.c
    ${EXTERNAL_DIR}/jpeg-8c/jquant2.c
    ${EXTERNAL_DIR}/jpeg-8c/jutils.c
    )

add_definitions(
    -DFLOATING_POINT
    -DHAVE_LRINTF
    -DUSE_INTERNAL_JPEG
    -DUSE_RENDERER_DLOPEN
    ${SDL2_DEFINES}
    )
	
if( MSVC ) # Check if we are using the Visual Studio compiler
add_definitions(
    -D_CRT_SECURE_NO_WARNINGS
    -D__AMD64__
    -D_AMD64_
	-D__WIN64__ 
	-D_WIN64
	)
endif()

target_link_libraries( renderercommon ${SDL2_LIBRARIES} )
