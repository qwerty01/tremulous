#include "g_api.h"

#include "lua.hpp"
#include "sol.hpp"

#include "g_local.h"

#include <iostream>

sol::state lua;

struct GEntity {
    GEntity() {
        ent = G_Spawn();
    }

    ~GEntity() {
        free_entity();
    }

    void free_entity()
    {
        G_FreeEntity(ent);
        ent = nullptr;
    }

    void set_classname(std::string name)
    {
        ent->classname = strdup(name.c_str());
        ent->_classname_alloced = qtrue; 
    }
    std::string get_classname()
    { return ent->classname; }

private:
    gentity_t *ent;
};

void Api_Init()
{
    lua.open_libraries(sol::lib::base, sol::lib::package);

	lua.new_usertype<GEntity>(
        "gentity_t", sol::constructors<sol::types<>>(),
        "classname", sol::property(&GEntity::get_classname, &GEntity::set_classname)
		);
}

void Cmd_LuaLoad_f( void )
{
  if( trap_Argc( ) < 2 )
  {
    G_Printf( "usage: lua <name>\n" );
    return;
  }

  std::string s = ConcatArgs( 1 );
  lua.script_file(s);
}
