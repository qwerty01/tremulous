/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.
Copyright (C) 2000-2013 Darklegion Development
Copyright (C) 2015-2019 GrangerHub

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
// cmd.c -- Quake script command processing module

#include "cmd.h"

#include "cvar.h"
#include "files.h"
#include "q_shared.h"
#include "qcommon.h"

#ifndef DEDICATED
#include "client/client.h"
#endif

#define	MAX_CMD_BUFFER  128*1024
#define	MAX_CMD_LINE	1024

typedef struct {
	byte	*data;
	int		maxsize;
	int		cursize;
} cmd_t;

int			cmd_wait;
cmd_t		cmd_text;
byte		cmd_text_buf[MAX_CMD_BUFFER];

//=============================================================================
// From RUST
bool Cmd_ExecuteCommand(const char* command);
bool Cmd_CommandCompletionSafe(void(*callback)(char*));

//=============================================================================

/*
============
Cmd_Wait_f

Causes execution of the remainder of the command buffer to be delayed until
next frame.  This allows commands like:
bind g "cmd use rocket ; +attack ; wait ; -attack ; cmd use blaster"
============
*/
void Cmd_Wait_f( void ) {
	if ( Cmd_Argc() == 2 ) {
		cmd_wait = atoi( Cmd_Argv( 1 ) );
		if ( cmd_wait < 0 )
			cmd_wait = 1; // ignore the argument
	} else {
		cmd_wait = 1;
	}
}


/*
=============================================================================

						COMMAND BUFFER

=============================================================================
*/

/*
============
Cbuf_Init
============
*/
void Cbuf_Init (void)
{
	cmd_text.data = cmd_text_buf;
	cmd_text.maxsize = MAX_CMD_BUFFER;
	cmd_text.cursize = 0;
}

/*
============
Cbuf_AddText

Adds command text at the end of the buffer, does NOT add a final \n
============
*/
void Cbuf_AddText( const char *text ) {
	int		l;
	
	l = strlen (text);

	if (cmd_text.cursize + l >= cmd_text.maxsize)
	{
		Com_Printf ("Cbuf_AddText: overflow\n");
		return;
	}
	::memcpy(&cmd_text.data[cmd_text.cursize], text, l);
	cmd_text.cursize += l;
}


/*
============
Cbuf_InsertText

Adds command text immediately after the current command
Adds a \n to the text
============
*/
void Cbuf_InsertFmtText( const char *fmt, ... ) {
	int		len;
	int		i;

	char text[MAXPRINTMSG];

    va_list args;
    va_start(args, fmt);
    Q_vsnprintf(text, sizeof(text), fmt, args);
    va_end(args);

	len = strlen( text ) + 1;
	if ( len + cmd_text.cursize > cmd_text.maxsize ) {
		Com_Printf( "Cbuf_InsertText overflowed\n" );
		return;
	}

	// move the existing command text
	for ( i = cmd_text.cursize - 1 ; i >= 0 ; i-- ) {
		cmd_text.data[ i + len ] = cmd_text.data[ i ];
	}

	// copy the new text in
	::memcpy( cmd_text.data, text, len - 1 );

	// add a \n
	cmd_text.data[ len - 1 ] = '\n';

	cmd_text.cursize += len;
}

void Cbuf_InsertText( const char *text ) {
	int		len;
	int		i;

	len = strlen( text ) + 1;
	if ( len + cmd_text.cursize > cmd_text.maxsize ) {
		Com_Printf( "Cbuf_InsertText overflowed\n" );
		return;
	}

	// move the existing command text
	for ( i = cmd_text.cursize - 1 ; i >= 0 ; i-- ) {
		cmd_text.data[ i + len ] = cmd_text.data[ i ];
	}

	// copy the new text in
	::memcpy( cmd_text.data, text, len - 1 );

	// add a \n
	cmd_text.data[ len - 1 ] = '\n';

	cmd_text.cursize += len;
}

