//
// Created by matthijs on 26-4-18.
//

#include "ci_zebro_heading_sensor.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif


argos::CCI_ZebroHeadingSensor::CCI_ZebroHeadingSensor()
{
    m_cHeading = CRadians();
}

argos::CRadians argos::CCI_ZebroHeadingSensor::GetHeading()
{
    return m_cHeading;
}

void argos::CCI_ZebroHeadingSensor::CreateLuaState(lua_State *pt_lua_state)
{
    CLuaUtility::OpenRobotStateTable(pt_lua_state, "heading");
    CLuaUtility::StartTable(pt_lua_state, 1);
    CLuaUtility::AddToTable(pt_lua_state, "angle",  m_cHeading.GetValue());
    CLuaUtility::EndTable  (pt_lua_state);
    CLuaUtility::CloseRobotStateTable(pt_lua_state);
}

void argos::CCI_ZebroHeadingSensor::ReadingsToLuaState(lua_State *pt_lua_state)
{
    lua_getfield(pt_lua_state, -1, "heading");
    lua_pushnumber(pt_lua_state, 1);
    lua_gettable  (pt_lua_state, -2                  );
    lua_pushnumber(pt_lua_state, m_cHeading.GetValue());
    lua_setfield  (pt_lua_state, -2, "angle"         );
    lua_pop(pt_lua_state, 2);
}
