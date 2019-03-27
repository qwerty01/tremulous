#include "tr_cvar.h"
#include "ref_import.h"
#include "VKimpl.h"

void vk_recreateSwapChain(void)
{

    ri.Printf( PRINT_ALL, " Recreate swap chain \n");

    if( r_fullscreen->integer )
    {
        ri.Cvar_Set( "r_fullscreen", "0" );
        r_fullscreen->modified = true;
    }
    
    // hasty prevent crash.
    ri.Cmd_ExecuteText (EXEC_NOW, "vid_restart\n");
}