/*
============
Cbuf_ExecuteText
============
*/
void Cbuf_ExecuteText (int exec_when, const char *text)
{
	switch (exec_when)
	{
	case EXEC_NOW:
		if (text && strlen(text) > 0) {
			Com_DPrintf(S_COLOR_YELLOW "EXEC_NOW %s\n", text);
			Cmd_ExecuteString (text);
		} else {
			Cbuf_Execute();
			Com_DPrintf(S_COLOR_YELLOW "EXEC_NOW %s\n", cmd_text.data);
		}
		break;
	case EXEC_INSERT:
		Cbuf_InsertText (text);
		break;
	case EXEC_APPEND:
		Cbuf_AddText (text);
		break;
	default:
		Com_Error (ERR_FATAL, "Cbuf_ExecuteText: bad exec_when");
	}
}

/*
============
Cbuf_Execute
============
*/
void Cbuf_Execute (void)
{
	int		i;
	char	*text;
	char	line[MAX_CMD_LINE];
	int		quotes;

	// This will keep // style comments all on one line by not breaking on
	// a semicolon.  It will keep /* ... */ style comments all on one line by not
	// breaking it for semicolon or newline.
	bool in_star_comment = false;
	bool in_slash_comment = false;
	while (cmd_text.cursize)
	{
		if ( cmd_wait > 0 ) {
			// skip out while text still remains in buffer, leaving it
			// for next frame
			cmd_wait--;
			break;
		}

		// find a \n or ; line break or comment: // or /* */
		text = (char *)cmd_text.data;

		quotes = 0;
		for (i=0 ; i< cmd_text.cursize ; i++)
		{
			if (text[i] == '"')
				quotes++;

			if ( !(quotes&1)) {
				if (i < cmd_text.cursize - 1) {
					if (! in_star_comment && text[i] == '/' && text[i+1] == '/')
						in_slash_comment = true;
					else if (! in_slash_comment && text[i] == '/' && text[i+1] == '*')
						in_star_comment = true;
					else if (in_star_comment && text[i] == '*' && text[i+1] == '/') {
						in_star_comment = false;
						// If we are in a star comment, then the part after it is valid
						// Note: This will cause it to NUL out the terminating '/'
						// but ExecuteString doesn't require it anyway.
						i++;
						break;
					}
				}
				if (! in_slash_comment && ! in_star_comment && text[i] == ';')
					break;
			}
			if (! in_star_comment && (text[i] == '\n' || text[i] == '\r')) {
				in_slash_comment = false;
				break;
			}
		}

		if( i >= (MAX_CMD_LINE - 1)) {
			i = MAX_CMD_LINE - 1;
		}
				
		::memcpy (line, text, i);
		line[i] = 0;
		
// delete the text from the command buffer and move remaining commands down
// this is necessary because commands (exec) can insert data at the
// beginning of the text buffer

		if (i == cmd_text.cursize)
			cmd_text.cursize = 0;
		else
		{
			i++;
			cmd_text.cursize -= i;
			memmove (text, text+i, cmd_text.cursize);
		}

// execute the command line

		Cmd_ExecuteString (line);		
	}
}


/*
==============================================================================

						SCRIPT COMMANDS

==============================================================================
*/


/*
===============
Cmd_Exec_f
===============
*/
void Cmd_Exec_f( void ) {
	bool quiet;
	union {
		char	*c;
		void	*v;
	} f;
	char	filename[MAX_QPATH];

	quiet = !Q_stricmp(Cmd_Argv(0), "execq");

	if (Cmd_Argc () != 2) {
		Com_Printf ("exec%s <filename> : execute a script file%s\n",
		            quiet ? "q" : "", quiet ? " without notification" : "");
		return;
	}

	Q_strncpyz( filename, Cmd_Argv(1), sizeof( filename ) );
	COM_DefaultExtension( filename, sizeof( filename ), ".cfg" );
	FS_ReadFile( filename, &f.v);
	if (!f.c) {
		Com_Printf ("couldn't exec %s\n", filename);
		return;
	}
	if (!quiet)
		Com_Printf ("execing %s\n", filename);
	
	Cbuf_InsertText (f.c);

	FS_FreeFile (f.v);
}


