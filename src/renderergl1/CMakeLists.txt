set(PARENT_DIR ${CMAKE_SOURCE_DIR}/src)

include_directories(
    ${PARENT_DIR}
    ${CMAKE_SOURCE_DIR}/external/jpeg-8c
    ${SDL2_INCLUDE_DIRS} 
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

    ${CMAKE_SOURCE_DIR}/src/qcommon/cvar.h
    ${CMAKE_SOURCE_DIR}/src/qcommon/json.h
    ${CMAKE_SOURCE_DIR}/src/qcommon/puff.cpp
    ${CMAKE_SOURCE_DIR}/src/qcommon/puff.h
    ${CMAKE_SOURCE_DIR}/src/qcommon/q_math.c
    ${CMAKE_SOURCE_DIR}/src/qcommon/q_shared.c
    ${CMAKE_SOURCE_DIR}/src/qcommon/qfiles.h
    )

if(NOT USE_RENDERER_DLOPEN)
    add_library(
        renderergl1 STATIC
        ${renderergl1_SRCS}
        )

    target_link_libraries(
        renderergl1 renderercommon
        SDL2::SDL2 
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
        SDL2::SDL2
        )
endif(NOT USE_RENDERER_DLOPEN)
