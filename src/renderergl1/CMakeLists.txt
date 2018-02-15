include("${CMAKE_SOURCE_DIR}/cmake/SDL2.cmake")

find_package(OpenGL)
find_package(SDL2)

set(ENGINE_DIR ${CMAKE_SOURCE_DIR}/src)
set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/external)

include_directories(
    ${EXTERNAL_DIR}/jpeg-8c
    ${ENGINE_DIR}/renderercommon 
    ${SDL2_INCLUDE_DIR} 
	${ENGINE_DIR}
    )

set(renderergl1_SRCS
    tr_animation.cpp
    tr_backend.cpp
    tr_bsp.cpp
    tr_cmds.cpp
    tr_curve.cpp
    tr_flares.cpp
    tr_image.cpp
    tr_init.cpp
    tr_light.cpp
    tr_local.h
    tr_main.cpp
    tr_marks.cpp
    tr_mesh.cpp
    tr_model.cpp
    tr_model_iqm.cpp
    tr_scene.cpp
    tr_shade.cpp
    tr_shade_calc.cpp
    tr_shader.cpp
    tr_shadows.cpp
    tr_sky.cpp
    tr_subs.cpp
    tr_surface.cpp
    tr_world.cpp
    tr_local.h
    ${ENGINE_DIR}/qcommon/puff.cpp
    ${ENGINE_DIR}/qcommon/q_shared.c
    ${ENGINE_DIR}/qcommon/q_math.c
    )

if(WIN32 AND MSVC)
	if(CMAKE_CL_64)
		add_definitions(
			-D_CRT_SECURE_NO_WARNINGS
			-D__AMD64__
			-D_AMD64_
			-D__WIN64__ 
			-D_WIN64
			)
	else()
		add_definitions(
			-_WIN32
			-WIN32
			)
	endif()
endif()
	
if(NOT USE_RENDERER_DLOPEN)
    add_library(
        renderergl1 STATIC
        ${renderergl1_SRCS}
        )

    target_link_libraries(
        renderergl1 renderercommon
        ${SDL2_LIBRARY}
        )
else(NOT USE_RENDERER_DLOPEN)
    add_library(
        renderergl1 SHARED
        ${renderergl1_SRCS}
        )
    target_link_libraries(
        renderergl1
        renderercommon
        ${FRAMEWORKS}
        ${OPENGL_LIBRARIES}
        ${SDL2_LIBRARY}
        )

endif(NOT USE_RENDERER_DLOPEN)