/*
===============
Cmd_Vstr_f

Inserts the current value of a variable as command text
===============
*/
void Cmd_Vstr_f( void ) {
	if (Cmd_Argc() != 2) {
		Com_Printf ("vstr <variablename> : execute a variable command\n");
		return;
	}

	const char* v = Cvar_VariableString( Cmd_Argv( 1 ) );
	Cbuf_InsertFmtText( "%s\n", v );
}


/*
===============
Cmd_Echo_f

Just prints the rest of the line to the console
===============
*/
void Cmd_Echo_f (void)
{
	Com_Printf ("%s\n", Cmd_Args());
}


/*
=============================================================================

					COMMAND EXECUTION

=============================================================================
*/

struct cmd_function_t
{
	cmd_function_t	*next;
	char			*name;
	xcommand_t		function;
	completionFunc_t complete;
};


typedef struct cmdContext_s
{
	int		argc;
	char	*argv[ MAX_STRING_TOKENS ];	// points into cmd.tokenized
	char	tokenized[ BIG_INFO_STRING + MAX_STRING_TOKENS ];	// will have 0 bytes inserted
	char	cmd[ BIG_INFO_STRING ]; // the original command we received (no token processing)
} cmdContext_t;

static cmdContext_t		cmd;
static cmdContext_t		savedCmd;
//static cmd_function_t	*cmd_functions;		// possible commands to execute

/*
============
Cmd_SaveCmdContext
============
*/
void Cmd_SaveCmdContext( void )
{
	::memcpy( &savedCmd, &cmd, sizeof( cmdContext_t ) );
}

/*
============
Cmd_RestoreCmdContext
============
*/
void Cmd_RestoreCmdContext( void )
{
	::memcpy( &cmd, &savedCmd, sizeof( cmdContext_t ) );
}

/*
============
Cmd_Argc
============
*/
int		Cmd_Argc( void ) {
	return cmd.argc;
}

/*
============
Cmd_Argv
============
*/
const char* Cmd_Argv( int arg ) {
	if ( arg >= cmd.argc ) {
		return (char*)"\0";
	}
	return cmd.argv[arg];	
}

/*
============
Cmd_ArgvBuffer

The interpreted versions use this because
they can't have pointers returned to them
============
*/
void	Cmd_ArgvBuffer( int arg, char *buffer, int bufferLength ) {
	Q_strncpyz( buffer, Cmd_Argv( arg ), bufferLength );
}


/*
============
Cmd_Args

Returns a single string containing argv(1) to argv(argc()-1)
============
*/
char	*Cmd_Args( void ) {
	static	char		cmd_args[MAX_STRING_CHARS];
	int		i;

	cmd_args[0] = 0;
	for ( i = 1 ; i < cmd.argc ; i++ ) {
		strcat( cmd_args, cmd.argv[i] );
		if ( i != cmd.argc-1 ) {
			strcat( cmd_args, " " );
		}
	}

	return cmd_args;
}

/*
============
Cmd_Args

Returns a single string containing argv(arg) to argv(argc()-1)
============
*/
char *Cmd_ArgsFrom( int arg ) {
	static	char		cmd_args[BIG_INFO_STRING];
	int		i;

	cmd_args[0] = 0;
	if (arg < 0)
		arg = 0;
	for ( i = arg ; i < cmd.argc ; i++ ) {
		strcat( cmd_args, cmd.argv[i] );
		if ( i != cmd.argc-1 ) {
			strcat( cmd_args, " " );
		}
	}

	return cmd_args;
}

