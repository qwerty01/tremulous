set(ENGINE_DIR ${CMAKE_SOURCE_DIR}/src)

add_definitions(
    -Didx64
    )

add_library( asm 
	STATIC 
	#${ENGINE_DIR}/asm/snapvector.asm 
	#${ENGINE_DIR}/asm/float.asm
	${ENGINE_DIR}/asm/vm_x86_64.asm 
	)