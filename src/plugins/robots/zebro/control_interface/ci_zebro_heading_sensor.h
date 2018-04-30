//
// Created by matthijs on 26-4-18.
//

#ifndef ARGOS3_SIMULATOR_CI_ZEBRO_HEADING_SENSOR_H
#define ARGOS3_SIMULATOR_CI_ZEBRO_HEADING_SENSOR_H

namespace argos
{
    class CCI_ZebroHeadingSensor;
}

#include <argos3/core/control_interface/ci_sensor.h>
#include <argos3/core/utility/math/angles.h>

namespace argos
{
    class CCI_ZebroHeadingSensor : public CCI_Sensor
    {



    public:

        /**
         * Class constructor
         */
        CCI_ZebroHeadingSensor();

        /**
         * Class destructor
         */
        virtual ~CCI_ZebroHeadingSensor() = default;

        /**
         * Returns the current heading (relative to north) of the robot.
         * @return      - Deviation from north in degrees.
         */
        CRadians GetHeading();

#ifdef ARGOS_WITH_LUA

        virtual void CreateLuaState(lua_State* pt_lua_state);

        virtual void ReadingsToLuaState(lua_State* pt_lua_state);

#endif

    protected:

        /* This variable stores the heading of the robot. */
        CRadians m_cHeading;

    };
}


#endif //ARGOS3_SIMULATOR_CI_ZEBRO_HEADING_SENSOR_H