/*
============
Cmd_ArgsBuffer

The interpreted versions use this because
they can't have pointers returned to them
============
*/
void	Cmd_ArgsBuffer( char *buffer, int bufferLength ) {
	Q_strncpyz( buffer, Cmd_Args(), bufferLength );
}

/*
============
Cmd_LiteralArgsBuffer

The interpreted versions use this because
they can't have pointers returned to them
============
*/
void	Cmd_LiteralArgsBuffer( char *buffer, int bufferLength ) {
	Q_strncpyz( buffer, cmd.cmd, bufferLength );
}

/*
============
Cmd_Cmd

Retrieve the unmodified command string
For rcon use when you want to transmit without altering quoting
https://zerowing.idsoftware.com/bugzilla/show_bug.cgi?id=543
============
*/
char *Cmd_Cmd(void)
{
	return cmd.cmd;
}

/*
============
Cmd_TokenizeString

Parses the given string into command line tokens.
The text is copied to a seperate buffer and 0 characters
are inserted in the apropriate place, The argv array
will point into this temporary buffer.
============
*/
// NOTE TTimo define that to track tokenization issues
//#define TKN_DBG
static void Cmd_TokenizeString2( const char *text_in, bool ignoreQuotes ) {
	const char	*text;
	char	*textOut;

#ifdef TKN_DBG
  // FIXME TTimo blunt hook to try to find the tokenization of userinfo
  Com_DPrintf("Cmd_TokenizeString: %s\n", text_in);
#endif

	// clear previous args
	cmd.argc = 0;
	cmd.cmd[ 0 ] = '\0';

	if ( !text_in ) {
		return;
	}
	
	Q_strncpyz( cmd.cmd, text_in, sizeof(cmd.cmd) );

	text = text_in;
	textOut = cmd.tokenized;

	while ( 1 ) {
		if ( cmd.argc == MAX_STRING_TOKENS ) {
			return;			// this is usually something malicious
		}

		while ( 1 ) {
			// skip whitespace
			while ( *text && *text <= ' ' ) {
				text++;
			}
			if ( !*text ) {
				return;			// all tokens parsed
			}

			// skip // comments
			if ( text[0] == '/' && text[1] == '/' ) {
				return;			// all tokens parsed
			}

			// skip /* */ comments
			if ( text[0] == '/' && text[1] =='*' ) {
				while ( *text && ( text[0] != '*' || text[1] != '/' ) ) {
					text++;
				}
				if ( !*text ) {
					return;		// all tokens parsed
				}
				text += 2;
			} else {
				break;			// we are ready to parse a token
			}
		}

		// handle quoted strings
    // NOTE TTimo this doesn't handle \" escaping
		if ( !ignoreQuotes && *text == '"' ) {
			cmd.argv[cmd.argc] = textOut;
			cmd.argc++;
			text++;
			while ( *text && *text != '"' ) {
				*textOut++ = *text++;
			}
			*textOut++ = 0;
			if ( !*text ) {
				return;		// all tokens parsed
			}
			text++;
			continue;
		}

		// regular token
		cmd.argv[cmd.argc] = textOut;
		cmd.argc++;

		// skip until whitespace, quote, or command
		while ( *text > ' ' ) {
			if ( !ignoreQuotes && text[0] == '"' ) {
				break;
			}

			if ( text[0] == '/' && text[1] == '/' ) {
				break;
			}

			// skip /* */ comments
			if ( text[0] == '/' && text[1] =='*' ) {
				break;
			}

			*textOut++ = *text++;
		}

		*textOut++ = 0;

		if ( !*text ) {
			return;		// all tokens parsed
		}
	}
	
}

/*
============
Cmd_TokenizeString
============
*/
void Cmd_TokenizeString( const char *text_in ) {
	Cmd_TokenizeString2( text_in, false );
}

/*
============
Cmd_TokenizeStringIgnoreQuotes
============
*/
void Cmd_TokenizeStringIgnoreQuotes( const char *text_in ) {
	Cmd_TokenizeString2( text_in, true );
}

