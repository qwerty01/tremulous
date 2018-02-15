/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.
Copyright (C) 2000-2013 Darklegion Development
Copyright (C) 2015-2018 GrangerHub


This file is part of Tremulous.

Tremulous is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 3 of the License,
or (at your option) any later version.

Tremulous is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Tremulous; if not, see <https://www.gnu.org/licenses/>
===========================================================================
*/

#ifdef _WIN32

#include "sys_local.h"

#include "qcommon/cvar.h"
#include "qcommon/q_shared.h"
#include "qcommon/q_platform.h"

#include <windows.h>
#include <lmerr.h>
#include <lmcons.h>
#include <lmwksta.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <direct.h>
#include <io.h>
#include <conio.h>
#include <wincrypt.h>
#include <shlobj.h>
#include <psapi.h>
#include <float.h>

// Used to determine where to store user-specific files
static char homePath[ MAX_OSPATH ] = { 0 };

/*
================
Sys_DefaultHomePath
================
*/
char *Sys_DefaultHomePath( void )
{
    TCHAR szPath[MAX_PATH];

    if(!*homePath && com_homepath)
    {
        if( !SUCCEEDED( SHGetFolderPath( NULL, CSIDL_APPDATA, NULL, 0, szPath ) ) )
        {
            Com_Printf("Unable to detect CSIDL_APPDATA\n");
            return NULL;
        }
        
        Com_sprintf(homePath, sizeof(homePath), "%s%c", szPath, PATH_SEP);

        if(com_homepath->string[0])
            Q_strcat(homePath, sizeof(homePath), com_homepath->string);
        else
            Q_strcat(homePath, sizeof(homePath), HOMEPATH_NAME_WIN);
    }

    return homePath;
}

#endif //!_WIN32
