
#ifndef G_API_H
#define G_API_H

#ifdef __cplusplus
extern "C" { 
namespace sol
{
    class state;
};

extern sol::state lua;
#endif

#include "qcommon/q_shared.h"

typedef struct gentity_s gentity_t;
typedef struct gclient_s gclient_t;

void Api_Init();
void Cmd_LuaLoad_f( void );

#ifdef __cplusplus
}
#endif
#endif