/*
============
Cmd_FindCommand
============
*/
//cmd_function_t *Cmd_FindCommand( const char *cmd_name )
//{
//	cmd_function_t *cmd;
//	for( cmd = cmd_functions; cmd; cmd = cmd->next )
//		if( !Q_stricmp( cmd_name, cmd->name ) )
//			return cmd;
//	return nullptr;
//}

/*
============
Cmd_FindCommand
============
*/
//bool Cmd_CommadExists( const char *cmd_name )
//{
//	return Cmd_FindCommand( cmd_name ) ? true : false;
//}

/*
============
Cmd_AddCommand
============
*/
//void	Cmd_AddCommand( const char *cmd_name, xcommand_t function ) {
//	cmd_function_t	*cmd;
//	
//	// fail if the command already exists
//	if( Cmd_FindCommand( cmd_name ) )
//	{
//		// allow completion-only commands to be silently doubled
//		if( function != nullptr )
//			Com_Printf( "Cmd_AddCommand: %s already defined\n", cmd_name );
//		return;
//	}
//
//	// use a small malloc to avoid zone fragmentation
//	cmd = new cmd_function_t;
//	cmd->name = CopyString( cmd_name );
//	cmd->function = function;
//	cmd->complete = nullptr;
//	cmd->next = cmd_functions;
//	cmd_functions = cmd;
//}

/*
============
Cmd_SetCommandCompletionFunc
============
*/
//void Cmd_SetCommandCompletionFunc( const char *command, completionFunc_t complete ) {
//	cmd_function_t	*cmd;
//
//	for( cmd = cmd_functions; cmd; cmd = cmd->next ) {
//		if( !Q_stricmp( command, cmd->name ) ) {
//			cmd->complete = complete;
//			return;
//		}
//	}
//}

/*
============
Cmd_RemoveCommand
============
*/
//void Cmd_RemoveCommand( const char *cmd_name )
//{
//	cmd_function_t	*cmd, **back;
//
//	back = &cmd_functions;
//	for ( ;; )
//    {
//		cmd = *back;
//		if ( !cmd ) {
//			// command wasn't active
//			return;
//		}
//		if ( !strcmp( cmd_name, cmd->name ) ) {
//			*back = cmd->next;
//			if (cmd->name) {
//				Z_Free(cmd->name);
//			}
//			delete cmd;
//			return;
//		}
//		back = &cmd->next;
//	}
//}

/*
============
Cmd_RemoveCommandSafe

Only remove commands with no associated function
============
*/
//void Cmd_RemoveCommandSafe( const char *cmd_name )
//{
//	cmd_function_t *cmd = Cmd_FindCommand( cmd_name );
//
//	if( !cmd )
//		return;
//
//	if( cmd->function )
//	{
//		Com_Error( ERR_DROP, "Restricted source tried to remove system command \"%s\"",
//                cmd_name );
//		return;
//	}
//
//	Cmd_RemoveCommand( cmd_name );
//}

/*
============
Cmd_CommandCompletion
============
*/
//void	Cmd_CommandCompletion( void(*callback)(const char *s) ) {
//	cmd_function_t	*cmd;
//	
//	for (cmd=cmd_functions ; cmd ; cmd=cmd->next) {
//		callback( cmd->name );
//	}
//}

/*
============
Cmd_CompleteArgument
============
*/
//void Cmd_CompleteArgument( const char *command, char *args, int argNum )
//{
//    cmd_function_t	*cmd;
//
//    // FIXIT-H: There needs to be a way to toggle this functionality at runtime
//    // rather than just crashing when a cgame doesn't provide support. #45
//    //  https://github.com/GrangerHub/tremulous/issues/45
//#if 0
//#ifndef DEDICATED
//    // Forward command argument completion to CGAME VM
//    if( cls.cgame && !VM_Call( cls.cgame, CG_CONSOLE_COMPLETARGUMENT, argNum ) )
//#endif
//#endif
//    // Call local completion if VM doesn't pick up
//    for( cmd = cmd_functions; cmd; cmd = cmd->next )
//        if( !Q_stricmp( command, cmd->name ) && cmd->complete )
//            cmd->complete( args, argNum );
//}


