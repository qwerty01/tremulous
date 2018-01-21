add_definitions(
    -Didx64
    )

add_library( asm STATIC snapvector.asm ftola.asm vm_x86_64.asm )
