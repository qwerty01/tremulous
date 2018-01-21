set(PARENT_DIR ${CMAKE_SOURCE_DIR}/src)

if(USE_RENDERER_DLOPEN)
    add_definitions( -DUSE_RENDERER_DLOPEN )
endif(USE_RENDERER_DLOPEN)
 
embed_resources(
    GLSL_SHADERS
    glsl/bokeh_fp.glsl
    glsl/bokeh_vp.glsl
    glsl/calclevels4x_fp.glsl
    glsl/calclevels4x_vp.glsl
    glsl/depthblur_fp.glsl
    glsl/depthblur_vp.glsl
    glsl/dlight_fp.glsl
    glsl/dlight_vp.glsl
    glsl/down4x_fp.glsl
    glsl/down4x_vp.glsl
    glsl/fogpass_fp.glsl
    glsl/fogpass_vp.glsl
    glsl/generic_fp.glsl
    glsl/generic_vp.glsl
    glsl/lightall_fp.glsl
    glsl/lightall_vp.glsl
    glsl/pshadow_fp.glsl
    glsl/pshadow_vp.glsl
    glsl/shadowfill_fp.glsl
    glsl/shadowfill_vp.glsl
    glsl/shadowmask_fp.glsl
    glsl/shadowmask_vp.glsl
    glsl/ssao_fp.glsl
    glsl/ssao_vp.glsl
    glsl/texturecolor_fp.glsl
    glsl/texturecolor_vp.glsl
    glsl/tonemap_fp.glsl
    glsl/tonemap_vp.glsl
    )

set(renderergl2_SRCS
    tr_local.h
    tr_dsa.h
    tr_extramath.h
    tr_extratypes.h
    tr_fbo.h
    tr_postprocess.h
    tr_animation.cpp
    tr_backend.cpp
    tr_bsp.cpp
    tr_cmds.cpp
    tr_curve.cpp
    tr_dsa.cpp
    tr_extensions.cpp
    tr_extramath.cpp
    tr_fbo.cpp
    tr_flares.cpp
    tr_glsl.cpp
    tr_image.cpp
    tr_image_dds.cpp
    tr_init.cpp
    tr_light.cpp
    tr_main.cpp
    tr_marks.cpp
    tr_mesh.cpp
    tr_model.cpp
    tr_model_iqm.cpp
    tr_postprocess.cpp
    tr_scene.cpp
    tr_shade.cpp
    tr_shade_calc.cpp
    tr_shader.cpp
    tr_shadows.cpp
    tr_sky.cpp
    tr_subs.cpp
    tr_surface.cpp
    tr_vbo.cpp
    tr_world.cpp

    ${CMAKE_SOURCE_DIR}/src/qcommon/cvar.h
    ${CMAKE_SOURCE_DIR}/src/qcommon/json.h
    ${CMAKE_SOURCE_DIR}/src/qcommon/puff.cpp
    ${CMAKE_SOURCE_DIR}/src/qcommon/puff.h
    ${CMAKE_SOURCE_DIR}/src/qcommon/q_math.c
    ${CMAKE_SOURCE_DIR}/src/qcommon/q_shared.c
    ${CMAKE_SOURCE_DIR}/src/qcommon/qfiles.h

    ${GLSL_SHADERS}
    )

include_directories(
    ${PARENT_DIR} 
    ${CMAKE_BINARY_DIR}/glsl
    #${PARENT_DIR}/renderercommon 
    ${CMAKE_SOURCE_DIR}/external/embed-resource
    ${CMAKE_SOURCE_DIR}/external/jpeg-8c
    ${CMAKE_SOURCE_DIR}/external/nettle-3.3
    ${SDL2_INCLUDE_DIRS} 
    )

if(NOT USE_RENDERER_DLOPEN)

    add_library( renderer_opengl2 STATIC ${renderergl2_SRCS} )
    target_link_libraries(renderergl2 renderercommon SDL2::SDL2)

else(NOT USE_RENDERER_DLOPEN)

    add_library( renderer_opengl2 SHARED ${renderergl2_SRCS} )
    target_link_libraries( renderer_opengl2 renderercommon ${FRAMEWORKS} ${OPENGL_LIBRARIES} SDL2::SDL2)

endif(NOT USE_RENDERER_DLOPEN)

