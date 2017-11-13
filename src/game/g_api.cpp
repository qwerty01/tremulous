
#include "lua.hpp"
#include "sol.hpp"

#include "g_local.h"

struct Entity {};

static inline void Api_Init(sol::state&& lua)
{
    lua.new_usertype<gentity_t>("gentity");
}
