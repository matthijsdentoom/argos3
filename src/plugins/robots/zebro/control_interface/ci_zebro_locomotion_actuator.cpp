//
// Created by matthijs on 5-5-18.
//

#include "ci_zebro_locomotion_actuator.h"

#ifdef  ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif


namespace argos {

    /****************************************/
    /****************************************/

#ifdef ARGOS_WITH_LUA
    /*
    * The stack must have two values in this order:
    * 1. left wheel speed (a number)
    * 2. right wheel speed (a number)
    */
   int LuaUpdateMovement(lua_State* pt_lua_state) {
      /* Check parameters */
      if(lua_gettop(pt_lua_state) != 2) {
         return luaL_error(pt_lua_state, "robot.locomotion.update_movement() expects 2 arguments");
      }
      luaL_checktype(pt_lua_state, 1, LUA_TNUMBER);
      luaL_checktype(pt_lua_state, 2, LUA_TNUMBER);
      /* Perform action */
      CLuaUtility::GetDeviceInstance<CCI_ZebroLocomotionActuator>(pt_lua_state, "locomotion")->
              UpdateMovement(lua_tonumber(pt_lua_state, 1),
                           lua_tonumber(pt_lua_state, 2));
      return 0;
   }
#endif

    /****************************************/
    /****************************************/

#ifdef ARGOS_WITH_LUA
    void CCI_ZebroLocomotionActuator::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "locomotion");
      CLuaUtility::AddToTable(pt_lua_state, "_instance", this);
      CLuaUtility::AddToTable(pt_lua_state, "update_movement", &LuaUpdateMovement);
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif

    /****************************************/
    /****************************************/

}