#include "tr_local.h"
#include "tr_model.h"
#include "tr_globals.h"
#include "ref_import.h"


typedef struct
{
	const char *ext;
	qhandle_t (* ModelLoader)( const char * , model_t * );
} modelExtToLoaderMap_t;

// Note that the ordering indicates the order of preference used
// when there are multiple models of different formats available
static const modelExtToLoaderMap_t modelLoaders[ ] =
{
    { "md3", R_RegisterMD3 },
	{ "mdr", R_RegisterMDR },
    { "iqm", R_RegisterIQM }
};

static const uint32_t numModelLoaders = ARRAY_LEN(modelLoaders);


/*
====================
Loads in a model for the given name

Zero will be returned if the model fails to load.
An entry will be retained for failed models as an
optimization to prevent disk rescanning if they are
asked for again.
====================
*/
qhandle_t RE_RegisterModel( const char *name )
{
	bool	orgNameFailed = false;
	model_t* mod;
	int			orgLoader = -1;

	ri.Printf( PRINT_ALL, "RegisterModel: %s. \n", name);

	if ( !name || !name[0] ) {
		ri.Printf( PRINT_WARNING, "RE_RegisterModel: NULL name\n" );
		return 0;
	}

	if ( strlen( name ) >= MAX_QPATH ) {
		ri.Printf( PRINT_WARNING, "Model name exceeds MAX_QPATH\n" );
		return 0;
	}

	//
	// search the currently loaded models
	//
    qhandle_t hModel;

	for ( hModel = 1; hModel < tr.numModels; hModel++ ) {
		mod = tr.models[hModel];
		if ( !strcmp( mod->name, name ) ) {
			if( mod->type == MOD_BAD ) {
				ri.Printf( PRINT_WARNING, "tr.models[%d]->type = MOD_BAD \n", hModel);
				return 0;
			}
			return hModel;
		}
	}


	// allocate a new model_t
	ri.Printf( PRINT_ALL, "Allocate Memory for %s. \n", name);
	if ( ( mod = R_AllocModel() ) == NULL ) {
		ri.Printf( PRINT_WARNING, "RE_RegisterModel: R_AllocModel() failed for '%s'\n", name);
		return 0;
	}

	// only set the name after the model has been successfully loaded
	Q_strncpyz( mod->name, name, sizeof( mod->name ) );

	mod->type = MOD_BAD;
	mod->numLods = 0;

	// load the files

    const char* dot = strrchr(name, '.');
    
    if(dot != NULL)
    {
        if( (dot[1] == 'm') && (dot[2] == 'd') && (dot[3] == '3') )
        {
            hModel = R_RegisterMD3(name, mod);
        }
        else if( (dot[1] == 'm') && (dot[2] == 'd') && (dot[3] == 'r') )
        {
            hModel = R_RegisterMDR(name, mod);
        }
        else if( (dot[1] == 'i') && (dot[2] == 'q') && (dot[3] == 'm') )
        {
            hModel = R_RegisterIQM(name, mod);
        }
        else
        {
        	ri.Printf( PRINT_WARNING, " %s format not support now. \n ", name);
        }
    }
	else    
	{
        ri.Printf( PRINT_WARNING, "RegisterModel: %s without extention. "
            " Try and find a suitable match using all the model formats supported\n", name);

        uint32_t i;
        for( i = 0; i < numModelLoaders; i++ )
        {
            if (i == orgLoader)
                continue;
            
            char altName[ MAX_QPATH * 2 ] = {0};
            snprintf( altName, sizeof (altName), "%s.%s", name, modelLoaders[ i ].ext );

            // Load
            hModel = modelLoaders[ i ].ModelLoader( altName, mod );

            if( hModel )
            {
                if( orgNameFailed )
                {
                    ri.Printf( PRINT_ALL, "WARNING: %s not present, using %s instead\n",
                            name, altName );
                }

                break;
            }
        }
    }
	return hModel;
}
