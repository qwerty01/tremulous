#ifndef G_SYSCALLS_H
#define G_SYSCALLS_H

struct gentity_t;

int PASSFLOAT( float x );
void  trap_Print( const char *fmt );
void  trap_Error( const char *fmt );
int   trap_Milliseconds( void );
int   trap_Argc( void );
void  trap_Argv( int n, char *buffer, int bufferLength );
int   trap_FS_FOpenFile( const char *qpath, fileHandle_t *f, enum FS_Mode mode );
void  trap_FS_Read( void *buffer, int len, fileHandle_t f );
void  trap_FS_Write( const void *buffer, int len, fileHandle_t f );
void  trap_FS_FCloseFile( fileHandle_t f );
int trap_FS_GetFileList(  const char *path, const char *extension, char *listbuf, int bufsize );
void  trap_SendConsoleCommand( int exec_when, const char *text );
void  trap_Cvar_Register( vmCvar_t *cvar, const char *var_name, const char *value, int flags );
void  trap_Cvar_Update( vmCvar_t *cvar );
void trap_Cvar_Set( const char *var_name, const char *value );
int trap_Cvar_VariableIntegerValue( const char *var_name );
void trap_Cvar_VariableStringBuffer( const char *var_name, char *buffer, int bufsize );
void trap_LocateGameData( gentity_t *gEnts, int numGEntities, int sizeofGEntity_t, playerState_t *clients, int sizeofGClient );
void trap_DropClient( int clientNum, const char *reason );
void trap_SendServerCommand( int clientNum, const char *text );
void trap_SetConfigstring( int num, const char *string );
void trap_GetConfigstring( int num, char *buffer, int bufferSize );
void trap_SetConfigstringRestrictions( int num, const clientList_t *clientList );
void trap_GetUserinfo( int num, char *buffer, int bufferSize );
void trap_SetUserinfo( int num, const char *buffer );
void trap_GetServerinfo( char *buffer, int bufferSize );
void trap_SetBrushModel( gentity_t *ent, const char *name );
void trap_Trace( trace_t *results, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int passEntityNum, int contentmask );
void trap_TraceCapsule( trace_t *results, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int passEntityNum, int contentmask );
int trap_PointContents( const vec3_t point, int passEntityNum );
bool trap_InPVS( const vec3_t p1, const vec3_t p2 );
bool trap_InPVSIgnorePortals( const vec3_t p1, const vec3_t p2 );
void trap_AdjustAreaPortalState( gentity_t *ent, bool open );
bool trap_AreasConnected( int area1, int area2 );
void trap_LinkEntity( gentity_t *ent );
void trap_UnlinkEntity( gentity_t *ent );
int trap_EntitiesInBox( const vec3_t mins, const vec3_t maxs, int *list, int maxcount );
bool trap_EntityContact( const vec3_t mins, const vec3_t maxs, const gentity_t *ent );
bool trap_EntityContactCapsule( const vec3_t mins, const vec3_t maxs, const gentity_t *ent );
void trap_GetUsercmd( int clientNum, usercmd_t *cmd );
bool trap_GetEntityToken( char *buffer, int bufferSize );
int trap_RealTime( qtime_t *qtime );
void trap_SnapVector( float *v );
bool trap_Parse_AddGlobalDefine( char *define );
bool trap_Parse_LoadSource( const char *filename );
bool trap_Parse_FreeSource( int handle );
bool trap_Parse_ReadToken( int handle, pc_token_t *pc_token );
bool trap_Parse_SourceFileAndLine( int handle, char *filename, int *line );
void trap_AddCommand( const char *cmdName );
void trap_RemoveCommand( const char *cmdName );
int trap_FS_GetFilteredFiles( const char *path, const char *extension, const char *filter, char *listbuf, int bufsize );

#endif