/*
============
Cmd_ExecuteString

A complete command line has been parsed, so try to execute it
============
*/
void	Cmd_ExecuteString( const char *text ) {	
	cmd_function_t	*cmdFunc, **prev;

	// execute the command line
	Cmd_TokenizeString( text );		
	if ( !Cmd_Argc() ) {
		return;		// no tokens
	}

	// check registered command functions	
	if ( Cmd_ExecuteCommand ( cmd.argv[0] ) ) {
		return;
	}
	//for ( prev = &cmd_functions ; *prev ; prev = &cmdFunc->next ) {
	//	cmdFunc = *prev;
	//	if ( !Q_stricmp( cmd.argv[0], cmdFunc->name ) ) {
	//		// rearrange the links so that the command will be
	//		// near the head of the list next time it is used
	//		*prev = cmdFunc->next;
	//		cmdFunc->next = cmd_functions;
	//		cmd_functions = cmdFunc;

	//		// perform the action
	//		if ( !cmdFunc->function ) {
	//			// let the cgame or game handle it
	//			break;
	//		} else {
	//			cmdFunc->function ();
	//		}
	//		return;
	//	}
	//}
	
	// check cvars
	if ( Cvar_Command() ) {
		return;
	}

	// check client game commands
	if ( com_cl_running && com_cl_running->integer && CL_GameCommand() ) {
		return;
	}

	// check server game commands
	if ( com_sv_running && com_sv_running->integer && SV_GameCommand() ) {
		return;
	}

	// check ui commands
	if ( com_cl_running && com_cl_running->integer && UI_GameCommand() ) {
		return;
	}

	// send it as a server command if we are connected
	CL_ForwardCommandToServer ( text );
}

/*
============
Cmd_List_f
============
*/
static const char *sg_match;
static size_t sg_count;
static void list_callback(char* name) {
	if (sg_match && !Com_Filter(sg_match, name, false))
		return;
	Com_Printf ("%s\n", name);
	sg_count++;
}
void Cmd_List_f (void)
{
	cmd_function_t* cmd;

	if ( Cmd_Argc() > 1 ) {
		sg_match = Cmd_Argv( 1 );
	} else {
		sg_match = nullptr;
	}

	sg_count = 0;
	Cmd_CommandCompletionSafe(list_callback);
	//for (cmd=cmd_functions ; cmd ; cmd=cmd->next) {
	//	if (match && !Com_Filter(match, cmd->name, false)) continue;

	//	i++;
	//}
	Com_Printf ("%li commands\n", sg_count);
}

/*
==================
Cmd_CompleteCfgName
==================
*/
void Cmd_CompleteCfgName( char *args, int argNum ) {
	if( argNum == 2 ) {
		Field_CompleteFilename( "", "cfg", false, true );
	}
}

/*
============
Cmd_Init
============
*/
void Cmd_Init (void) {
	Cmd_AddCommand ("cmdlist",Cmd_List_f);
	Cmd_AddCommand ("exec",Cmd_Exec_f);
	Cmd_AddCommand ("execq",Cmd_Exec_f);
	Cmd_SetCommandCompletionFunc( "exec", Cmd_CompleteCfgName );
	Cmd_SetCommandCompletionFunc( "execq", Cmd_CompleteCfgName );
	Cmd_AddCommand ("vstr",Cmd_Vstr_f);
	Cmd_SetCommandCompletionFunc( "vstr", Cvar_CompleteCvarName );
	Cmd_AddCommand ("echo",Cmd_Echo_f);
	Cmd_AddCommand ("wait", Cmd_Wait_f);
}
